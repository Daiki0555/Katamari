#pragma once
#include "GameManager.h"
class Game;
class TimerUI:public IGameObject
{
public:
	~TimerUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �^�C�}�[�̎擾
	/// </summary>
	/// <returns></returns>
	const float GetTimer() const
	{
		return m_timer;
	}

private:
	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();
private:
	Game*			m_game = nullptr;
	SpriteRender	m_arrowRender;								//���摜
	SpriteRender	m_clockBackRender;							//�w�i�̉摜
	SpriteRender	m_clockLimitRender;							//�������ԗp�̃X�v���C�g
	FontRender		m_timeRender;								//���Ԃ̃t�H���g
	Quaternion		m_rotation=Quaternion::Identity;
	float			m_rot = 0.0f;
	float			m_limitTime = 0.0f;
	float			m_timer = 0.0f;
	float			m_degree = 0.0f;
	float			m_startTime = 0.0f;							//�J�n����(��)						
	int			    m_intTime = 0;
	bool			m_isProcessing = false;
};

