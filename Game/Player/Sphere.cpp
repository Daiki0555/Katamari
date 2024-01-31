#include "stdafx.h"
#include "Sphere.h"
#include "Object/Object.h"
#include "Game.h"
#include "Stick.h"
#include "Scene/Title.h"
namespace 
{
	//�^�C�g��
	const float		MODEL_UPPOS_RADIUS = 3.0f;							//���f���̔��a�ƍ��W
	const float		ALWAYS_TITLE_SPEED = 30.0f;							//�������̏�Z��

	const float		SPEED_DOWN = 0.99f;									//���x������
	const float		STANDARDSIZE = 40.0f;								//��̉�̒��a
	const float		ALWAYS_SPEED = 2.5f;								//�������̏�Z��
	const float		ALWAYS_RUNSPEED = 30.0f;							//���莞�̏�Z��
	const float     MAX_SPEED = 4.5f;									//�ō����x
	const float		BACK_SPEED = 2.0f;
	const float		ENTER_STICK = 0.001f;
	const float		INITIAL_RADIUS = 10.0f;								//�������a
	const float		GRAVITY = 10.0f;
	const float		DASH_AVAILABLE_TIME = 0.2f;							//�_�b�V���\����
	const int		DASH_AVAILABLE_COUNT = 5;							//�_�b�V���\�J�E���g
	const int		DASH_ROTATION_COUNT = 3;
	const float		DASH_ROTATION_SPEED = 10.0f;						//�_�b�V�����̉�]���x
	const float		MODEL_UP = 6.0f;
	const float		ROT_SPEED = 3.0f;
	const float		COLIION_YDOWNPOS = 5.0f;
	const float     BRAKE_SPEED_DOWN = 0.3f;							//�u���[�L���̃X�s�[�h������
	const float		BRAKE_ANGLE = 0.5f;									//�u���[�L���̃A���O��
	const float		BRAKE_LENGTH_SPEED = 1.5f;							//�u���[�L�o����Ƃ��̃X�s�[�h�̃x�N�g���̒���
	const float		BRAKE_LENGTH_ADDSPEED = 0.8f;						//�u���[�L�o���鎞�̃X�e�b�N�̃J�����ƃX�e�b�N�̃x�N�g���̒���
	const float		SPEED_ZERO_LENGTH = 1.3f;							//�ړ����~�܂�x�N�g���̒���
	const float		SPEED_DOWN_TIME = 0.005f;							//�X�s�[�h�������̎���	
	

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
	m_title = FindGO<Title>("title");

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

	if (GameManager::GetInstance()->GetGameSceneState() == GameManager::m_enGameState_Title) {
		//�������a�̐ݒ�
		m_radius = MODEL_UPPOS_RADIUS;
		//�L�����R���̐ݒ�
		m_charaCon.Init(
			m_radius,
			m_position
		);
	}
	else {
		//�������a�̐ݒ�
		m_radius = INITIAL_RADIUS;
		//�L�����R���̐ݒ�
		m_charaCon.Init(
			m_radius,
			m_position
		);
	}

	//���̑̐ς����߂�
	m_volume = Math::PI * pow(m_radius, 3.0f) * 4 / 3;


	return true;
}

void Sphere::Update()
{
	//�^�C�g�����̏���
	if (GameManager::GetInstance()->GetGameSceneState() == GameManager::m_enGameState_Title) {
		TitleMove();

		//�L�����R�����g�p����
		//�v���C���[�̍��W�ƃ��f���̍��W���X�V����
		m_charaCon.SetPosition(m_position);
		m_position = m_charaCon.Execute(
			m_moveSpeed,
			g_gameTime->GetFrameDeltaTime()
		);

		
		m_sphereRender.SetRotation(m_rotation);
		m_sphereRender.SetPosition(Vector3{ m_position.x,m_position.y + MODEL_UPPOS_RADIUS,m_position.z });
		m_sphereRender.Update();

		//�O�̍��W���L������
		m_beforePosition = m_position;

		if (m_title->GetIsHit()) {
			m_title->CalcMatrix();
		}

		
	}
	else if (GameManager::GetInstance()->GetGameSceneState()==GameManager::m_enGameState_DuringGamePlay) {
		DashCount();

		Move();

		Rotation();

		DashRotation();

		//�L�����R�����g�p����
		//�v���C���[�̍��W�ƃ��f���̍��W���X�V����
		m_charaCon.SetPosition(m_position);
		m_position = m_charaCon.Execute(
			m_moveSpeed,
			g_gameTime->GetFrameDeltaTime()
		);

		m_sphereRender.SetRotation(m_rotation);
		m_sphereRender.SetScale(Vector3{ m_radius,m_radius,m_radius });
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
}

void Sphere::TitleMove()
{
	//�J�����̑O����
	Vector3 cameraFoward = g_camera3D->GetForward();
	if (m_isTitleMove) {
		m_moveSpeed += cameraFoward * ALWAYS_TITLE_SPEED * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed.z *= SPEED_DOWN;
		m_moveSpeedMultiply = ALWAYS_TITLE_SPEED * (MODEL_UPPOS_RADIUS / m_radius);
		//��]��������
		Quaternion rot;
		rot.SetRotationDegX(ALWAYS_SPEED);
		m_rotation.Multiply(rot);
	}
	Vector3 Rstick = m_stick->GetRStickValue();
	Vector3 Lstick = m_stick->GetLStickValue();
	//�O�����ɂ�����x�|��Ă���Ȃ�
	if (Rstick.y > 0.5f &&
		Lstick.y > 0.5f) {
		m_isTitleMove = true;
	}
	m_position = m_moveSpeed;
}

void Sphere::Move()
{

	if (m_isReflective) {
		ReflectMove();
	}
	else {
		if (m_dashCount >= DASH_AVAILABLE_COUNT) {
			Dash();
		}
		else {

			//�ړ����x
			m_moveSpeedMultiply = ALWAYS_SPEED * (INITIAL_RADIUS / m_radius);
			Vector3 Rstick = m_stick->GetRStickValue();
			Vector3 Lstick = m_stick->GetLStickValue();
			m_inputStick = Rstick + Lstick;
			//�x�N�g���̓��ς��v�Z
			float dotProduct = Rstick.Dot(Lstick);

			//���ϕ��p�x���v�Z
			float angleRadians = acosf(dotProduct);
			float angleDegrees = angleRadians * (180.0f / Math::PI);
			//�u���[�L
			Brake();

			//�J�����̑O�����ƁA�E�����̎擾
			Vector3 cameraFoward = g_camera3D->GetForward();
			Vector3 cameraRight = g_camera3D->GetRight();

			//XZ���ʂł̑O�������ƉE�������擾
			cameraFoward.y = 0.0f;
			cameraFoward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			//���X�e�b�N�ƕ������x����Z������
			if (m_stick->GetStickState() == m_enStick_Both &&
				fabs(angleDegrees) < 85.0f) {
				m_moveSpeed += cameraFoward * m_inputStick.y * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime();
				m_moveSpeed += cameraRight * m_inputStick.x * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime();
				m_moveDownTime = 0.0f;
				m_wasMovingBackward = false;
				Vector3 speed = m_moveSpeed;
				//�����̐��K��
				speed.Normalize();
				//�x�N�g���̓��ς��v�Z
				float dotProduct = speed.Dot(cameraFoward);
				//���ϕ��p�x���v�Z
				float angleRadians = acosf(dotProduct);
				float angleDegrees = angleRadians * (180.0f / Math::PI);
				//�J�����̑O�����Ƒ����x�N�g�����ς̊p�x����肢�Ȃ��Ȃ�
				if (fabs(angleDegrees) <= 85.0f) {
					//���x�̑傫�����ō����x�𒴂��Ă��邩���肷��
					if (m_moveSpeed.Length() > MAX_SPEED) {
						//�ō����x�ɐ���
						m_moveSpeed.Normalize();
						m_moveSpeed = m_moveSpeed * MAX_SPEED;
					}
				}
				else {
					//�Ⴄ�Ȃ�ō����x���߂ɔ��肷��
					if (m_moveSpeed.Length() > BACK_SPEED) {
						m_moveSpeed.Normalize();
						m_moveSpeed = m_moveSpeed * BACK_SPEED;
					}
				}
			}
			//���������ł������Ă���	
			//�t���ɂǂ��炩�̃X�e�b�N���t�����|��Ă���Ȃ�
			else if (m_moveSpeed.Length() > SPEED_ZERO_LENGTH &&
				m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY ||
				m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
				InverseStickMove();
			}
		}

	}
		
	//���ʂɑ��x������������
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;

	Gravity();

	m_position += m_moveSpeed;
}

void Sphere::InverseStickMove() 
{
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
		IsMoving();
		//�E�X�e�b�N���v���X�Ȃ̂ō��X�e�b�N�̗ʂ�����
		Vector3 stickPower = m_stick->GetLStickValue() - m_stick->GetRStickValue();
		//��ނȂ�
		if (!m_isMovingBackward) {
			//��ނ��Ȃ��猸������
			m_moveSpeed -= cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed -= cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}
		//�O�i�Ȃ�
		else if (m_isMovingBackward) {
			//�O�i���Ȃ��猸������
			m_moveSpeed += cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed += cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}
	}
	else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
		IsMoving();
		//�E�X�e�b�N���v���X�Ȃ̂ō��X�e�b�N�̗ʂ�����
		Vector3 stickPower = m_stick->GetRStickValue() - m_stick->GetLStickValue();
		if (!m_isMovingBackward) {
			m_moveSpeed -= cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed -= cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}
		else if (m_isMovingBackward) {
			m_moveSpeed += cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed += cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}

	}
	//���x�̑傫�����ō����x�𒴂��Ă��邩���肷��
	//�ō����͒��O�̑��x
	if (m_moveSpeed.Length() > m_beforeSpeedLength) {
		//�ō����x�ɐ���
		m_moveSpeed.Normalize();
		m_moveSpeed = m_moveSpeed * m_beforeSpeedLength;
	}
	
}

void Sphere::IsMoving()
{
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//����
	Vector3 speed = m_moveSpeed;
	//�����̐��K��
	speed.Normalize();
	//�x�N�g���̓��ς��v�Z
	float dotProduct = speed.Dot(cameraFoward);
	if (!m_wasMovingBackward) {
		if (dotProduct > 0.0f) {
			m_isMovingBackward = true;
		}
		else {
			m_isMovingBackward = false;
		}
		m_wasMovingBackward = true;
		//���O�̑��x���ō����x�ɂ���
		m_beforeSpeedLength = m_moveSpeed.Length();
	}
}

void Sphere::Dash()
{
	//�J�����̑O�����ƁA�E�����̎擾
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	if (m_isDash) {
		m_moveSpeed += cameraFoward * ALWAYS_RUNSPEED * g_gameTime->GetFrameDeltaTime();
	}
	
	else {
		//�_�b�V��SE�𗬂�
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->Play(false);
		se->SetVolume(1.0f);
		//�_�b�V�����ɂ�����x���点��
		for (int i = 0; i < 200; i++) {
			m_moveSpeed += cameraFoward * ALWAYS_RUNSPEED * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= 0.9f;
		}
		m_isDash = true;
	}
}

void Sphere::Brake()
{
	Vector3 frontXZ = g_camera3D->GetForward() * m_inputStick.y;
	Vector3 rightXZ = g_camera3D->GetRight() * m_inputStick.x;
	Vector3 addSpeed = frontXZ + rightXZ;
	addSpeed.y = 0.0f;

	//�u���[�L���Ă��Ȃ����A�����̃X�e�b�N���|���ꂽ�Ƃ��Ɉړ����x��������x����Ȃ�
	if (!m_isBrake &&
		addSpeed.Length()>=BRAKE_LENGTH_ADDSPEED &&
		m_moveSpeed.Length()>=BRAKE_LENGTH_SPEED&&
		m_stick->GetStickState()!= m_enStick_FullLeftYInverseRightY&&
		m_stick->GetStickState()!=m_enStick_FullRightYInverseLeftY){
		//�O�̍��W�Ƃ̍��������߂�
		Vector3 move = m_moveSpeed;
		move.Normalize();	
		addSpeed.Normalize();
		//�ړ������ƃX�e�b�N�̌����̓���
		float angle = addSpeed.Dot(move);
		float b = fabs(acosf(angle));
		if (fabs(acosf(angle)) >= Math::PI * BRAKE_ANGLE) {
			m_moveSpeed.x *= BRAKE_SPEED_DOWN;
			m_moveSpeed.z *= BRAKE_SPEED_DOWN;
			m_isBrake = true;
			//�u���[�L�����Đ�
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(6);
			se->Play(false);
			se->SetVolume(0.5f);
		}
	}
	else {
		m_isBrake = false;
	}
}

void Sphere::DashCount()
{
	//���͂��Ԃɍ����Ă��Ȃ��Ȃ�
	if (m_dashTimer <= 0.0f) {
		m_dashCount = 0;
		m_isDash = false;
	}

	if (m_dashCount <= DASH_AVAILABLE_COUNT) {
		if (m_isInverseStick) {
			//�X�e�b�N�̌��������]�����Ȃ�
			if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}
		}
		else {
			//�X�e�b�N�̌��������]�����Ȃ�
			if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}
		}

		if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
			//���̏��������߂ĂȂ�
			if (m_dashCount == 0) {
				//true�ł����Z�ł���悤�ɂ���
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}
			m_isInverseStick = true;
		}
		else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
			//���̏��������߂ĂȂ�
			if (m_dashCount == 0) {
				//false�ł����Z�ł���悤�ɂ���
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}

			m_isInverseStick = false;
		}
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
	const float rotationSpeed = ALWAYS_SPEED *(m_moveSpeedMultiply/ALWAYS_SPEED);
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
	m_vertical = Cross(Vector3::AxisY, move);
	//�O�σx�N�g�������ɉ�]������N�H�[�^�j�I�������߂�
	Quaternion rot;
	rot.SetRotationDeg(m_vertical, length * rotationSpeed);

	//���߂��N�H�[�^�j�I������Z����
	m_rotation.Multiply(m_rotation,rot);
}

void Sphere::DashRotation()
{
	if (m_dashCount >= DASH_ROTATION_COUNT) {
		//�J�����̑O�x�N�g���Əc�x�N�g���̊O�ς����߂�
		Vector3 vertical = Cross(Vector3::AxisY, g_camera3D->GetForward());
		Quaternion rot;
		//�����Ă��Ȃ��Ȃ�
		if (fabsf(m_moveSpeed.x) < 0.001f
			&& fabsf(m_moveSpeed.z) < 0.001f) {
			//�J�����̑O�x�N�g���Əc�x�N�g���̊O�ς����ɂ���
			for (int i = 0; i < 10; i++) {
				if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
					rot.SetRotationDeg(vertical, DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
				else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
					rot.SetRotationDeg(vertical, -DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
			}
		}
		else {
			//�����Ă���Ȃ瓮���Ă�������Əc�x�N�g���̊O�ς����ɂ���
			for (int i = 0; i < 10; i++) {
				if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
					rot.SetRotationDeg(m_vertical, DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
				else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
					rot.SetRotationDeg(m_vertical, -DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
			}
		}	
		m_sphereRender.SetRotation(m_rotation);	
		m_sphereRender.Update();	
	}
}

void Sphere::ReflectMove()
{
	//���ˊJ�n���ɔ��]������
	if (m_reflectTime == REFLECTIVE_MAX_TIME) {
		//�x�N�g���𔽓]������
		m_moveSpeed = Vector3{ -m_moveSpeed.x,m_moveSpeed.y,-m_moveSpeed.z };
	}
	m_reflectTime -= g_gameTime->GetFrameDeltaTime();
	//�X�e�b�N�Ɉړ��̓��͂�����������
	if (m_stick->GetStickState() == m_enStick_Both) {
		//���ˎ��Ԃ��I����Ă���
		if (m_reflectTime < 0.0f) {
			m_isReflective = false;
			m_reflectTime = REFLECTIVE_MAX_TIME;
			Brake();
		}
	}
	
}

void Sphere::Render(RenderContext& rc)
{
	if (!m_isDraw) {
		return;
	}
	m_sphereRender.Draw(rc);
}

