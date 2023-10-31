#include "stdafx.h"
#include "Object.h"
#include "Sphere.h"
namespace {
	const float		HIT_OBJECT = 10.0f;					// 塊に当たる範囲
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
	//塊（コア）の逆行列を求める
	Matrix inverseMatrix;
	inverseMatrix.Inverse(m_sphere->GetSphereModel().GetModel().GetWorldMatrix());

	//オブジェクトのワールド行列とプレイヤーの逆行列を乗算して、
	//プレイヤーを基準としたオブジェクトのローカル行列を求める
	//3dsMaxと軸を合わせるためのバイアス。
	Matrix mBias, mWorld;
	mBias.MakeRotationX(Math::PI * -0.5f);
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	mRot.Multiply(mBias, mRot);
	mScale.MakeScaling(m_scale);
	mWorld =  mScale * mRot * mTrans;
	//塊（コア）を含むオブジェクトのワールド行列と塊の逆行列を計算する
	m_matInCore.Multiply(mWorld, inverseMatrix);
}

void Object::CalcMatrix()
{
	//巻き込み後のオブジェクトのワールド行列の計算
	m_objectWorldMatrix.Multiply(m_matInCore, m_sphere->GetSphereModel().GetModel().GetWorldMatrix());
	m_objectRender.InvolutionModelsUpdate(m_objectWorldMatrix, enModelUpAxisZ);
}

void Object::Render(RenderContext& rc)
{
	m_objectRender.Draw(rc);
}