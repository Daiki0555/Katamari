#include "stdafx.h"
#include "Object.h"
#include "Player/Sphere.h"
#include "Move/MoveFB.h"
#include "Move/MoveLR.h"
#include "Move/MoveUD.h"
#include "Move/MoveRand.h"
namespace {
	const float		HIT_OBJECT = 10.0f;					// 塊に当たる範囲
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