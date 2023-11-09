#include "stdafx.h"
#include "WordManager.h"

WordManager* WordManager::m_instance = nullptr;
void WordManager::Init()
{
	//左右の移動
	m_move.emplace_back(L"L");
	m_moveState.emplace_back(m_enMove_LR);
	//前後の移動
	m_move.emplace_back(L"F");
	m_moveState.emplace_back(m_enMove_FB);
	//上下の移動
	m_move.emplace_back(L"U");
	m_moveState.emplace_back(m_enMove_UD);
	//ランダムな移動
	m_move.emplace_back(L"R");
	m_moveState.emplace_back(m_enMove_Rand);
	//何もしない
	m_move.emplace_back(L"N");
	m_moveState.emplace_back(m_enMove_No);
}

StructMoveState WordManager::SerchMove(const wchar_t* objName)
{
	const wchar_t* name = objName;
	EnMoveState movestate = m_enMove_No;
	for (int i = 0; i < m_move.size(); i++) {
		//文字列を調べる
		const wchar_t* wcstr = wcsstr(name, m_move[i]);
		if (wcstr != NULL) {
			//ステートを読み込む
			movestate = m_moveState[i];
			return StructMoveState{ movestate };
		}
	}
	return StructMoveState{m_enMove_No};
}