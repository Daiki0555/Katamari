#include "stdafx.h"
#include "GameCamera.h"
#include "Player/Sphere.h"
#include "Stick.h"
namespace
{
	const Vector3 CAMERA_POS = { 0.0f,0.0f,-80.0f };
	const Vector3 TARGET_UP = { 0.0f, 20.0f, 0.0f };
	const float	CAMERA_ROTSPEED = 0.2f;
	const float CAMERA_BOTH_ROTSPEED = 0.6f;
}
GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_sphere = FindGO<Sphere>("sphere");
	m_stick = FindGO<Stick>("stick");
	m_initialCameraPos = CAMERA_POS;
	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(7000.0f);
	return true;
}

void GameCamera::Update()
{

	if (!m_isTurningCamera) {
		Rotation();
	}
	

	InvertCamera();

	g_camera3D->SetPosition(m_toCameraPos);
	g_camera3D->SetTarget(m_target);
	//更新
	g_camera3D->Update();
}

void GameCamera::Rotation()
{	
	//ダッシュカウントが2以上の時に回転させない用の座標
	Vector3 dashCountCameraPos = m_initialCameraPos;
	float stick = 0.0f;
	if (m_stick->GetRStickValue().y >= m_stick->GetLStickValue().y) {
		//右ステックと左ステックの入力量の差分を求める
		stick = m_stick->GetRStickValue().y - m_stick->GetLStickValue().y;
		//Y軸周りの回転
		m_rotation.SetRotationDeg(Vector3::AxisY, -CAMERA_BOTH_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);
	}
	else {
		//左ステックと右ステックの入力量の差分を求める
		stick = m_stick->GetLStickValue().y - m_stick->GetRStickValue().y;
		//Y軸周りの回転
		m_rotation.SetRotationDeg(Vector3::AxisY, CAMERA_BOTH_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);
	}


	if (m_stick->GetStickState() == m_enStick_Right){
		stick= m_stick->GetRStickValue().y;
		//Y軸周りの回転
		m_rotation.SetRotationDeg(Vector3::AxisY, -CAMERA_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);

	}
	else if (m_stick->GetStickState() == m_enStick_Left){
		stick = m_stick->GetLStickValue().y;
		m_rotation.SetRotationDeg(Vector3::AxisY, CAMERA_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);
	}

	//ダッシュカウントが1以上なら
	if (m_sphere->GetDashCount() > 1) {
		m_initialCameraPos = dashCountCameraPos;
	}

	//注視点のY座標を上げる
	m_target = m_sphere->GetPosition();
	m_target += TARGET_UP;
	m_toCameraPos = m_target + m_initialCameraPos;
}

void GameCamera::InvertCamera()
{
	//両方のステックが押し込まれたら
	if (g_pad[0]->IsTrigger(enButtonLB3) &&
		g_pad[0]->IsTrigger(enButtonRB3)) {
		m_isTurningCamera = true;

		//Y軸とカメラの見ている方向の外積を求める
		Vector3 Y_up = { 0.0f,1.0f,0.0f };
		m_axis = Cross(Y_up, g_camera3D->GetForward());
	}
	if (m_isTurningCamera) {
		m_turnTimer += g_gameTime->GetFrameDeltaTime();
		m_turnTimer = min(m_turnTimer, 1.0f);
		
		//180°までゆっくり回転させる
		m_degree = 180.0f * g_gameTime->GetFrameDeltaTime();
		m_degree = min(m_degree, 180.0f);

		m_rotation.SetRotationDeg(m_axis, m_degree);
		m_rotation.Apply(m_initialCameraPos);

		//注視点のY座標を上げる
		m_target = m_sphere->GetPosition();
		m_target += TARGET_UP;
		m_toCameraPos = m_target + m_initialCameraPos;
		
		if (m_turnTimer >= 1.0f) {
			m_isTurningCamera = false;
			m_turnTimer = 0.0f;
		}
	}
}