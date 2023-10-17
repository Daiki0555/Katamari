#include "stdafx.h"
#include "Sphere.h"
#include "Player.h"
#include "PlayerManagement.h"
using namespace nsPlayerManagement;
namespace 
{
	const float STANDARDSIZE = 40.0f;					//基準の塊の直径
	
}
Sphere::~Sphere()
{

}

bool Sphere::Start()
{
	m_sphereRender.InitToonModel("Assets/modelData/sphere/sphere.tkm", 0, 0, enModelUpAxisY, true, false, true);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetScale(m_scale);
	m_sphereRender.Update();

	m_charaCon.Init(
		m_radius,
		m_radius,
		m_position
	);



	m_player = FindGO<Player>("player");
	return true;
}

void Sphere::Update()
{
	Move();

	Rotation();
	m_beforePosition = m_player->GetPosition();
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.Update();
}

void Sphere::Move()
{
	//移動速度
	m_moveSpeedMultiply = SPEED * (m_radius / STANDARDSIZE);
	m_position.x = m_player->GetPosition().x;
	m_position.z = m_player->GetPosition().z;
	
}

void Sphere::Rotation()
{
	const float rotationSpeed = 3.0f;//=1.0f*(m_protMoveSpeedMultiply/ m_moveSpeedMultiply);

	Vector3 move = m_player->GetPosition()- m_beforePosition;
	move.y = 0.0f;

	if (fabsf(move.x) < 0.001f
		&& fabsf(move.z) < 0.001f) {
		return;
	}
	
	float length = move.Length();

	//ノーマライズして移動方向の方向ベクトルを求める
	move.Normalize();

	//上記の外積を求める
	Vector3 vertical = Cross(Vector3::AxisY, move);
	//外積ベクトルを元に回転させるクォータニオンを求める
	Quaternion rot;
	rot.SetRotationDeg(vertical, length * rotationSpeed);

	//求めたクォータニオンを乗算する
	m_rotation.Multiply(m_rotation,rot);

}

void Sphere::Render(RenderContext& rc)
{
	m_sphereRender.Draw(rc);
}

