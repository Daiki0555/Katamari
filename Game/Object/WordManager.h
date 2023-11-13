#pragma once
#include "Move/IMove.h"
struct StructMoveState
{
	EnMoveState m_state;		//�ړ����
	float m_move;				//�I�u�W�F�N�g�̑��x
	float m_range;				//�I�u�W�F�N�g�̈ړ��͈͂̌��E
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
	static const wchar_t* m_rangeLimit;
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