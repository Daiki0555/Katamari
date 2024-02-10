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

	//���U���g�p�̍\����
	struct ResultStruct
	{
	public:
		/// <summary>
		/// �I�u�W�F�N�g�̐����Z������
		/// </summary>
		void AddObjectCount()
		{
			m_objectCount++;
		}

		/// <summary>
		/// �I�u�W�F�N�g�̐��̎擾
		/// </summary>
		/// <returns></returns>
		const int GetObjectCount()const
		{
			return m_objectCount;
		}

		/// <summary>
		/// ��̑傫����ݒ�
		/// </summary>
		/// <param name="scale"></param>
		/// <returns></returns>
		const void SetSphereScale(const float scale)
		{
			m_sphereScale = scale;
		}

		/// <summary>
		/// ��̑傫���̎擾
		/// </summary>
		/// <returns></returns>
		const float GetSphereScale() const
		{
			return m_sphereScale;
		}


		/// <summary>
		/// �ڕW�B�����Ԃ̐ݒ�
		/// </summary>
		/// <param name="time"></param>
		/// <returns></returns>
		const void SetClearTime(const float time)
		{
			m_clearTime = time;
		}

		/// <summary>
		/// �ڕW�B�����Ԃ̎擾
		/// </summary>
		/// <returns></returns>
		const float GetClearTime()const
		{
			return m_clearTime;
		}

		/// <summary>
		/// ���U���g��������
		/// </summary>
		void ResultReset()
		{
			m_sphereScale = 0.0f;
			m_clearTime = 0.0f;
			m_objectCount = 0;
		}
		
	private:
		float					m_sphereScale = 0.0f;		//��̑傫��
		float					m_clearTime = 0.0f;			//�ڕW�B������
		int						m_objectCount = 0;			//�I�u�W�F�N�g�̐�
	};

	//�Q�[���̏��p�̍\����
	struct GameDataStruct
	{
	public:
		/// <summary>
		/// �ڕW�̑傫�����擾
		/// </summary>
		/// <returns></returns>
		const int GetTargetSize()const
		{
			return TARGET_SIZE;
		}

		/// <summary>
		/// �������Ԃ��擾
		/// </summary>
		/// <returns></returns>
		const float GetTimeLimit()const
		{
			return TIME_LIMIT;
		}

	private:
		const int		TARGET_SIZE = 18;					//�ڕW�T�C�Y
		const float		TIME_LIMIT = 3.0f;					//�^�C�����~�b�g(��)
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
	/// BGM���̎擾
	/// </summary>
	/// <returns></returns>
	SoundSource* GetBGM()
	{
		return m_bgm;
	}

	/// <summary>
	/// BGM��炷�B
	/// </summary>
	/// <param name="num">�炵�������̔ԍ�</param>
	void SetBGM(const int num);

	/// <summary>
	/// ���U���g�p�̍\���̂̎擾
	/// </summary>
	/// <returns></returns>
	ResultStruct& GetResultStruct()
	{
		return m_resultStruct;
	}

	/// <summary>
	/// �Q�[���f�[�^�p�̍\���̂̎擾
	/// </summary>
	/// <returns></returns>
	GameDataStruct& GetGameDataStruct()
	{
		return m_gameDataStruct;
	}

private:
	
private:
	ResultStruct			m_resultStruct;
	GameDataStruct			m_gameDataStruct;
	EnGameSceneState		m_gameSceneState = m_enGameState_Title;			//�Q�[���V�[���X�e�[�g
	static GameManager*		m_instance;										//�C���X�^���X
	SoundSource*			m_bgm = nullptr;								//BGM
};

