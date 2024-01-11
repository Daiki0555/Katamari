#pragma once
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
	Game* m_game = nullptr;
	SpriteRender m_arrowRender;
	SpriteRender m_clockBackRender;
	SpriteRender m_clockLimitRender;
	FontRender m_timeRender;
	Quaternion m_rotation=Quaternion::Identity;
	float m_rot = 0.0f;
	float m_limitTime = 0.0f;
	float m_timer = 0.0f;
	float m_degree = 0.0f;
	int m_intTime = 0;
	bool m_isProcessing = false;
};

