#pragma once
#include "Move/IMove.h"
struct StructMoveState
{
	EnMoveState m_state;		//移動状態
	float m_move;				//オブジェクトの速度
	float m_range;				//オブジェクトの移動範囲の限界
};
class WordManager
{
public:
	void Init();

	StructMoveState SerchMove(const wchar_t* objName);

	/// <summary>
	/// インスタンスの作成
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
	/// インスタンスの削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// インスタンスの取得
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
/// 文字の情報を取得する
/// </summary>
/// <returns></returns>
static inline WordManager* GetMoji()
{
	return WordManager::GetInstance();
}

/// <summary>
/// 名前から移動情報を取得
/// </summary>
/// <param name="moji"></param>
/// <returns></returns>
static inline const StructMoveState SerchMove(const wchar_t* moji)
{
	return GetMoji()->SerchMove(moji);
}