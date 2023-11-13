#pragma once
#include "Move/IMove.h"
struct StructMoveState
{
	EnMoveState state;			//�ړ����
};
class WordManager
{
public:
	void Init();

	StructMoveState SerchMove(const wchar_t* objName);

	/// <summary>
	/// �C���X�^���X�̍쐬
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance != nullptr)
		{
			std::abort();
		}
		m_instance = new WordManager;
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
	static WordManager* GetInstance()
	{
		if (m_instance == nullptr) {
			CreateInstance();
		}

		return m_instance;
	}
public:
private:
	
private:
	static WordManager* m_instance;
	std::vector <const wchar_t*> m_move;
	std::vector <EnMoveState> m_moveState;
};

/// <summary>
/// �����̏����擾����
/// </summary>
/// <returns></returns>
static inline WordManager* GetMoji()
{
	return WordManager::GetInstance();
}

/// <summary>
/// ���O����ړ������擾
/// </summary>
/// <param name="moji"></param>
/// <returns></returns>
static inline const StructMoveState SerchMove(const wchar_t* moji)
{
	return GetMoji()->SerchMove(moji);
}