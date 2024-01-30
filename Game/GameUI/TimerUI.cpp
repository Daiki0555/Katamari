#include "stdafx.h"
#include "TimerUI.h"
#include "Game.h"
namespace
{
	//アロースプライト用
	const Vector3 ARROWSPRITE_POSITION = { 750.0f,350.0f,0.0f };
	const Vector3 ARROWSPRITE_SCALE = {0.2f,0.4f,0.2f};
	const Vector2 ARROWSPRITE_PIVOT = { 0.5f,0.0f };
	//クロックの画像用
	const Vector3 CLOCK_POSITION = { 750.0f,350.0f,0.0f };
	const Vector3 CLOCK_SCALE = { 1.0f,1.0f,1.0f };
	const Vector2 CLOCK_PIVOT = { 0.5f,0.5f };
	//タイムフォント用
	const Vector3 TIME_FONT_POSITION = { 500.0f,500.0f,0.0f };
}
TimerUI::~TimerUI()
{

}

bool TimerUI::Start()
{
	//ゲーム取得
	m_game = FindGO<Game>("game");

	m_startTime = GameManager::GetInstance()->GetGameDataStruct().GetTimeLimit();

	//タイマーの初期化
	m_timer = m_startTime * 60.0f;
	//制限時間から円形ゲージの角度を決定する
	m_degree = 360.0f / m_startTime;
	RenderingEngine::GetInstance()->GetSpriteCB().m_degree = (m_degree *3.14) / 180.0f;
	
	//矢印のスプライトを初期化
	m_arrowRender.Init("Assets/sprite/arrow.DDS",208.0f,303.0f);
	m_arrowRender.SetPosition(ARROWSPRITE_POSITION);
	m_arrowRender.SetScale(ARROWSPRITE_SCALE);
	m_arrowRender.SetPivot(ARROWSPRITE_PIVOT);
	m_arrowRender.SetRotation(m_rotation);
	m_arrowRender.Update();

	//クロックの背景スプライトを初期化
	m_clockBackRender.Init("Assets/sprite/clockkari.DDS", 300.0f, 300.0f);
	m_clockBackRender.SetPosition(CLOCK_POSITION);
	m_clockBackRender.SetScale(CLOCK_SCALE);
	m_clockBackRender.SetPivot(CLOCK_PIVOT);
	m_clockBackRender.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 0.5f));
	m_clockBackRender.Update();

	//クロックのリミット画像
	m_clockLimitRender.Init("Assets/sprite/clockkari2.DDS", 300.0f, 300.0f, AlphaBlendMode_Trans,true);
	m_clockLimitRender.SetPosition(CLOCK_POSITION);
	m_clockLimitRender.SetScale(CLOCK_SCALE);
	m_clockLimitRender.SetPivot(CLOCK_PIVOT);
	m_clockLimitRender.Update();
	return true;
}

void TimerUI::Update()
{
	Rotation();
	wchar_t timeText[255];
	swprintf_s(timeText, L"%02d", m_intTime);
	m_timeRender.SetText(timeText);
	m_timeRender.SetPosition(TIME_FONT_POSITION);
	m_timeRender.SetScale(1.50f);
	m_timeRender.SetShadowParam(true, 1.5f, Vector4{ 0.0f,0.0f,0.0f,0.80f });

	//時間がなくなったら
	if (m_intTime <= 0.0f&&
		!m_isProcessing) {
		//時間切れにする
		GameManager::GetInstance()->SetGameSceneState(GameManager::m_enGameState_TimeUp);
		m_isProcessing = true;
	}
}

void TimerUI::Rotation()
{
	m_timer -= g_gameTime->GetFrameDeltaTime();
	m_timer = max(m_timer, -1.0f);
	m_intTime = m_timer/60.0f+1;
	if (!m_isProcessing) {
		m_rot = m_timer / (m_startTime * 60.0f) * 360.0f;
	}
	else {
		m_rot = 0.0f;
	}
	
	m_rotation.SetRotationDegZ(m_rot);
	
	m_arrowRender.SetRotation(m_rotation);
	m_arrowRender.Update();
}

void TimerUI::Render(RenderContext& rc)
{
	m_clockBackRender.Draw(rc);
	m_clockLimitRender.Draw(rc);
	m_arrowRender.Draw(rc);
	m_timeRender.Draw(rc);

}