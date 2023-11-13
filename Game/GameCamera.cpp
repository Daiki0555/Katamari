#include "stdafx.h"
#include "GameCamera.h"
#include "Player/Sphere.h"
#include "Stick.h"
namespace
{
	const Vector3 CAMERA_POS = { 0.0f,0.0f,-80.0f };
	const Vector3 TARGET_UP = { 0.0f, 20.0f, 0.0f };
	const float	CAMERA_ROTSPEED = 1.0f;
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
	g_camera3D->SetFar(5000.0f);
	return true;
}

void GameCamera::Update()
{
	Rotation();

	//’Ž‹“_‚ÌYÀ•W‚ðã‚°‚é
	m_target = m_sphere->GetPosition();
	m_target += TARGET_UP;
	m_toCameraPos = m_target + m_initialCameraPos;

	
	
	g_camera3D->SetPosition(m_toCameraPos);
	g_camera3D->SetTarget(m_target);
	//XV
	g_camera3D->Update();
}

void GameCamera::Rotation()
{
	float stick = 0.0f;
	if (m_stick->GetStickState() == m_enStick_Right){
		stick= m_stick->GetRStickValue().y;
		//YŽ²Žü‚è‚Ì‰ñ“]
		m_rotation.SetRotationDeg(Vector3::AxisY, -CAMERA_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);

	}
	else if (m_stick->GetStickState() == m_enStick_Left){
		stick = m_stick->GetLStickValue().y;
		//YŽ²Žü‚è‚Ì‰ñ“]
		m_rotation.SetRotationDeg(Vector3::AxisY, CAMERA_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);
	}
	
}