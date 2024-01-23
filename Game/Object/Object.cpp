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
	const float		HIT_OBJECT = 0.0f;					// 塊に当たる範囲
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
	/// キーを元に取得する
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
	m_objectUI->InitWipeModelUI(m_objData);

	//巻き込まれSEの再生　
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(0);
	se->Play(false);
	se->SetVolume(1.0f);
	//もし０番以外が指定されているなら
	if (m_objData.m_soundNumber!= 0) {
		SoundSource* se2 = NewGO<SoundSource>(0);
		se2->Init(m_objData.m_soundNumber);
		se2->Play(false);
		se2->SetVolume(1.0f);
	}
}

void Object::CalcMatrix()
{
	//巻き込み後のオブジェクトのワールド行列の計算
	m_objectWorldMatrix.Multiply(m_matInCore, m_sphere->GetSphereModel().GetModel().GetWorldMatrix());
	m_objectRender->InvolutionModelsUpdate(m_instanceNo,m_objectWorldMatrix, enModelUpAxisZ);
}

void Object::Render(RenderContext& rc)
{

}