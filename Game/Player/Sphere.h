#pragma once
class Object;
class Game;
class Stick;
class Sphere: public IGameObject
{
public:
	/// <summary>
	/// �{�[���̓����蔻��̑傫���̏��
	/// </summary>
	enum enSphereScaleState {
		m_enSphere_Level1,
		m_enSphere_Level2,
		m_enSphere_Level3,
		m_enSphere_Level4,
		m_enSphere_Level5,
		m_enSphere_LevelMax
	};

	/// <summary>
	/// �{�[���̏��
	/// </summary>
	enum enSphereState {
		m_enSphere_ObjectGet,		//�I�u�W�F�N�g�擾��
		m_enSphere_Dash,			//�_�b�V����
		m_enSphere_Damege,			//�_���[�W
		m_enSphere_Invincible,		//���G
		m_enSphere_InvincibleEnd,	//���G�I��
		m_enPlayer_GameEnd,			//�Q�[���I��
	};

	~Sphere();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	const void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �傫���̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()const
	{
		return m_scale;
	}

	/// <summary>
	/// �ړ����x�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetMoveSpeed()
	{
		return m_moveSpeed;
	}


	/// <summary>
	/// ���f�����̎擾
	/// </summary>
	/// <returns></returns>
	ModelRender& GetSphereModel()
	{
		return m_sphereRender;
	}

	/// <summary>
	/// ��̔��a
	/// </summary>
	/// <returns></returns>
	const float GetRadius() const
	{
		return m_radius;
	}

	/// <summary>
	/// ��̔��a�̐ݒ�
	/// </summary>
	const void SetRadius(const float radius)
	{
		m_radius = radius;
	}

	/// <summary>
	/// �̐ς����Z����
	/// </summary>
	/// <param name="volume"></param>
	const void AddVolume(const float volume)
	{
		m_volume += volume;
	}
public:

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �_�b�V��
	/// </summary>
	void Dash();

	/// <summary>
	/// �_�b�V���J�E���g
	/// </summary>
	void DashCount();

	/// <summary>
	/// �d��
	/// </summary>
	void Gravity();
	

	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();




private:
	Object* m_object = nullptr;
	Game* m_game = nullptr;
	Stick* m_stick = nullptr;

	ModelRender m_sphereRender;
	
	Vector3 m_position ={0.0f,50.0f,0.0f};
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_beforePosition = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	
	
	CharacterController m_charaCon;

	Quaternion m_rotation= Quaternion::Identity;
	
	CollisionObject m_collisionObject;
	
	const float m_protMoveSpeedMultiply = 5.0f;					//�ŏ��̑��x
	float m_moveSpeedMultiply = 5.0f;							//�ړ����x

	float m_radius = 0.0f;										//���̂̔��a

	float m_angle = 0.0f;

	float m_volume = 0.0f;										//�I�u�W�F�N�g�̑̐�

	float m_dashTimer=1.0f;										//�_�b�V���\�Ȕ���̎���

	int m_dashCount = 0;										//�_�b�V���J�E���g

	bool m_isInverseStick = true;								//�X�e�b�N���t�ɓ|�������ɔ��]����

	bool m_isDash = false;										//�_�b�V�����Ă��邩�ǂ���
};

