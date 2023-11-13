#include "stdafx.h"
#include "Object.h"
#include "Player/Sphere.h"
#include "Move/MoveFB.h"
#include "Move/MoveLR.h"
#include "Move/MoveUD.h"
#include "Move/MoveRand.h"
namespace {
	const float		HIT_OBJECT = 10.0f;					// ��ɓ�����͈�
}
Object::~Object()
{

}

bool Object::Start()
{
	m_sphere = FindGO<Sphere>("sphere");

	InitCollision();

	return true;
}

void Object::InitObject(const char* objName)
{
	char filepath[256];
	sprintf(filepath,"Assets/modelData/object/%s/%s.tkm", objName, objName);
	m_objectRender.InitDeferredRendering(
		filepath,
		0, 
		0,
		enModelUpAxisZ, 
		true, 
		false);

	m_objectRender.SetPosition(m_position);
	m_objectRender.SetRotation(m_rotation);
	m_objectRender.SetScale(m_scale);
	m_objectRender.Update();
}

void Object::InitCollision()
{

}

void Object::InitMove(
	EnMoveState state,
	const float speed,
	const float range)
{
	switch (state)
	{
	case m_enMove_LR:
		m_objectMove = std::make_unique<MoveFB>();
		break;
	case m_enMove_FB:
		m_objectMove = std::make_unique<MoveLR>();
		break;
	case m_enMove_UD:
		m_objectMove = std::make_unique<MoveUD>();
		break;
	case m_enMove_Rand:
		m_objectMove = std::make_unique<MoveRand>();
		break;
	case m_enMove_No:
		break;
	default:
		break;
	}
	m_objectMove->Init(m_position, m_rotation,speed, range);
	m_moveState = state;

	
}

void Object::Update()
{
	if (m_objectState != m_enObject_NotInvolution)
	{
		CalcMatrix();
	}
	else
	{
		Move();
		Hit();
		m_objectRender.SetPosition(m_position);
		m_objectRender.Update();
	}

	
}

void Object::Move()
{
	m_position = m_objectMove->Move();
}

void Object::Hit()
{
	Vector3 diff = m_sphere->GetPosition() - m_position;
	if (diff.Length() <= HIT_OBJECT)
	{
		Involution();
		m_objectState = m_enObject_Involution;
		m_sphere->AddVolume(m_objData->m_volume);
	}
}

void Object::Involution()
{
	//��i�R�A�j�̋t�s������߂�
	Matrix inverseMatrix;
	inverseMatrix.Inverse(m_sphere->GetSphereModel().GetModel().GetWorldMatrix());

	//�I�u�W�F�N�g�̃��[���h�s��ƃv���C���[�̋t�s�����Z���āA
	//�v���C���[����Ƃ����I�u�W�F�N�g�̃��[�J���s������߂�
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	Matrix mBias, mWorld;
	mBias.MakeRotationX(Math::PI * -0.5f);
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	mRot.Multiply(mBias, mRot);
	mScale.MakeScaling(m_scale);
	mWorld =  mScale * mRot * mTrans;
	//��i�R�A�j���܂ރI�u�W�F�N�g�̃��[���h�s��Ɖ�̋t�s����v�Z����
	m_matInCore.Multiply(mWorld, inverseMatrix);
}

void Object::CalcMatrix()
{
	//�������݌�̃I�u�W�F�N�g�̃��[���h�s��̌v�Z
	m_objectWorldMatrix.Multiply(m_matInCore, m_sphere->GetSphereModel().GetModel().GetWorldMatrix());
	m_objectRender.InvolutionModelsUpdate(m_objectWorldMatrix, enModelUpAxisZ);
}

void Object::Render(RenderContext& rc)
{
	m_objectRender.Draw(rc);
}