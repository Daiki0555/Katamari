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
	Vector3				m_target;						//�����_
	Vector3				m_toCameraPos;					//�����_���王�_�Ɍ������x�N�g��
};

