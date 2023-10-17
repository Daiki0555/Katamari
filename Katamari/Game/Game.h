#pragma once

class Player;
class Sphere;
class Game :public IGameObject
{
public:
	enum EnGameSceneState
	{
		m_enGameState_GameStart,		//�Q�[���X�^�[�g��
		m_enGameState_DuringGamePlay,	//�Q�[����
		m_enGameState_GameClearable,	//�N���A�\
		m_enGameState_GameClear,		//�N���A
		m_enGameState_GameFade,			//�Q�[���t�F�[�h
		m_enGameState_GameOver,			//�Q�[���I�[�o�[
		m_enGameState_GameBuck			//���̃��j���[�ɖ߂�
	};

	enum EnGameClearState {
		m_enGameClearState_GameClearable,	//�N���A�\
		m_enGameClearState_GameUnclearable, //�N���A�s�\
	};

	Game();
	~Game();
	bool Start();
	void Update();
public:
private:
	/// <summary>
	/// ���x���t�@�C���̏�����
	/// </summary>
	void InitLevel();
private:
	EnGameClearState m_gameClearState = m_enGameClearState_GameUnclearable;
	Player* m_player=nullptr;
	Sphere* m_sphere = nullptr;
};

