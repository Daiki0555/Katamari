#include "stdafx.h"
#include "Player.h"
namespace
{
	const float		SPEED_DOWN = 0.8;									//速度減速率
	const float		PLAYER_WALKSPEED = 30.0f;							//歩き時の乗算量
	const float		PLAYER_RUNSPEED = 60.0f;							//走り時の乗算量
}
Player::~Player()
{

}

bool Player::Start()
{
	//アニメーションを読み込む
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);

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

	return true;
}

void Player::Update()
{

	Move();

	Rotation();

	ManageState();

	Animation();

	//m_playerRender.SetRotation(m_rotation);
	m_playerRender.SetPosition(m_position);
	m_playerRender.Update();
}

void Player::Move()
{
	Vector3 lstic;
	lstic.x = 0.0f;
	lstic.z = 0.0f;
	//速度を初期化
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;
	//左ステックの情報を取得
	lstic.x = g_pad[0]->GetLStickXF();
	lstic.y = g_pad[0]->GetLStickYF();
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	//XZ平面での前方方向と右方向を取得
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//もしAボタンが押されているなら
	if (g_pad[0]->IsPress(enButtonA) ){
		//左ステックと歩く速度を乗算させる
		m_moveSpeed += cameraFoward * lstic.y * PLAYER_RUNSPEED * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed += cameraRight * lstic.x * PLAYER_RUNSPEED * g_gameTime->GetFrameDeltaTime();
	}
	else {
		//左ステックと歩く速度を乗算させる
		m_moveSpeed += cameraFoward * lstic.y * PLAYER_WALKSPEED * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed += cameraRight * lstic.x * PLAYER_WALKSPEED * g_gameTime->GetFrameDeltaTime();
	}
	m_position += m_moveSpeed;
}

void Player::Rotation()
{
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
	//m_playerRender.Draw(rc);
}