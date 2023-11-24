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

	/// <summary>
	/// �ړ����̉�]
	/// </summary>
	void MoveRotation();

	/// <summary>
	/// �J�����̌����𔽓]������
	/// </summary>
	void InvertCamera();
private:
	Sphere*				m_sphere=nullptr;
	Stick*				m_stick = nullptr;

	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_target;								//�����_
	Vector3				m_toCameraPos;							//�����_���王�_�Ɍ������x�N�g��
	Vector3				m_initialCameraPos;
	Vector3				m_axis = Vector3::Zero;
	float				m_turnTimer = 0.0f;						//�J�����̉�]����
	float				m_degree = 0.0f;
	bool				m_isTurningCamera = false;				//�J��������]�����Ă���ǂ���
};

