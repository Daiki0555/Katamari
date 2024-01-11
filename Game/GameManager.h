#pragma once 
class GameManager : public Noncopyable
{
public:
	/// <summary>
	/// �Q�[����Ԃ̗񋓌^
	/// </summary>
	enum EnGameState
	{
		m_enState_Title,				//�^�C�g��	
		m_enState_Game,					//�Q�[���V�[��
		m_enState_Result				//���U���g
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
	/// BGM��炷�B
	/// </summary>
	/// <param name="num">�炵�������̔ԍ�</param>
	void SetBGM(const int num);

private:
private:
	static GameManager* m_instance;						//�C���X�^���X
	SoundSource* m_bgm = nullptr;						//BGM
};

