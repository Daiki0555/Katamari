#include "stdafx.h"
#include "Player.h"
#include "Sphere.h"
#include "Stick.h"
namespace
{
	const float PLAYER_RADIUS = 5.0f;					//���a
	const float PLAYER_HEIGHT = 10.0f;					//����

}
Player::~Player()
{

}

bool Player::Start()
{
	m_sphere = FindGO<Sphere>("sphere");
	m_stick = FindGO<Stick>("stick");

	//�A�j���[�V������ǂݍ���
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);

	//�v���C���[�̐ݒ�
	m_playerRender.InitToonModel(
		"Assets/modelData/player/unityChan.tkm",
		m_animationClips,
		m_enAnimationClip_Num,
		enModelUpAxisY,
		true,
		false,
		false
	);

	m_playerRender.SetPosition(m_position);
	m_playerRender.SetScale(m_scale);
	m_playerRender.SetRotation(m_rotation);
	m_playerRender.Update();

	//�L�����R���̐ݒ�
	m_characon.Init(
		PLAYER_RADIUS,
		PLAYER_HEIGHT,
		m_position
	);
	return true;
}

void Player::Update()
{


	Move();

	Rotation();

	ManageState();

	Animation();

	//�L�����R�����g�p����
	//�v���C���[�̍��W�ƃ��f���̍��W���X�V����
	m_characon.SetPosition(m_position);
	m_position = m_characon.Execute(
		m_moveSpeed,
		g_gameTime->GetFrameDeltaTime()
	);

	m_playerRender.SetRotation(m_rotation);
	m_playerRender.SetPosition(m_position);
	m_playerRender.Update();
}


void Player::Move()
{
	//�J�����̑O�x�N�g��
	Vector3 pos = g_camera3D->GetForward();
	//�J�����̑O�x�N�g���Ɖ�̔��a�̏�Z����
	pos = pos * m_sphere->GetRadius() * 1.2f;
	//�������̂���̍��W�������
	m_position= m_sphere->GetPosition() - pos;
}

void Player::Rotation()
{
	m_moveSpeed = m_sphere->GetMoveSpeed();
	//�J�����̑O�x�N�g��
	Vector3 pos = g_camera3D->GetForward();
	m_rotation.SetRotation(Vector3::AxisY, atan2f(pos.x, pos.z));
}

void Player::ManageState()
{
	switch (m_playerState)
	{
	case Player::m_enPlayer_Idle:
		ProcessIdleStateTransition();
		break;
	case Player::m_enPlayer_walk:
		ProcessWalkStateTransition();
		break;
	case Player::m_enPlayer_Run:
		ProcessRunStateTransition();
		break;
	case Player::m_enPlayer_Back:
		break;
	case Player::m_enPlayer_Damage:
		break;
	case Player::m_enPlayer_Invincible:
		break;
	case Player::m_enPlayer_InvincibleEnd:
		break;
	case Player::m_enPlayer_GameEnd:
		break;
	default:
		break;
	}
}

void Player::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessWalkStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	ProcessCommonStateTransition();
}

void Player::ProcessCommonStateTransition()
{	
	if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY ||
		m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY||
		m_sphere->GetDashCount()>=1) {
		m_playerState = m_enPlayer_Run;
	}

	else if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f) {
		//�������ɂ���
		m_playerState = m_enPlayer_walk;
	}
	else {
		m_playerState = m_enPlayer_Idle;
	}
}

void Player::Animation()
{
	switch (m_playerState)
	{
	case Player::m_enPlayer_Idle:
		m_playerRender.PlayAnimation(m_enAnimationClip_Idle, 0.5f);
		break;
	case Player::m_enPlayer_walk:
		m_playerRender.PlayAnimation(m_enAnimationClip_Walk, 0.5f);
		break;
	case Player::m_enPlayer_Run:
		m_playerRender.PlayAnimation(m_enAnimationClip_Run, 1.0f);
		break;
	case Player::m_enPlayer_Back:
		break;
	case Player::m_enPlayer_Damage:
		break;
	case Player::m_enPlayer_Invincible:
		break;
	case Player::m_enPlayer_InvincibleEnd:
		break;
	case Player::m_enPlayer_GameEnd:
		break;
	default:
		break;
	}
}

void Player::Render(RenderContext& rc)
{
	if (!m_isDraw){
		return;
	}
	m_playerRender.Draw(rc);
}