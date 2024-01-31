#include "stdafx.h"
#include "GameClear.h"
#include "Player/Player.h"
#include "Player/Sphere.h"
#include "GameUI/ModelUI.h"	
#include "GameCamera.h"
#include "Game.h"
#include "Object/Object.h"
namespace
{
	const Vector3 RAIN_POSITION = { 0.0f,550.0f,0.0f };
	const Vector3 RAIN_SCALE = { 1.0f,1.0f,1.0f };
	const Vector2 RAIN_PIVOT = { 0.50f,1.00f };
	const float MAX_SIZE = 1.1f;
	const float MIN_SIZE = 0.01f;
}

GameClear::~GameClear()
{

}

bool GameClear::Start()
{
	//情報の取得
	m_player = FindGO<Player>("player");
	m_sphere = FindGO<Sphere>("sphere");
	m_modelUI = FindGO<ModelUI>("modelUI");
	m_gameCamera = FindGO<GameCamera>("gameCamera");
	m_game = FindGO<Game>("game");

	//虹画像の初期化
	m_rainbowRender.Init("Assets/sprite/ClearUI/niji.DDS", 1920, 1080);
	m_rainbowRender.SetPosition(RAIN_POSITION);
	m_rainbowRender.SetScale(RAIN_SCALE);
	m_rainbowRender.SetPivot(RAIN_PIVOT);
	m_rainbowRender.Update();
	return true;
}

void GameClear::Update()
{
	if (!m_isStart) {
		return;
	}
	ScaleChange();
	m_rainbowRender.Update();
}

void GameClear::ScaleChange()
{
	
	if (!m_isScaleDown) {
		//拡大
		m_scale *= 1.05f;
		m_scale.x = min(m_scale.x, MAX_SIZE);
		m_scale.y = min(m_scale.y, MAX_SIZE);
	}
	else
	{
		m_stopTime -= g_gameTime->GetFrameDeltaTime();
	}

	if (m_stopTime <= 0.0f) {
		m_modelUI->IsDraw(false);
		//縮小
		m_scale *= 0.95f;
		//一定より小さいなら
		if (m_scale.x < MIN_SIZE) {
			m_isEnd = true;
			m_scale.x = min(m_scale.x, 0.0f);
			m_scale.y = min(m_scale.y, 0.0f);
		}
	}

	if (m_scale.x == MAX_SIZE &&
		m_scale.y == MAX_SIZE) {
		m_isScaleDown = true;
		//プレイヤーと塊の描画を止める
		//巻き込まれたオブジェクトはインスタンシング描画しているので座標を下げて消えたように見せる
		m_sphere->GetSphereModel().SetPosition(Vector3{ 0.0f,-1000.0f,0.0f });
		m_sphere->GetSphereModel().Update();
		m_player->IsDraw(false);
		m_gameCamera->SetEndCameraFoward(g_camera3D->GetForward());
	}

	m_rainbowRender.SetScale({ m_scale.x,m_scale.y,1.0f });
}

void GameClear::Render(RenderContext& rc)
{
	if (!m_isStart) {
		return;
	}
	m_rainbowRender.Draw(rc);
}
