#include "stdafx.h"
#include "WordManager.h"
const wchar_t* WordManager::m_rangeLimit = L"H";

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
	float move = 0.0, rangeLimit = 0.0f;
	const wchar_t* name = objName;
	EnMoveState movestate = m_enMove_No;
	for (int i = 0; i < m_move.size(); i++) {
		//文字列を調べる
		const wchar_t* wcstr = wcsstr(name, m_move[i]);
		if (wcstr != NULL) {
			//文字列の中の数字を求める
			int wcsf = (int)(wcstr - name+1);
			name += wcsf;
			wchar_t* word;
			//移動速度の取得
			move = wcstof(name, &word);
			//ステートを読み込む
			movestate = m_moveState[i];
			const wchar_t* rwcstr = wcsstr(name,m_rangeLimit);
			if (rwcstr == NULL)
			{
				return StructMoveState{ movestate, move,0.0f };
			}
			//文字列の中の数字を求める
			int rwcsf = (int)(rwcstr - name + 1);
			name += rwcsf;
			//移動範囲を読み込む
			rangeLimit = wcstof(name, &word);
			return StructMoveState{ movestate, move,rangeLimit};
		}
	}
	return StructMoveState{ m_enMove_No,0,0 };
}