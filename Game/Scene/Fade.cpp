#include "stdafx.h"
#include "Fade.h"
namespace 
{
	const float FADE_MAX = 2.0f;											//ƒtƒF[ƒh‚ÌÅ‘å’lB
	const float FADE_MIN = 0.0f;
}
Fade::~Fade()
{

}

bool Fade::Start()
{
	GameManager::GetInstance();
	//”wŒi‰æ‘œ
	m_backRender.Init("Assets/sprite/Fade/black.DDS", 1920, 1080);
	
	char filepath[256];
	//ƒeƒLƒXƒg‰æ‘œ‚Ì‰Šú‰»
	for (int i = 0; i < START_NUMBER; i++) {
		sprintf(filepath, "Assets/sprite/Fade/fade%d.DDS", i);
		m_startRender[i].Init(filepath, 1920, 1080);
	}
	m_startSpriteNumber = 0;
	return true;
}

void Fade::Update()
{
	switch (m_state){
	case Fade::m_enState_FadeIn:
		m_alpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_alpha <=FADE_MIN){
			m_alpha = FADE_MIN;
			m_state = m_enState_Idle;
		}
		break;
	case Fade::m_enState_FadeOut:
		m_alpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_alpha >= FADE_MAX){
			m_alpha = FADE_MAX;
			m_state = m_enState_Idle;
		}
		break;
	case Fade::m_enState_Idle:
		break;
	}

	if (m_alpha < FADE_MIN) {
		return;
	}
	
	SpriteUpdate();
	FontUpdate();
	
}

void Fade::SpriteUpdate()
{
	Vector4 alpha = Vector4(1.0f, 1.0f, 1.0f, m_alpha);
	//”wŒi‰æ‘œ‚ğİ’è
	m_backRender.SetMulColor(alpha);
	m_backRender.Update();

}

void Fade::FontUpdate()
{
	Vector4 alpha = Vector4(1.0f, 1.0f, 1.0f, m_alpha);
	if (m_isGameStart) {
		m_countUpdateTime -= g_gameTime->GetFrameDeltaTime();
		if (m_countUpdateTime < 0.0f) {
			if (m_startSpriteNumber == 0) {
				//Šª‚«‚Ü‚êSE‚ÌÄ¶@
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(8);
				se->Play(false);
				se->SetVolume(0.5f);
			}
			m_startSpriteNumber++;
			m_startSpriteNumber = min(m_startSpriteNumber, START_NUMBER - 1);
			
			m_countUpdateTime = STRT_NUMBER_UPDATE;
		}
	}
	
	m_startRender[m_startSpriteNumber].SetPosition(Vector3::Zero);
	m_startRender[m_startSpriteNumber].SetMulColor(alpha);
	m_startRender[m_startSpriteNumber].Update();
}

void Fade::Render(RenderContext& rc)
{
	if (m_alpha <= FADE_MIN) {
		return;
	}
	//”wŒi‰æ‘œ‚Ì•`‰æ
	m_backRender.Draw(rc);
	if (m_isGameStart) {
		m_startRender[m_startSpriteNumber].Draw(rc);
	}
}