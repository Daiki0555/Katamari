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
class Result;
class Game :public IGameObject
{
public:
	enum EnGameState
	{
		m_enGameState_GameClear,		//�Q�[���N���A
		m_enGameState_GameOver,			//�Q�[���I�[�o�[
	};

	Game();
	~Game();
	bool Start();
	void Update();

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
	const void SetGameSceneState(const EnGameState state)
	{
		m_gameState = state;
	}

	/// <summary>
	/// �Q�[���X�e�[�g�̎擾
	/// </summary>
	/// <returns></returns>
	const EnGameState GetGameSceneState()const
	{
		return m_gameState;
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
	EnGameState							m_gameState = m_enGameState_GameOver;		//�Q�[���X�e�[�g
	SpriteRender						m_clearRender;
	Player*								m_player = nullptr;
	Sphere*								m_sphere = nullptr;
	FlowerUI*							m_flowerUI = nullptr;
	TimerUI*							m_timerUI = nullptr;
	GameClear*							m_gameClear = nullptr;
	Fade*								m_fade = nullptr;
	Result* m_result = nullptr;
	bool								m_isGameClearable = false;						//�N���A�\���ǂ���
	bool								m_isStartBGM = false;							//BGM���n�܂������ǂ���
	bool								m_isWaitFadeOut = false;						//�t�F�[�h�����ǂ���								
	std::vector<Object*>				m_objctList;
};

