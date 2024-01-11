#pragma once
class Sphere;
class Stick;
class Player;
class Game;
class GameClear;
class GameCamera:public IGameObject
{
public:
	~GameCamera();
	bool Start();
	void Update();
	
	/// <summary>
	/// �Q�[���I�����̃J�����̑O������ݒ�
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetEndCameraFoward(Vector3 pos)
	{
		m_endCameraFront = pos;
	}

private:
	/// <summary>
	/// �J�����̉�]
	/// </summary>
	void Rotation();

	/// <summary>
	/// �J�����̌����𔽓]������
	/// </summary>
	void InvertCamera();

	/// <summary>
	/// �I��莞�̃J�����̏���
	/// </summary>
	void EndCamera();
private:
	Sphere*				m_sphere=nullptr;
	//Player* m_sphere = nullptr;
	Stick*				m_stick = nullptr;
	Game*				m_game = nullptr;
	GameClear*			m_gameClear = nullptr;


	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_target;								//�����_
	Vector3				m_toCameraPos;							//�����_���王�_�Ɍ������x�N�g��
	Vector3				m_initialCameraPos;
	Vector3				m_axis = Vector3::Zero;
	Vector3				m_endCameraFront = Vector3::Zero;


	float				m_turnTimer = 0.0f;						//�J�����̉�]����
	float				m_degree = 0.0f;
	float				m_endTime = 2.0f;
	bool				m_isTurningCamera = false;				//�J��������]�����Ă���ǂ���
};

