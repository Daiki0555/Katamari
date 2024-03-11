#include "stdafx.h"
#include "LineSegment.h"
#include "Physics/CollisionAttr.h"
#include "Player/Sphere.h"
//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;									//�Փ˃t���O�B
	Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//�Փ˓_�B
	Vector3 startPos = Vector3::Zero;				//���C�̎n�_�B
	Vector3 hitNormal = Vector3::Zero;				//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
	float dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
	float playerRadius = 0.0f;							//�v���C���[�̔��a
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
			) {
			//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
		//������Ɩ@���̂Ȃ��p�x�����߂�B
		float angle = hitNormalTmp.y;
		angle = fabsf(acosf(angle));
		if (angle < Math::PI * 0.3f		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //�������̓R���W�����������n�ʂƎw�肳��Ă���B
			) {
			//�Փ˂��Ă���B
			isHit = true;
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//�Փ˓_�̋��������߂�B�B
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
				hitPos = hitPosTmp;
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}
		return 0.0f;
	}
};
//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	Vector3 hitPos;							//�Փ˓_�B
	Vector3 startPos;						//���C�̎n�_�B
	float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
	Vector3 hitNormal;						//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
											//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
			) {
			//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		Vector3 hitNormalTmp;
		hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;

		//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
		float angle = fabsf(acosf(hitNormalTmp.y));
		if (angle >= Math::PI * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//�������̓R���W�����������L�����N�^�Ȃ̂ŕǂƂ݂Ȃ��B
			) {
			isHit = true;
			Vector3 hitPosTmp;
			hitPosTmp = *(Vector3*)&convexResult.m_hitNormalLocal;

			//��_�Ƃ̋����𒲂ׂ�B
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}
		return 0.0f;
	}
};


void LineSegment::Init(const Vector3& pos)
{
	m_collider.Create(m_radius);

	//���̂�������
	RigidBodyInitData rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Init(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//���̂̈ʒu���X�V
	trans.setOrigin(btVector3(pos.x, pos.z, pos.z));
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_User);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
}

void LineSegment::Execute(const Vector3& pos, const Vector3 linesegment)
{
	//���̈ړ���ƂȂ���W���v�Z����B
	Vector3 nextPosition = pos;
	//���x���炱�̃t���[���ł̈ړ��ʂ����߂�B�I�C���[�ϕ��B
	Vector3 addPos = linesegment;
	nextPosition += addPos;
	Vector3 nextPosition2 = nextPosition;

	btRigidBody* btBody = m_rigidBody.GetBody();
	//���̂𓮂����B
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//���̂̈ʒu���X�V�B
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	//�������𒲂ׂ�B
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
		start.setOrigin(btVector3(pos.x, pos.y, pos.z));
		end.setOrigin(btVector3(nextPosition.x, nextPosition.y, nextPosition.z));
		SweepResultGround callback;
		callback.me = m_rigidBody.GetBody();
		callback.startPos = *(Vector3*)&start.getOrigin();
		callback.playerRadius = m_sphere->GetRadius();
		//�Փˌ��o�B
		if (fabsf(nextPosition.y - callback.startPos.y) > FLT_EPSILON) {
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.isHit) {
				nextPosition = callback.hitPos;
			}
		}
		Vector3 vector = nextPosition2 - nextPosition;
		Vector3 pos = m_sphere->GetPosition();
		pos.y -= vector.y;
		m_sphere->SetPosition(pos - Vector3::AxisY * m_sphere->GetRadius());
	}
}