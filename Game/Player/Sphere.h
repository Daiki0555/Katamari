#pragma once
class Object;
class Game;
class Stick;
class Title;
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
	/// �X�V�O�̍��W
	/// </summary>
	/// <returns></returns>
	const Vector3& GetBeforePosition() const
	{
		return m_beforePosition;
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
	/// �_�b�V���J�E���g�̎擾
	/// </summary>
	/// <returns></returns>
	const int GetDashCount() const
	{
		return m_dashCount;
	}

	/// <summary>
	/// �̐ς����Z����
	/// </summary>
	/// <param name="volume"></param>
	const void AddVolume(const float volume)
	{
		m_volume += volume;
		m_radius = pow(3.0f * m_volume / (4.0f * Math::PI), 1.0f / 3.0f);
		m_charaCon.SetRadius(m_radius);
	}

	/// <summary>
	/// �`�悷�邩�ǂ���
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void IsDraw(const bool state)
	{
		m_isDraw = state;
	}

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �^�C�g�����̓���
	/// </summary>
	void TitleMove();

	/// <summary>
	/// ���Γ��m�̃X�e�b�N���̈ړ�
	/// </summary>
	void InverseStickMove();


	/// <summary>
	/// �ǂ���̌����Ɉړ����Ă��邩�̔���
	/// </summary>
	void IsMoving();
	
	/// <summary>
	/// �_�b�V��
	/// </summary>
	void Dash();

	/// <summary>
	/// �u���[�L
	/// </summary>
	void Brake();

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

	/// <summary>
	/// �_�b�V����]
	/// </summary>
	void DashRotation();

private:
	Object* m_object = nullptr;
	Game* m_game = nullptr;
	Stick* m_stick = nullptr;
	Title* m_title = nullptr;

	ModelRender m_sphereRender;
	FontRender m_fontRender;
	
	Vector3 m_position ={0.0f,50.0f,0.0f};

	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_beforePosition = Vector3::Zero;					//�ړ��O�̍��W

	Vector3 m_scale = Vector3::One;
	Vector3 m_inputStick = Vector3::Zero;
	
	Vector3 m_vertical = Vector3::Zero;
	
	CharacterSphereController m_charaCon;


	Quaternion m_rotation= Quaternion::Identity;
	
	
	float m_moveSpeedMultiply = 5.0f;							//�ړ����x

	float m_radius = 0.0f;										//���̂̔��a

	float m_angle = 0.0f;

	float m_volume = 0.0f;										//�I�u�W�F�N�g�̑̐�

	float m_dashTimer=0.0f;										//�_�b�V���\�Ȕ���̎���

	float m_moveDownTime = 0.0f;

	float m_beforeSpeedLength = 0.0f;
	
	int m_dashCount = 0;										//�_�b�V���J�E���g

	bool m_isInverseStick = true;								//�X�e�b�N���t�ɓ|�������ɔ��]����

	bool m_isDash = false;										//�_�b�V�����Ă��邩�ǂ���

	bool m_isBrake = false;										//�u���[�L���Ă��Ȃ��Ȃ�

	bool m_isMovingBackward = false;							// ��ޒ����ǂ����������t���O (false�Ȃ�O�i)
	bool m_wasMovingBackward = false;							// ���O�܂Ō�ޒ����������ǂ����������t���O (false�Ȃ�O�i)

	bool m_isDraw = true;

	bool m_isTitleMove = false;
};

