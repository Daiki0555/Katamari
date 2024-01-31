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
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/SE/player/brake.wav");
	//�_�b�V��
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/SE/player/dash.wav");
	//��b��
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/SE/fadefont.wav");
	//���U���gSE
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/SE/fanfare.wav");
	//BGM
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/BGM/BGM1.wav");
	//���U���gBGM
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/SE/dram.wav");
	//�h�������[��
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/BGM/space1.wav");
	//�^�C�g��BGM
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/BGM/titleBGM.wav");
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
	m_bgm->SetVolume(0.3f);
	m_bgm->Play(true);
}
