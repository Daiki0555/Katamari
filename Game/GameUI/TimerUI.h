#pragma once
class TimerUI:public IGameObject
{
public:
	~TimerUI();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
public:
	void Rotation();
private:

private:
	SpriteRender m_arrowRender;
	SpriteRender m_clockBackRender;
	SpriteRender m_clockLimitRender;
	Quaternion m_rotation=Quaternion::Identity;
	float m_rot = 0.0f;
	float m_limitTime = 0.0f;
	float m_timer = 0.0f;
};

