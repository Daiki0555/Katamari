#pragma once
#include "GameManager.h"
class Player;
class Sphere;
class Object;
class Stick;
class FlowerUI;
class TimerUI;
class GameClear;
class Fade;
class Game :public IGameObject
{
public:
	enum EnGameSceneState
	{
		m_enGameState_GameStart,		//�Q�[���X�^�[�g��
		m_enGameState_DuringGamePlay,	//�Q�[����
		m_enGameState_TimeUp,			//���Ԑ؂�
		m_enGameState_GameFade,			//�Q�[���t�F�[�h
		m_enGameState_GameClear,		//�Q�[���N���A
		m_enGameState_GameOver,			//�Q�[���I�[�o�[
		m_enGameState_GameBuck,			//���̃��j���[�ɖ߂�
		m_enGameState_GameEnd			//�Q�[���I��
	};

	Game();
	~Game();
	bool Start();
	void Update();

	void InitSound();

	/// <summary>
	/// �I�u�W�F�N�g���X�g�̐ݒ�
	/// </summary>
	/// <param name="object"></param>
	/// <returns></returns>
	const void SetObjectList(Object* object)
	{
		m_objctList.emplace_back(object);
	}

	/// <summary>
	/// �I�u�W�F�N�g���X�g�̐ݒ�
	/// </summary>
	/// <returns></returns>
	const std::vector<Object*> GetObjectList()
	{
		return m_objctList;
	}

	/// <summary>
	/// �N���A�\���ǂ����̐ݒ�
	/// </summary>
	/// <returns></returns>
	const void SetClearState(const bool state) 
	{
		m_isGameClearable = state;
	}

	/// <summary>
	/// �Q�[���X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetGameSceneState(const EnGameSceneState state)
	{
		m_gameSceneState = state;
	}

	/// <summary>
	/// �Q�[���X�e�[�g�̎擾
	/// </summary>
	/// <returns></returns>
	const EnGameSceneState GetGameSceneState()const
	{
		return m_gameSceneState;
	}


private:
	/// <summary>
	/// ���x���t�@�C���̏�����
	/// </summary>
	void InitLevel();
	
	/// <summary>
	/// �N���A����
	/// </summary>
	void ClearProcessing();

	/// <summary>
	/// �Q�[���I�[�o�[����
	/// </summary>
	void GameOverProcessing();

	void Render(RenderContext& rc);
private:
	EnGameSceneState m_gameSceneState = m_enGameState_GameStart;
	SpriteRender m_clearRender;
	Player* m_player=nullptr;
	Sphere* m_sphere = nullptr;
	FlowerUI* m_flowerUI = nullptr;
	TimerUI* m_timerUI = nullptr;
	GameClear* m_gameClear = nullptr;
	Fade* m_fade = nullptr;
	bool m_isGameClearable = false;
	bool m_isStartBGM = false;			//BGM���n�܂������ǂ���
	std::vector<Object*>				m_objctList;
};

