#pragma once 
class GameManager : public Noncopyable
{
public:
	enum EnGameSceneState
	{
		m_enGameState_Title,			//�^�C�g��
		m_enGameState_GameStart,		//�Q�[���X�^�[�g��
		m_enGameState_DuringGamePlay,	//�Q�[����
		m_enGameState_TimeUp,			//���Ԑ؂�
		m_enGameState_GameFade,			//�Q�[���t�F�[�h
		m_enGameState_GameClear,		//�Q�[���N���A
		m_enGameState_GameOver,			//�Q�[���I�[�o�[
		m_enGameState_GameBuck,			//���̃��j���[�ɖ߂�
		m_enGameState_GameEnd,			//�Q�[���I��
		m_enGameState_Result
	};
	~GameManager();

	bool Start();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �C���X�^���X�̐����B
	/// </summary>
	static void CreateInstance()
	{
		//���ɐ�������Ă���Ȃ�B
		if (m_instance != nullptr) {
			std::abort();
		}

		m_instance = new GameManager;
		m_instance->Start();
	}

	/// <summary>
	/// �C���X�^���X�̍폜�B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// �C���X�^���X�̎擾�B
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		if (m_instance == nullptr) {
			CreateInstance();
		}

		return m_instance;
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

	/// <summary>
	/// BGM��炷�B
	/// </summary>
	/// <param name="num">�炵�������̔ԍ�</param>
	void SetBGM(const int num);

private:
private:
	EnGameSceneState m_gameSceneState = m_enGameState_Title;
	static GameManager* m_instance;						//�C���X�^���X
	SoundSource* m_bgm = nullptr;						//BGM
};

