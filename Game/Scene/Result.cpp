#include "stdafx.h"
#include "Result.h"
#include "Scene/Fade.h"
Result::~Result()
{

}

bool Result::Start()
{
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	m_backRender.Init("Assets/sprite/Fade/fade1.DDS", 1920, 1080);
	m_backRender.Update();
	return true;
}

void Result::Update()
{

}

void Result::Render(RenderContext& rc)
{
	m_backRender.Draw(rc);
}