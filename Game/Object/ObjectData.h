#pragma once
/// <summary>
/// �G�N�Z���̃I�u�W�F�N�g�f�[�^
/// </summary>
struct StructObjectData
{
	std::string m_name;			//�I�u�W�F�N�g�f�[�^�̖��O
	std::string m_jName;		//���{��ł̖��O
	float m_volume;				//�I�u�W�F�N�g�̑̐�
	float m_involutionScale;	//�������܂��傫��
	int m_soundNumber;			//SE�̔ԍ�
};

class ObjectData
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �I�u�W�F�N�g�f�[�^��Ԃ�
	/// </summary>
	/// <returns></returns>
	std::vector<StructObjectData> GetObjectData()
	{
		return objects;
	};

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance != nullptr)
		{
			std::abort();
		}
		m_instance = new ObjectData;
		m_instance->Init();
	}

	/// <summary>
	/// �C���X�^���X�̍폜
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	static ObjectData* GetInstance()
	{
		if (m_instance == nullptr) {
			CreateInstance();
		}

		return m_instance;
	}
public:
private:

private:
	std::vector<StructObjectData> objects;
	static ObjectData* m_instance;

};

