#include "stdafx.h"
#include "Object.h"
#include "Player/Sphere.h"
#include "Move/MoveFB.h"
#include "Move/MoveLR.h"
#include "Move/MoveUD.h"
#include "Move/MoveRand.h"
#include "Move/MoveNo.h"
#include "GameUI/ObjectUI.h"
#include "GameUI/FlowerUI.h"
#include "ObjectRender.h"
namespace {
	const float		HIT_OBJECT = 0.0f;					// ��ɓ�����͈�
}
Object::~Object()
{

}

bool Object::Start()
{

	m_sphere = FindGO<Sphere>("sphere");
	m_objectUI = FindGO<ObjectUI>("objectUI");
	m_flowerUI = FindGO<FlowerUI>("flowerUI");
	/// <summary>
	/// �L�[�����Ɏ擾����
	/// </summary>
	/// <returns></returns>
	m_objectRender = FindGO<ObjectRender>(m_objData.m_name.c_str());
	
	
	
	InitCollision();

	return true;
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
		m_objectMove = std::make_unique<MoveNo>();
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
		if (!m_isDraw) {
			m_objectRender->RemoveInstance(m_instanceNo);
		}
	}
	else
	{
		Move();
		Hit();
		m_objectRender->UpdateInstancingData(
			m_instanceNo,
			m_position,
			m_rotation,
			m_scale
			);

	}

	
}

void Object::Move()
{
	m_position = m_objectMove->Move();
}

void Object::Hit()
{
	Vector3 pos = m_sphere->GetPosition();
	Vector3 diff = pos - m_position;
	if (diff.Length() <= m_sphere->GetRadius()){
		if (m_objData.m_involutionScale <= m_sphere->GetRadius()){
			Involution();
			m_objectState = m_enObject_Involution;
			m_sphere->AddVolume(m_objData.m_volume);
			GameManager::GetInstance().
		}
		else {
			m_sphere->IsReflective(true);
		}
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
	m_objectUI->InitWipeModelUI(m_objData);

	//�������܂�SE�̍Đ��@
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(0);
	se->Play(false);
	se->SetVolume(1.0f);
	//�����O�ԈȊO���w�肳��Ă���Ȃ�
	if (m_objData.m_soundNumber!= 0) {
		SoundSource* se2 = NewGO<SoundSource>(0);
		se2->Init(m_objData.m_soundNumber);
		se2->Play(false);
		se2->SetVolume(1.0f);
	}
}

void Object::CalcMatrix()
{
	//�������݌�̃I�u�W�F�N�g�̃��[���h�s��̌v�Z
	m_objectWorldMatrix.Multiply(m_matInCore, m_sphere->GetSphereModel().GetModel().GetWorldMatrix());
	m_objectRender->InvolutionModelsUpdate(m_instanceNo,m_objectWorldMatrix, enModelUpAxisZ);
}

void Object::Render(RenderContext& rc)
{

}