#include "stdafx.h"
#include "Object.h"
#include "Sphere.h"
namespace {
	const float		HIT_OBJECT = 10.0f;					// ��ɓ�����͈�
}
Object::~Object()
{

}

bool Object::Start()
{
	m_sphere = FindGO<Sphere>("sphere");

	InitObject();
	InitCollision();

	/*m_physicsObj.CreateFromModel(
		m_objectRender.GetModel(), 
		m_objectRender.GetModel().GetWorldMatrix()
	);*/
	return true;
}

void Object::InitObject()
{
	m_objectRender.InitDeferredRendering(
		"Assets/modelData/object/donut/donut.tkm", 
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

void Object::Update()
{
	if (m_objectState != m_enObject_NotInvolution)
	{
		CalcMatrix();
	}
	else
	{
		
		Hit();
		m_objectRender.Update();
	}

	
}

void Object::Hit()
{
	Vector3 diff = m_sphere->GetPosition() - m_position;
	if (diff.Length() <= HIT_OBJECT)
	{
		Involution();
		m_objectState = m_enObject_Involution;
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