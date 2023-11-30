#include "stdafx.h"
#include "TimerUI.h"
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
	const float GAME_TIME = 5.0f;
}
TimerUI::~TimerUI()
{

}

bool TimerUI::Start()
{
	m_rot = 330.0f/((GAME_TIME-1.0f)*360.0f);
	//タイマーの初期化
	m_timer = GAME_TIME * 60.0f;
	//m_limitTime=

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
	m_clockLimitRender.Init("Assets/sprite/clockkari2.DDS", 300.0f, 300.0f);
	m_clockLimitRender.SetPosition(CLOCK_POSITION);
	m_clockLimitRender.SetScale(CLOCK_SCALE);
	m_clockLimitRender.SetPivot(CLOCK_PIVOT);
	m_clockLimitRender.Update();
	return true;
}

void TimerUI::Update()
{
	Rotation();
}

void TimerUI::Rotation()
{

	m_rotation.AddRotationDegZ(-m_rot);
	m_arrowRender.SetRotation(m_rotation);
	m_arrowRender.Update();
}

void TimerUI::Render(RenderContext& rc)
{
	m_clockBackRender.Draw(rc);
	m_clockLimitRender.Draw(rc);
	m_arrowRender.Draw(rc);

}