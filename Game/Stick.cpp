#include "stdafx.h"
#include "Stick.h"
namespace {
	const float		ENTER_STICK = 0.001f;				//�X�e�b�N��|�������̍ŏ��l
	const float		FULL_KNOCKDOWN_STICK = 0.9f;		//�ő�܂œ|�������̒l
}
Stick::~Stick()
{

}

void Stick::Update()
{
	StickState();
}

void Stick::StickState()
{
	//���X�e�b�N�̏����擾
	m_Lstick.x = g_pad[0]->GetLStickXF();
	m_Lstick.y = g_pad[0]->GetLStickYF();
	//�E�X�e�b�N�̏����擾
	m_Rstick.x = g_pad[0]->GetRStickXF();
	m_Rstick.y = g_pad[0]->GetRStickYF();

	if (m_Lstick.y > FULL_KNOCKDOWN_STICK &&
		m_Rstick.y < -FULL_KNOCKDOWN_STICK) {
		m_stickState = m_enStick_FullLeftYInverseRightY;
	}
	else if (m_Lstick.y < -FULL_KNOCKDOWN_STICK &&
		m_Rstick.y > FULL_KNOCKDOWN_STICK) {
		m_stickState = m_enStick_FullRightYInverseLeftY;
	}



	//�E�X�e�B�b�N�̓��͂����肩��
	else if (m_Rstick.LengthSq() >= ENTER_STICK) {
		//���X�e�b�N�̓��͂������Ȃ�
		if (m_Lstick.LengthSq() <= ENTER_STICK) {
			m_stickState = m_enStick_Right;
		}
		else {
			m_stickState = m_enStick_Both;
		}
	}
	//���X�e�B�b�N�̓��͂����肩��
	else if (m_Lstick.LengthSq() >= ENTER_STICK) {
		//�E�X�e�b�N�̓��͂������Ȃ�
		if (m_Rstick.LengthSq() <= ENTER_STICK) {
			m_stickState = m_enStick_Left;
		}
		else {
			m_stickState = m_enStick_Both;
		}
	}
	else {
		m_stickState = m_enStick_No;
	}
}