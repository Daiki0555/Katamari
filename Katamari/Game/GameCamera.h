#pragma once
class Player;
class GameCamera:public IGameObject
{
public:
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();
public:
private:
private:
	Player* m_player;
	Vector3				m_target;						//注視点
	Vector3				m_toCameraPos;					//注視点から視点に向かうベクトル
};

