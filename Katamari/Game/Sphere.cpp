#include "stdafx.h"
#include "Sphere.h"
#include "Object.h"
#include "Game.h"
#include "Stick.h"
namespace 
{
	const float		SPEED_DOWN = 0.8;									//���x������
	const float		STANDARDSIZE = 40.0f;								//��̉�̒��a
	const float		ALWAYS_SPEED = 10.0f;								//�������̏�Z��
	const float		PLAYER_RUNSPEED = 10.0f;							//���莞�̏�Z��
	const float		ENTER_STICK = 0.001f;
	const float		INITIAL_RADIUS = 10.0f;								//�������a
	const float		GRAVITY = 10.0f;
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

	m_sphereRender.InitToonModel("Assets/modelData/sphere/sphere.tkm", 0, 0, enModelUpAxisY, true, false, true);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetScale(m_scale);
	m_sphereRender.Update();

	m_radius = INITIAL_RADIUS;

	m_charaCon.Init(
		100.0f,
		100.0f,
		m_position
	);

	m_collisionObject.CreateSphere(
		m_position,
		m_rotation,
		m_radius
	);

	return true;
}

void Sphere::Update()
{

	Move();

	Rotation();
	
	//�L�����R�����g�p����
	//�v���C���[�̍��W�ƃ��f���̍��W���X�V����
	m_charaCon.SetPosition(m_position);
	m_position = m_charaCon.Execute(
		m_moveSpeed, 
		g_gameTime->GetFrameDeltaTime()
	);

	m_beforePosition = m_position;
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.Update();

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
	//���x��������
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;

	Vector3 Rstick = m_stick->GetRStickValue();
	Vector3 Lstick = m_stick->GetLStickValue();
	Vector3 stick = Rstick + Lstick;
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	//XZ���ʂł̑O�������ƉE�������擾
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//���X�e�b�N�ƕ������x����Z������
	if (m_stick->GetStickState() == Stick::m_enStick_Both){
		m_moveSpeed += cameraFoward * stick.y * ALWAYS_SPEED * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed += cameraRight * stick.x * ALWAYS_SPEED * g_gameTime->GetFrameDeltaTime();
	}

	//�����n�ʂɒ����Ă��Ȃ��Ȃ�
	if (m_charaCon.IsOnGround() == false){
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_moveSpeed.y = 0.0f;
	}

	m_position += m_moveSpeed;
}

void Sphere::Rotation()
{
	const float rotationSpeed = 3.0f;//=1.0f*(m_protMoveSpeedMultiply/ m_moveSpeedMultiply);

	Vector3 move = m_position - m_beforePosition;
	move.y = 0.0f;

	if (fabsf(move.x) < 0.001f
		&& fabsf(move.z) < 0.001f) {
		return;
	}
	
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

