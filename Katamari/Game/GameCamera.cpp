#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
namespace
{
	Vector3 CAMERA_POS = { 0.0f,0.0f,-12.0f };
}
GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	m_player = FindGO<Player>("player");

	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(5000.0f);
	return true;
}

void GameCamera::Update()
{
	//注視点のY座標を上げる
	m_target = m_player->GetPosition();
	m_target += Vector3(0.0f, 4.0f, 0.0f);

	m_toCameraPos = m_target + CAMERA_POS;

	g_camera3D->SetPosition(m_toCameraPos);
	g_camera3D->SetTarget(m_target);
	//更新
	g_camera3D->Update();
}