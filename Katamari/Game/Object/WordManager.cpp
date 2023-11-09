#include "stdafx.h"
#include "WordManager.h"

WordManager* WordManager::m_instance = nullptr;
void WordManager::Init()
{
	//���E�̈ړ�
	m_move.emplace_back(L"L");
	m_moveState.emplace_back(m_enMove_LR);
	//�O��̈ړ�
	m_move.emplace_back(L"F");
	m_moveState.emplace_back(m_enMove_FB);
	//�㉺�̈ړ�
	m_move.emplace_back(L"U");
	m_moveState.emplace_back(m_enMove_UD);
	//�����_���Ȉړ�
	m_move.emplace_back(L"R");
	m_moveState.emplace_back(m_enMove_Rand);
	//�������Ȃ�
	m_move.emplace_back(L"N");
	m_moveState.emplace_back(m_enMove_No);
}

StructMoveState WordManager::SerchMove(const wchar_t* objName)
{
	const wchar_t* name = objName;
	EnMoveState movestate = m_enMove_No;
	for (int i = 0; i < m_move.size(); i++) {
		//������𒲂ׂ�
		const wchar_t* wcstr = wcsstr(name, m_move[i]);
		if (wcstr != NULL) {
			//�X�e�[�g��ǂݍ���
			movestate = m_moveState[i];
			return StructMoveState{ movestate };
		}
	}
	return StructMoveState{m_enMove_No};
}