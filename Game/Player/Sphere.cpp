#include "stdafx.h"
#include "Sphere.h"
#include "Object/Object.h"
#include "Game.h"
#include "Stick.h"
namespace 
{
	const float		SPEED_DOWN = 0.9f;									//���x������
	const float		STANDARDSIZE = 40.0f;								//��̉�̒��a
	const float		ALWAYS_SPEED = 5.0f;								//�������̏�Z��
	const float		ALWAYS_RUNSPEED = 50.0f;							//���莞�̏�Z��
	const float		ENTER_STICK = 0.001f;
	const float		INITIAL_RADIUS = 11.0f;							//�������a
	const float		GRAVITY = 10.0f;
	const float		DASH_AVAILABLE_TIME = 0.5f;							//�_�b�V���\����
	const int		DASH_AVAILABLE_COUNT=5;								//�_�b�V���\�J�E���g
	const float		MODEL_UP = 10.0f;
	const float		ROT_SPEED = 3.0f;
	const float		COLIION_YDOWNPOS = 5.0f;
	const float     BRAKE_SPEED_DOWN = 0.3f;							//�u���[�L���̃X�s�[�h������
}
Sphere::~Sphere()
{

}

bool Sphere::Start()
{
	//���ꂼ��̃N���X�̏��������Ă���
	m_object = FindGO<Object>("object");
	m_game = FindGO<Game>("game");
	m_stick = FindGO<Stick>("stick");

	//�X�t�B�A���f���̏��̐ݒ�
	m_sphereRender.InitToonModel("Assets/modelData/sphere/sphere.tkm",
		0,
		0, 
		enModelUpAxisY, 
		true, 
		false, 
		true
	);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetScale(m_scale);
	m_sphereRender.Update();

	//�������a�̐ݒ�
	m_radius = INITIAL_RADIUS;

	//�L�����R���̐ݒ�
	m_charaCon.Init(
		m_radius,
		m_position
	);

	//���̑̐ς����߂�
	m_volume = Math::PI * pow(m_radius, 3.0f) * 4 / 3;
	return true;
}

void Sphere::Update()
{
	DashCount();

	Move();

	Rotation();
	
	//�L�����R�����g�p����
	//�v���C���[�̍��W�ƃ��f���̍��W���X�V����
	m_charaCon.SetPosition(m_position);
	m_position = m_charaCon.Execute(
		m_moveSpeed, 
		g_gameTime->GetFrameDeltaTime()
	);

	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetPosition(Vector3{ m_position.x,m_position.y + MODEL_UP,m_position.z });
	m_sphereRender.Update();

	//�O�̍��W���L������
	m_beforePosition = m_position;


	//��ԐV������̃��[���h�s�񂪕K�v�Ȃ̂�
	//�����Ŋ������܂ꂽ�I�u�W�F�N�g�̃��[���h�s����v�Z����
	for (int i = 0; i < m_game->GetObjectList().size(); i++) {
		if (m_game->GetObjectList()[i]->GetObjectState() ==
			Object::m_enObject_Involution) {
			m_game->GetObjectList()[i]->CalcMatrix();
		}
		
	}
}

void Sphere::Move()
{
	if (m_dashCount >= DASH_AVAILABLE_COUNT) {
		Dash();
	}
	else {	
		//�ړ����x
		m_moveSpeedMultiply= ALWAYS_SPEED*(INITIAL_RADIUS/m_radius);

		Vector3 Rstick = m_stick->GetRStickValue();
		Vector3 Lstick = m_stick->GetLStickValue();
		Vector3 Stick = Rstick + Lstick;
		//�J�����̑O�����ƁA�E�����̎擾
		Vector3 cameraFoward = g_camera3D->GetForward();
		Vector3 cameraRight = g_camera3D->GetRight();

		//XZ���ʂł̑O�������ƉE�������擾
		cameraFoward.y = 0.0f;
		cameraFoward.Normalize();
		cameraRight.y = 0.0f;
		cameraRight.Normalize();

		//���X�e�b�N�ƕ������x����Z������
		if (m_stick->GetStickState() == m_enStick_Both) {
			m_moveSpeed += cameraFoward * Stick.y * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed += cameraRight * Stick.x * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime();
		}
	}
	
	//���ʂɑ��x������������
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;
	
	Gravity();

	Brake();

	m_position += m_moveSpeed;
}

void Sphere::Dash()
{
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	if (m_isDash) {
		m_moveSpeed += cameraFoward * ALWAYS_RUNSPEED * g_gameTime->GetFrameDeltaTime();

	}
	//�_�b�V�����ɂ�����x���点��
	else {
		for (int i = 0; i < 5; i++) {
			m_moveSpeed += cameraFoward * ALWAYS_RUNSPEED * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= SPEED_DOWN;
		}
		m_isDash = true;
	}
}

void Sphere::Brake()
{

	//�u���[�L���Ă��Ȃ����A�����̃X�e�b�N���|���ꂽ�Ƃ��Ɉړ����x��������x����Ȃ�
	if (!m_isBrake &&
		m_stick->GetStickState() == m_enStick_Both) {
		
	}
	else {
		m_isBrake = false;
	}
}

void Sphere::DashCount()
{
	if (m_isInverseStick) {
		//�X�e�b�N�̌��������]�����Ȃ�
		if (m_stick->GetStickState() == m_enStick_RightYInverseLeftY) {
			m_dashCount++;
			m_dashTimer = DASH_AVAILABLE_TIME;
		}
	}
	else {
		//�X�e�b�N�̌��������]�����Ȃ�
		if (m_stick->GetStickState() == m_enStick_LeftYInverseRightY) {
			m_dashCount++;
			m_dashTimer = DASH_AVAILABLE_TIME;
		}
	}

	if (m_stick->GetStickState() == m_enStick_LeftYInverseRightY) {
		//���̏��������߂ĂȂ�
		if (m_dashCount == 0) {
			//true�ł����Z�ł���悤�ɂ���
			m_dashCount++;
			m_dashTimer = DASH_AVAILABLE_TIME;
		}
		m_isInverseStick = true;
	}
	else if (m_stick->GetStickState() == m_enStick_RightYInverseLeftY) {
		//���̏��������߂ĂȂ�
		if (m_dashCount == 0) {
			//false�ł����Z�ł���悤�ɂ���
			m_dashCount++;
			m_dashTimer = DASH_AVAILABLE_TIME;
		}
		m_isInverseStick = false;
	}
	//���͂��Ԃɍ����Ă��Ȃ��Ȃ�
	if (m_dashTimer <= 0.0f) {
		m_dashCount = 0;
		m_isDash = false;
	}

	m_dashTimer -= g_gameTime->GetFrameDeltaTime();
}

void Sphere::Gravity()
{

	//�����n�ʂɒ����Ă��Ȃ��Ȃ�
	if (m_charaCon.IsOnGround() == false) {
		//��������悤�ɂ���
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}
	else {
		//�������Ȃ��悤�ɂ���
		m_moveSpeed.y = 0.0f;
	}
}

void Sphere::Rotation()
{
	const float rotationSpeed = 3.0f*(m_moveSpeedMultiply/ALWAYS_SPEED);
	//�O�̍��W�Ƃ̍��������߂�
	Vector3 move = m_position - m_beforePosition;
	move.y = 0.0f;

	//�����Ă��Ȃ��Ȃ�
	if (fabsf(move.x) < 0.001f
		&& fabsf(move.z) < 0.001f) {
		return;
	}
	
	//�i�s�����̃x�N�g���̒��������߂�
	float length = move.Length();

	//�m�[�}���C�Y���Ĉړ������̕����x�N�g�������߂�
	move.Normalize();

	//��L�̊O�ς����߂�
	Vector3 vertical = Cross(Vector3::AxisY, move);
	//�O�σx�N�g�������ɉ�]������N�H�[�^�j�I�������߂�
	Quaternion rot;
	rot.SetRotationDeg(vertical, length * rotationSpeed);

	//���߂��N�H�[�^�j�I������Z����
	m_rotation.Multiply(m_rotation,rot);
}

void Sphere::Render(RenderContext& rc)
{
	m_sphereRender.Draw(rc);
}

