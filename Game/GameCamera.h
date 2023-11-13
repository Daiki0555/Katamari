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
	/// �J�����̉�]
	/// </summary>
	void Rotation();
private:
	Sphere*				m_sphere=nullptr;
	Stick*				m_stick = nullptr;

	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_target;								//�����_
	Vector3				m_toCameraPos;							//�����_���王�_�Ɍ������x�N�g��
	Vector3				m_initialCameraPos;
};

