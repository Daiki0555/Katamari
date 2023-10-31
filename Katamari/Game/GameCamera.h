#pragma once
class Sphere;
class Stick;
class GameCamera:public IGameObject
{
public:
	~GameCamera();
	bool Start();
	void Update();
public:
private:
	/// <summary>
	/// カメラの回転
	/// </summary>
	void Rotation();
private:
	Sphere*				m_sphere=nullptr;
	Stick*				m_stick = nullptr;

	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_target;								//注視点
	Vector3				m_toCameraPos;							//注視点から視点に向かうベクトル
	Vector3				m_initialCameraPos;
};

