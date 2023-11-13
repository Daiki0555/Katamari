#include "stdafx.h"
#include "WordManager.h"
const wchar_t* WordManager::m_rangeLimit = L"H";

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
	float move = 0.0, rangeLimit = 0.0f;
	const wchar_t* name = objName;
	EnMoveState movestate = m_enMove_No;
	for (int i = 0; i < m_move.size(); i++) {
		//������𒲂ׂ�
		const wchar_t* wcstr = wcsstr(name, m_move[i]);
		if (wcstr != NULL) {
			//������̒��̐��������߂�
			int wcsf = (int)(wcstr - name+1);
			name += wcsf;
			wchar_t* word;
			//�ړ����x�̎擾
			move = wcstof(name, &word);
			//�X�e�[�g��ǂݍ���
			movestate = m_moveState[i];
			const wchar_t* rwcstr = wcsstr(name,m_rangeLimit);
			if (rwcstr == NULL)
			{
				return StructMoveState{ movestate, move,0.0f };
			}
			//������̒��̐��������߂�
			int rwcsf = (int)(rwcstr - name + 1);
			name += rwcsf;
			//�ړ��͈͂�ǂݍ���
			rangeLimit = wcstof(name, &word);
			return StructMoveState{ movestate, move,rangeLimit};
		}
	}
	return StructMoveState{ m_enMove_No,0,0 };
}