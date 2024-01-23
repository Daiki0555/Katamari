#include "stdafx.h"
#include "GameManager.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

GameManager* GameManager::m_instance = nullptr;

GameManager::~GameManager()
{

}

bool GameManager::Start()
{
	//SE
	//オブジェクト巻き込み時
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/SE/object/obj.wav");
	//クラクション
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/SE/object/carhorn.wav");
	//コイン
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/SE/object/coin.wav");
	//炭酸ドリンク
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/SE/object/drink.wav");
	//馬
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/SE/player/brake.wav");
	//ネズミ
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/SE/object/rat.wav");
	//ブレーキ
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/SE/player/brake.wav");
	//ダッシュ
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/SE/player/dash.wav");
	//会話音
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/SE/fadefont.wav");
	//BGM
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/BGM/BGM1.wav");
	return true;

}

void GameManager::SetBGM(const int num)
{
	if (m_bgm != nullptr) {
		m_bgm->Stop();
	}
	else {
		m_bgm = NewGO<SoundSource>(0);
	}
	m_bgm->Init(num);
	m_bgm->SetVolume(0.5f);
	m_bgm->Play(true);
}
