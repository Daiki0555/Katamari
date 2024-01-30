#include "stdafx.h"
#include "GameCamera.h"
#include "Player/Sphere.h"
#include "Player/Player.h"
#include "Stick.h"
#include "Game.h"
#include "Scene/GameClear.h"
namespace
{
	const Vector3 CAMERA_POS = { 0.0f,0.0f,-70.0f };
	const Vector3 TARGET_UP = { 0.0f, 30.0f, 0.0f };
	const Vector3 CAMERA_POS_ADD = { 0.0f,0.0f,-10.0f };
	const Vector3 TARGET_UP_ADD = { 0.0f, 0.5f, 0.0f };
	const float	CAMERA_ROTSPEED = 0.4f;
	const float CAMERA_BOTH_ROTSPEED = 0.8f;
	const float CAMERA_ROT_DEGREE = 87.0f;
	const float CAMERA_ROT_DEGREE2 = 95.0f;
	const float RADIUS_INCREMENT = 2.0f;												//臒l���グ���
	const int	CAMERA_UP_MAX_FREQUENCY = 20.0f;										//�J�����̑傫�����グ���		
}
GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//���̎擾
	m_sphere = FindGO<Sphere>("sphere");
	m_stick = FindGO<Stick>("stick");
	m_game = FindGO<Game>("game");
	m_gameClear = FindGO<GameClear>("gameClear");
	m_initialCameraPos = CAMERA_POS;
	m_targetUp = TARGET_UP;
	m_thresholdCameraUp = m_sphere->GetRadius() + RADIUS_INCREMENT;
	
	return true;
}

void GameCamera::Update()
{
	GameCameraState();
	if (GameManager::GetInstance()->GetGameSceneState()==GameManager::m_enGameState_DuringGamePlay) {
		if (!m_isTurningCamera) {
			Rotation();
		}
		InvertCamera();

	}

	if (m_gameClear->GetIsEndState()) {
		EndCamera();
	}
	
	g_camera3D->SetPosition(m_toCameraPos);
	g_camera3D->SetTarget(m_target);
	//�X�V
	g_camera3D->Update();
}

void GameCamera::Rotation()
{	
	float stick = 0.0f;
	Vector3 Rstick = m_stick->GetRStickValue();
	Vector3 Lstick = m_stick->GetLStickValue();
	//�x�N�g���̓��ς��v�Z
	float dotProduct = Rstick.Dot(Lstick);
	//���ϕ��p�x���v�Z
	float angleRadians = acosf(dotProduct);
	float angleDegrees = angleRadians * (180.0f / Math::PI);
	//�ړ����Ă���Ƃ���������
	if (fabs(angleDegrees)<= CAMERA_ROT_DEGREE ||
		fabs(angleDegrees)>= CAMERA_ROT_DEGREE2) {
		if (m_stick->GetRStickValue().y >= m_stick->GetLStickValue().y) {
			//�E�X�e�b�N�ƍ��X�e�b�N�̓��͗ʂ̍��������߂�
			stick = m_stick->GetRStickValue().y - m_stick->GetLStickValue().y;
			//Y������̉�]
			m_rotation.SetRotationDeg(Vector3::AxisY, -CAMERA_BOTH_ROTSPEED * stick);
			m_rotation.Apply(m_initialCameraPos);
		}
		else {
			//���X�e�b�N�ƉE�X�e�b�N�̓��͗ʂ̍��������߂�
			stick = m_stick->GetLStickValue().y - m_stick->GetRStickValue().y;
			//Y������̉�]
			m_rotation.SetRotationDeg(Vector3::AxisY, CAMERA_BOTH_ROTSPEED * stick);
			m_rotation.Apply(m_initialCameraPos);
		}
	}

	if (m_stick->GetStickState() == m_enStick_Right){
		stick= m_stick->GetRStickValue().y;
		//Y������̉�]
		m_rotation.SetRotationDeg(Vector3::AxisY, -CAMERA_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);

	}
	else if (m_stick->GetStickState() == m_enStick_Left){
		stick = m_stick->GetLStickValue().y;
		m_rotation.SetRotationDeg(Vector3::AxisY, CAMERA_ROTSPEED * stick);
		m_rotation.Apply(m_initialCameraPos);
	}

	//�����_��Y���W���グ��
	m_target = m_sphere->GetPosition();
	m_target += m_targetUp;
	m_toCameraPos = m_target + m_initialCameraPos;
}

void GameCamera::InvertCamera()
{
	//�����̃X�e�b�N���������܂ꂽ��
	if (g_pad[0]->IsTrigger(enButtonLB3) &&
		g_pad[0]->IsTrigger(enButtonRB3)) {
		m_isTurningCamera = true;

		//Y���ƃJ�����̌��Ă�������̊O�ς����߂�
		Vector3 Y_up = { 0.0f,1.0f,0.0f };
		m_axis = Cross(Y_up, g_camera3D->GetForward());
	}
	if (m_isTurningCamera) {
		m_turnTimer += g_gameTime->GetFrameDeltaTime();
		m_turnTimer = min(m_turnTimer, 1.0f);
		
		//180���܂ł�������]������
		m_degree = 180.0f * g_gameTime->GetFrameDeltaTime();
		m_degree = min(m_degree, 180.0f);

		m_rotation.SetRotationDeg(m_axis, m_degree);
		m_rotation.Apply(m_initialCameraPos);

		//�����_��Y���W���グ��
		m_target = m_sphere->GetPosition();
		m_target += m_targetUp;
		m_toCameraPos = m_target + m_initialCameraPos;
		
		if (m_turnTimer >= 1.0f) {
			m_isTurningCamera = false;
			m_turnTimer = 0.0f;
		}
	}
}


void GameCamera::EndCamera()
{
	GameManager::GetInstance()->SetGameSceneState(GameManager::m_enGameState_GameEnd);
	m_toCameraPos -= m_endCameraFront;
	m_toCameraPos.y += 0.3f;
	m_endTime -= g_gameTime->GetFrameDeltaTime();
}

void GameCamera::GameCameraState()
{
	//臒l���傫���Ȃ�����
	if (m_sphere->GetRadius() >= m_thresholdCameraUp&&
		!m_isTargetLevelUp) {
		m_isTargetLevelUp = true;
	}
	//�^�[�Q�g�����Z�ł���悤�ɂ���
	if (m_cameraUpFrequency<= CAMERA_UP_MAX_FREQUENCY&&
		m_isTargetLevelUp) {
		//���W��ύX����
		Vector3 front=g_camera3D->GetForward();
		m_initialCameraPos -= front;
		m_targetUp += TARGET_UP_ADD;
		m_cameraUpFrequency++;
	}
	if(m_cameraUpFrequency> CAMERA_UP_MAX_FREQUENCY) {
		//臒l���傫���Ȃ邲�Ƃɉ��Z������
		m_thresholdCameraUp += RADIUS_INCREMENT;
		m_cameraUpFrequency = 0;
		m_isTargetLevelUp = false;
	}
}