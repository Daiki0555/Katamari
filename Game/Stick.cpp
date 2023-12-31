#include "stdafx.h"
#include "Stick.h"
namespace {
	const float		ENTER_STICK = 0.001f;				//ステックを倒した時の最小値
	const float		FULL_KNOCKDOWN_STICK = 0.9f;		//最大まで倒した時の値
	const float		STICK_ANGLE = 95.0f;
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
	//左ステックの情報を取得
	m_Lstick.x = g_pad[0]->GetLStickXF();
	m_Lstick.y = g_pad[0]->GetLStickYF();
	//右ステックの情報を取得
	m_Rstick.x = g_pad[0]->GetRStickXF();
	m_Rstick.y = g_pad[0]->GetRStickYF();

	//ベクトルの内積を計算
	float dotProduct = m_Rstick.Dot(m_Lstick);
	//内積方角度を計算
	float angleRadians = acosf(dotProduct);
	float angleDegrees = angleRadians * (180.0f / Math::PI);
	if (fabs(angleDegrees) > 95.0f) {
		if (m_Lstick.y > m_Rstick.y) {
			m_stickState = m_enStick_FullLeftYInverseRightY;
		}
		else if (m_Rstick.y > m_Lstick.y) {
			m_stickState = m_enStick_FullRightYInverseLeftY;
		}

	}
	




	//右スティックの入力がありかつ
	else if (m_Rstick.LengthSq() >= ENTER_STICK) {
		//左ステックの入力が無いなら
		if (m_Lstick.LengthSq() <= ENTER_STICK) {
			m_stickState = m_enStick_Right;
		}
		else {
			m_stickState = m_enStick_Both;
		}
	}
	//左スティックの入力がありかつ
	else if (m_Lstick.LengthSq() >= ENTER_STICK) {
		//右ステックの入力が無いなら
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