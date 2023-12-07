#include "stdafx.h"
#include "Player.h"
#include "Sphere.h"

namespace
{
	const float PLAYER_RADIUS = 5.0f;					//半径
	const float PLAYER_HEIGHT = 15.0f;					//高さ

}
Player::~Player()
{

}

bool Player::Start()
{
	m_sphere = FindGO<Sphere>("sphere");

	//アニメーションを読み込む
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);

	//プレイヤーの設定
	m_playerRender.InitToonModel(
		"Assets/modelData/player/unityChan.tkm",
		m_animationClips,
		m_enAnimationClip_Num,
		enModelUpAxisY,
		true,
		false,
		true
	);
	m_playerRender.SetPosition(m_position);
	m_playerRender.SetScale(m_scale);
	m_playerRender.SetRotation(m_rotation);
	m_playerRender.Update();

	//キャラコンの設定
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

	//Rotation();

	ManageState();

	Animation();

	//キャラコンを使用して
	//プレイヤーの座標とモデルの座標を更新する
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
	Vector3 pos = g_camera3D->GetForward();
	pos = pos * m_sphere->GetRadius() * 1.2f;
	m_position= m_sphere->GetPosition() - pos;
}

void Player::Rotation()
{
	m_moveSpeed = m_sphere->GetMoveSpeed();

	//もし少しも動いていないなら
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}
	//atn2を使って角度を求める
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//SetRotationを使用する
	m_rotation.SetRotationY(-angle);
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
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f){
		if (g_pad[0]->IsPress(enButtonA)){
			//ダッシュ中にする
			m_playerState = m_enPlayer_Run;
		}
		else{
			//歩き中にする
			m_playerState = m_enPlayer_walk;
		}
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
		m_playerRender.PlayAnimation(m_enAnimationClip_Run, 0.5f);
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
	m_playerRender.Draw(rc);
}