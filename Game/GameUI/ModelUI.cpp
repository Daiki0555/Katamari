#include "stdafx.h"
#include "ModelUI.h"
#include "Player/Player.h"
namespace
{
	//���f�����W
	const Vector3	MODEL_POSITION = { 11190.0f ,-90.0f,0.0f };
	//�n�����f���̍��W
	const Vector3	ERATH_POSITION = { 11280.0f,-150.0f,30.0f };
	const Vector3	ERATH_SCALE = { 2.0f,2.0f,2.0f };
}
ModelUI::~ModelUI()
{

}

bool ModelUI::Start()
{
	//�v���C���[�̏��
	m_player = FindGO<Player>("player");

	//�A�j���[�V������ǂݍ���
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Walk].Load("Assets/animData/player/walk.tka");
	m_animationClips[m_enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[m_enAnimationClip_Run].Load("Assets/animData/player/run.tka");
	m_animationClips[m_enAnimationClip_Run].SetLoopFlag(true);

	//�v���C���[�̐ݒ�
	m_modelUI.InitDeferredRendering(
		"Assets/modelData/player/unityChan.tkm",
		m_animationClips,
		m_enAnimationClip_Num,
		enModelUpAxisY,
		false,
		false,
		true
	);
	m_modelUI.SetPosition(MODEL_POSITION);
	m_modelUI.SetScale(Vector3{0.5f,0.5f,0.5f});
	m_rotation.SetRotationY(180.0f);
	m_modelUI.SetRotation(m_rotation);
	m_modelUI.Update();

	//�n�����f���̓ǂݍ���
	m_earthRender.InitDeferredRendering(
		"Assets/modelData/UI/earth.tkm",
		0,
		0,
		enModelUpAxisZ,
		false,
		false,
		true
	);
	m_earthRender.SetPosition(ERATH_POSITION);
	m_earthRender.SetScale(ERATH_SCALE);
	m_earthRender.Update();


	return true;
}

void ModelUI::Update()
{
	Animation();
	m_modelUI.Update();
}

void ModelUI::Animation()
{
	switch (m_player->GetPlayerState())
	{
	case Player::m_enPlayer_Idle:
		m_modelUI.PlayAnimation(m_enAnimationClip_Idle, 0.5f);
		break;
	case Player::m_enPlayer_walk:
		m_modelUI.PlayAnimation(m_enAnimationClip_Walk, 0.5f);
		break;
	case Player::m_enPlayer_Run:
		m_modelUI.PlayAnimation(m_enAnimationClip_Run, 0.5f);
		break;
		break;
	default:
		break;
	}
}

void ModelUI::Render(RenderContext& rc)
{
	m_earthRender.Draw(rc);
	m_modelUI.Draw(rc);
}