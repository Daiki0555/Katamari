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
	/// タイマーの取得
	/// </summary>
	/// <returns></returns>
	const float GetTimer() const
	{
		return m_timer;
	}

private:
	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();
private:
	Game*			m_game = nullptr;
	SpriteRender	m_arrowRender;								//矢印画像
	SpriteRender	m_clockBackRender;							//背景の画像
	SpriteRender	m_clockLimitRender;							//制限時間用のスプライト
	FontRender		m_timeRender;								//時間のフォント
	Quaternion		m_rotation=Quaternion::Identity;
	float			m_rot = 0.0f;
	float			m_limitTime = 0.0f;
	float			m_timer = 0.0f;
	float			m_degree = 0.0f;
	float			m_startTime = 0.0f;							//開始時間(分)						
	int			    m_intTime = 0;
	bool			m_isProcessing = false;
};

