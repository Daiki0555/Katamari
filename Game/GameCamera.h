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

	/// <summary>
	/// 移動時の回転
	/// </summary>
	void MoveRotation();

	/// <summary>
	/// カメラの向きを反転させる
	/// </summary>
	void InvertCamera();
private:
	Sphere*				m_sphere=nullptr;
	Stick*				m_stick = nullptr;

	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_target;								//注視点
	Vector3				m_toCameraPos;							//注視点から視点に向かうベクトル
	Vector3				m_initialCameraPos;
	Vector3				m_axis = Vector3::Zero;
	float				m_turnTimer = 0.0f;						//カメラの回転時間
	float				m_degree = 0.0f;
	bool				m_isTurningCamera = false;				//カメラを回転させているどうか
};

