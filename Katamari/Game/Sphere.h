#pragma once
class Player;
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
	/// �ړ����x�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()const
	{
		return m_scale;
	}


public:

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();
	

	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();

private:
	ModelRender m_sphereRender;
	Player* m_player;
	Vector3 m_position ={0.0f,50.0f,0.0f};
	Vector3 m_beforePosition = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	const float m_protMoveSpeedMultiply = 5.0f;					//�ŏ��̑��x
	float m_moveSpeedMultiply = 5.0f;							//�ړ����x
	float m_radius=0.0f;


	CharacterController m_charaCon;

	Quaternion m_rotation= Quaternion::Identity;
	CollisionObject m_collisionObject;
	float m_angle = 0.0f;
};

