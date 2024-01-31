#include "stdafx.h"
#include "Result.h"
#include "Player/Sphere.h"
#include "Object/Object.h"
#include "Object/ObjectRender.h"
#include "Scene/Fade.h"
#include "Title.h"
namespace
{
	//��̑傫��
	const Vector3 RESULT_KATAMRI_FONT_POS = { 400.0f,150.0f,0.0f };
	const Vector3 RESULT_KATAMRI_TEXT_POS = { 400.0f,200.0f,0.0f };
	const float	  RESULT_KATAMRI_FONT_SCALE = 2.0f;
	
	//�ڕW�B������
	const Vector3 RESULT_CLEAR_FONT_POS = { 400.0f,0.0f,0.0f };
	const Vector3 RESULT_CLEAR_TEXT_POS = { 400.0f,50.0f,0.0f };
	const float	  RESULT_CLEAR_FONT_SCALE = 1.0f;
	
	//�������񂾃I�u�W�F�N�g�̐�
	const Vector3 RESULT_OBJECTNUM_FONT_POS = { 400.0f,-110.0f,0.0f };
	const Vector3 RESULT_OBJECTNUM_TEXT_POS = { 400.0f,-60.0f,0.0f };
	const float	  RESULT_OBJECTNUM_FONT_SCALE = 1.0f;
	
	//�e�L�X�g�̋��ʂ̒l
	const Vector3 TEXT_COLOR = { 0.0f,0.0f,1.0f };				//�e�L�X�g�����_�[�p�̃J���[
	const float   TEXT_SCALE = 1.0f;
	const Vector3 CAMERA_POS = { 0.0f,0.0f,-60.0f };			//�J�����̃|�W�V����
	const float   ADD_SPHERE_POS = 400.0f;						//�X�t�B�A�����W�����Z�������
	const float	  SPHERE_UPDATE_TIME = 4.0f;

	//�X�^�[���f��
	const Vector3 STAR_SCALE = { 1.0f,1.0f,1.0f };

	//���̖��O�p�̒l
	const Vector3 STAR_FONT_POS = { -200.0f,-200.0f,0.0f };
	const float	  STAR_FONT_SCALE = 2.0f;

	const Vector3 RESULT_CLEAR_FONT_POS2 = { -200.0f,-300.0f,0.0f };

	//�n�����f��
	const Vector3 EARTH_POS = {0.0f,-60.0f,0.0f};
	const Vector3 EARTH_SCALE = {2.0f,1.0f,2.0f};

	const float	  SPEED_DOWN = 2.0f;							//����㏸�����鉺����
	
	const float	  DIRECTION_START_TIME = 0.5f;
}

Result::~Result()
{
	for (auto object:m_objctList) {
		DeleteGO(object);
	}
	//�C���X�^���V���O�`��p�̃I�u�W�F�N�g�����_�[�̍폜
	for (auto object : m_objectRenderList) {
		DeleteGO(object);
	}
	//���X�g�̃N���A
	m_objctList.clear();
	m_objectRenderList.clear();
	//�X�J�C�L���[�u�̍폜
	DeleteGO(FindGO<SkyCube>("skycube"));
	//����폜
	DeleteGO(m_sphere);
}

bool Result::Start()
{
	//�t�F�[�h�C��
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	m_sphere = FindGO<Sphere>("sphere");
	m_sphere->GetSphereModel().SetPosition(Vector3::Zero);
	m_sphere->IsDraw(true);
	m_sphere->GetSphereModel().Update();
	
	//��̑傫��
	//���U���g�p�̐ݒ�
	float value = GameManager::GetInstance()->GetResultStruct().GetSphereScale();
	//cm��mm�ɕ���
	int cm = static_cast<int>(value);
	int mm = static_cast<int>((value - cm) * 10);
	wchar_t scaleText[255];
	swprintf_s(scaleText, L"%dcm %dmm", cm, mm);
	m_resultRender[m_enResult_Scale].SetText(scaleText);
	m_resultRender[m_enResult_Scale].SetPosition(RESULT_KATAMRI_FONT_POS);
	m_resultRender[m_enResult_Scale].SetColor(Vector3::One);
	m_resultRender[m_enResult_Scale].SetScale(RESULT_KATAMRI_FONT_SCALE);

	//�e�L�X�g�̐ݒ�
	swprintf_s(scaleText, L":�傫��");
	m_textRender[m_enResult_Scale].SetText(scaleText);
	m_textRender[m_enResult_Scale].SetPosition(RESULT_KATAMRI_TEXT_POS);
	m_textRender[m_enResult_Scale].SetColor(TEXT_COLOR);
	m_textRender[m_enResult_Scale].SetScale(TEXT_SCALE);

	//�ڕW�B������
	float clearTime = GameManager::GetInstance()->GetResultStruct().GetClearTime();
	int minutes = static_cast<int>(floor(clearTime / 60));
	int remainingSeconds = static_cast<int>(fmod(clearTime, 60.0f));
	wchar_t timeText[255];
	swprintf_s(timeText, L"%d��%d�b", minutes, remainingSeconds);
	m_resultRender[m_enResult_ClearTime].SetText(timeText);
	m_resultRender[m_enResult_ClearTime].SetPosition(RESULT_CLEAR_FONT_POS);
	m_resultRender[m_enResult_ClearTime].SetColor(Vector3::One);
	m_resultRender[m_enResult_ClearTime].SetScale(RESULT_CLEAR_FONT_SCALE);

	//�e�L�X�g�̐ݒ�
	swprintf_s(timeText, L":�ڕW�B��");
	m_textRender[m_enResult_ClearTime].SetText(timeText);
	m_textRender[m_enResult_ClearTime].SetPosition(RESULT_CLEAR_TEXT_POS);
	m_textRender[m_enResult_ClearTime].SetColor(TEXT_COLOR);
	m_textRender[m_enResult_ClearTime].SetScale(TEXT_SCALE);


	//�������񂾃I�u�W�F�N�g�̐�
	int objectNum = GameManager::GetInstance()->GetResultStruct().GetObjectCount();
	wchar_t objectText[255];
	swprintf_s(objectText, L"%d�R", objectNum);
	m_resultRender[m_enResult_ObjectNum].SetText(objectText);
	m_resultRender[m_enResult_ObjectNum].SetPosition(RESULT_OBJECTNUM_FONT_POS);
	m_resultRender[m_enResult_ObjectNum].SetColor(Vector3::One);
	m_resultRender[m_enResult_ObjectNum].SetScale(RESULT_OBJECTNUM_FONT_SCALE);

	//�e�L�X�g�̐ݒ�
	swprintf_s(objectText, L":���m");
	m_textRender[m_enResult_ObjectNum].SetText(objectText);
	m_textRender[m_enResult_ObjectNum].SetPosition(RESULT_OBJECTNUM_TEXT_POS);
	m_textRender[m_enResult_ObjectNum].SetColor(TEXT_COLOR);
	m_textRender[m_enResult_ObjectNum].SetScale(TEXT_SCALE);

	//���̖��O��ݒ�
	wchar_t starText[255];
	swprintf_s(starText, L"�J�^�}����");
	m_starNameFont.SetText(starText);
	m_starNameFont.SetPosition(STAR_FONT_POS);
	m_starNameFont.SetScale(STAR_FONT_SCALE);
	m_starNameFont.SetColor(Vector3::One);


	//�t���b�V���p�̃X�v���C�g��������
	m_flashRender.Init("Assets/sprite/Result/white.DDS", 1920, 1080);
	m_flashRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,0.0f });
	m_flashRender.Update();

	//�X�^�[���f���̏�����
	m_starRender.InitDeferredRendering(
		"Assets/modelData/clear/star.tkm",
		0,
		0,
		enModelUpAxisY
	);
	m_starRender.SetScale(STAR_SCALE);
	m_starRender.Update();

	//�n�����f��
	m_earthRender.InitDeferredRendering("Assets/modelData/UI/earth.tkm");
	m_earthRender.SetPosition(EARTH_POS);
	m_earthRender.SetScale(EARTH_SCALE);
	m_earthRender.Update();


	//�X�J�C�L���[�u�̍쐬
	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetLuminance(1.0f);
	skyCube->SetType((EnSkyCubeType)enSkyCubeType_Night);
	skyCube->SetScale(600.0f);
	skyCube->Update();

	g_camera3D->SetTarget(m_sphere->GetSphereModel().GetPosition());
	g_camera3D->SetPosition(CAMERA_POS);
	return true;
}

void Result::Update()
{
	//���o���I���������A�{�^���������ꂽ��
	if (m_isDirectionEnd&&
		g_pad[0]->IsTrigger(enButtonA)) {
		m_fade->StartFadeOut();
		m_isWaitFadeOut = true;
	}

	if (m_isWaitFadeOut) {
		if (!m_fade->IsFade()) {
			//�Q�[���}�l�[�W���[�̒l���폜����
			GameManager::GetInstance()->GetResultStruct().ResultReset();
			GameManager::GetInstance()->DeleteInstance();
			//���g�������ă^�C�g���𐶐�����
			NewGO<Title>(0, "title");
			DeleteGO(this);

		}
	}

	//�t�F�[�h���I�������
	if (!m_fade->IsFade()) {
		//���o���n�߂�
		if (g_pad[0]->IsTrigger(enButtonA)&&
			!m_isDramSE) {
			GameManager::GetInstance()->GetBGM()->Stop();
			//�������܂�SE�̍Đ��@	
			m_dramSE = NewGO<SoundSource>(0);
			m_dramSE->Init(11);
			m_dramSE->Play(false);
			m_dramSE->SetVolume(0.3f);
			m_isDramSE = true;
			m_isFontDraw = false;
			//�傫���̃t�H���g�͕ʂŎg���̂ō��W�Ƒ傫�����Đݒ�
			m_resultRender[m_enResult_Scale].SetPosition(RESULT_CLEAR_FONT_POS2);
		}
		if (!m_isStartBGM) {
			GameManager::GetInstance()->SetBGM(12);
			m_isStartBGM = true;
		}
	}

	if (!m_isFontDraw) {
		SphereUpdate();
	}

	//��Ɠ������W�ɂ���
	m_starRender.SetPosition(m_spherePos);
	m_starRender.Update();

	for (auto object : m_objctList) {
		if (object->GetIsDraw()) {
			object->CalcMatrix();
		}
	}

	//�J�����̍��W���������ɂ������_����ɂ���
	g_camera3D->SetTarget(m_sphere->GetSphereModel().GetPosition());
	g_camera3D->SetPosition(CAMERA_POS);
}

void Result::SphereUpdate()
{
	
	if (m_sphereTime < SPHERE_UPDATE_TIME) {
		m_sphereTime += g_gameTime->GetFrameDeltaTime();
		m_spherePos.y += ADD_SPHERE_POS * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_dramSE->Stop();
		//�I�u�W�F�N�g�̍폜
		for (auto object : m_objctList) {
			if (object->GetObjectState() == Object::m_enObject_Involution) {
				object->IsDraw(false);
			}
		}
		m_sphere->IsDraw(false);
		m_isFlash = true;
		flashUpdate();
	}
	m_sphere->GetSphereModel().SetPosition(m_spherePos);
	m_sphere->GetSphereModel().Update();
}

void Result::flashUpdate()
{		
	m_flashRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,m_alpha });
	m_flashRender.Update();
	//�����̊Ԕ����C���X�g�o��������悤�ɂ���
	m_DirectionStartTime += g_gameTime->GetFrameDeltaTime();
	if (m_DirectionStartTime < DIRECTION_START_TIME) {
		return;
	}
	//���l������������
	m_alpha -= g_gameTime->GetFrameDeltaTime() / SPEED_DOWN;
	m_alpha = max(m_alpha, 0.0f);
	if (m_alpha == 0.0f) {
		//���o���I����Ă��Ȃ��Ȃ�
		if (!m_isDirectionEnd) {
			//�������܂�SE�̍Đ��@
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(9);
			se->Play(false);
			se->SetVolume(0.2f);
		}
		m_isDirectionEnd = true;
	}
}


void Result::Render(RenderContext& rc)
{
	if (m_isFlash) {
		m_starNameFont.Draw(rc);
		m_resultRender[m_enResult_Scale].Draw(rc);
		m_flashRender.Draw(rc);
		m_starRender.Draw(rc);
		return;
	}
	if (m_isFontDraw) {
		for (int i = 0; i < m_enResult_Num; i++) {
			m_resultRender[i].Draw(rc);
			m_textRender[i].Draw(rc);
		}
	}
	m_earthRender.Draw(rc);
}