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
	//�I�u�W�F�N�g�������ݎ�
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/SE/object/obj.wav");
	//�N���N�V����
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/SE/object/carhorn.wav");
	//�R�C��
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/SE/object/coin.wav");
	//�Y�_�h�����N
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/SE/object/drink.wav");
	//�n
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/SE/player/brake.wav");
	//�l�Y�~
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/SE/object/rat.wav");
	//�u���[�L
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/SE/object/carhorn.wav");
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
	m_bgm->SetVolume(1.0f);
	m_bgm->Play(true);
}
