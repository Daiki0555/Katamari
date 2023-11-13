#pragma once
class Sphere;

class Player :public IGameObject
{
public:
	enum EnPlayerState {
		m_enPlayer_Idle,			//�ҋ@��
		m_enPlayer_walk,			//����
		m_enPlayer_Run,				//����
		m_enPlayer_Back,			//���
		m_enPlayer_Damage,			//�_���[�W
		m_enPlayer_Invincible,		//���G
		m_enPlayer_InvincibleEnd,	//���G�I��
		m_enPlayer_GameEnd,			//�Q�[���I��
	};



	~Player();
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
	/// �v���C���[�̉�]�̐ݒ�
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �v���C���[�̉�]�̎擾
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
	/// �v���C���[�̏�Ԃ�ݒ�
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetPlayerState(EnPlayerState state)
	{
		m_playerState = state;
	}

	/// <summary>
	/// �v���C���[�̏�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	const EnPlayerState GetPlayerState()const
	{
		return m_playerState;
	}

public:

private:
	/// <summary>
	/// �v���C���[�̈ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �v���C���[�̉�]
	/// </summary>
	void Rotation();

	
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void Animation();

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();

	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();

	/// <summary>
	/// �ړ��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessWalkStateTransition();

	/// <summary>
	/// �_�b�V���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();

	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();

private:
	Sphere* m_sphere=nullptr;


	ModelRender m_playerRender;


	//�A�j���[�V����
	enum EnAnimationClip {
		m_enAnimationClip_Idle,				//�ҋ@�A�j���[�V����
		m_enAnimationClip_Walk,				//�����A�j���[�V����
		m_enAnimationClip_Run,				//����A�j���[�V����
		m_enAnimationClip_Num,				//�A�j���[�V�����̐�
	};
	EnPlayerState m_playerState = m_enPlayer_Idle;				//�ҋ@���
	AnimationClip m_animationClips[m_enAnimationClip_Num];

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;


	Vector3 m_scale = Vector3::One;

	Quaternion m_rotation = Quaternion::Identity;
	
	CharacterController* m_characon;							//�L�����R��

	float m_radius = 0.0f;										//���̂̔��a

	
};

