#include "stdafx.h"
#include "ObjectUI.h"
#include "Object/ObjectData.h"
namespace
{
	//���C�v
	const Vector3	WIPE_POS_MAX = { 8.0f, 662.0f, 0.0f };			//���C�v�̍ő���W
	const Vector3	WIPE_POS_MIN = { -270.0f, 662.0f, 0.0f };		//���C�v�̍ŏ����W
	const Vector3	OUTLINE_POS_MAX = { -671.5f, -280.0f, 0.0f };	//�֊s�摜�̍ő���W
	const Vector3	OUTLINE_POS_MIN = { -950.0f, -280.0f, 0.0f };	//�֊s�摜�̍ŏ����W
	//�t�H���g
	const Vector3 FONT_POSITION = { 0.0f,0.0f,0.0f };
	const float FONT_SCALE = 1.0f;

	const float DRAW_TIME = 2.0f;									//�`�掞��

}
ObjectUI::~ObjectUI()
{

}

bool ObjectUI::Start()
{
	
	m_wipePos = WIPE_POS_MIN;
	m_outlinePos = OUTLINE_POS_MIN;
	m_drawTimer = DRAW_TIME;
	//�I�u�W�F�N�g�̏�����
	for (int i = 0; i < OBJECT_NUM_SORT; i++) {
		//�I�u�W�F�N�g�̖��O�������Ă���
		m_objectUIDatas[i].m_modelName = ObjectData::GetInstance()->GetObjectData()[i].m_name.c_str();
		const char* objName;
		objName = m_objectUIDatas[i].m_modelName.c_str();
		//�t�@�C���p�X�����߂�
		char filepath[256];
		sprintf(filepath, "Assets/modelData/object/%s/%s.tkm", objName, objName);
		m_objectUIDatas[i].m_objectModel.InitToonModel(
			filepath,
			0,
			0,
			enModelUpAxisZ,
			false,
			false,
			false,
			true
		);
		m_objectUIDatas[i].m_objectModel.SetPosition(Vector3(10000.0f, 0.0f, 0.0f));
		m_objectUIDatas[i].m_objectModel.SetScale(Vector3::One);
		m_objectUIDatas[i].m_objectModel.Update();
		//wchar_t�^�ɕϊ�
		std::wstring wchar(m_objectUIDatas[i].m_modelName.begin(), m_objectUIDatas[i].m_modelName.end());
		m_objectUIDatas[i].m_jpName.SetText(wchar.c_str());
		m_objectUIDatas[i].m_jpName.SetPosition(FONT_POSITION);
		m_objectUIDatas[i].m_jpName.SetScale(FONT_SCALE);
	}
	return true;
}

void ObjectUI::Update()
{
	WipeTimer();
}

void ObjectUI::WipeTimer()
{
	m_drawTimer -= g_gameTime->GetFrameDeltaTime();
	//��莞�Ԍo������
	if (m_drawTimer >= 0.0f) {
		for (int i = 0; i < OBJECT_NUM_SORT; i++) {
			//�`�悵�Ȃ��悤�ɂ���
			m_objectUIDatas[i].m_isDraw = false;
		}
	}
}

void ObjectUI::InitWipeModelUI(const StructObjectData& objectData)
{
	//��ɕ`�悷�郂�f������ɂ���
	for (int i = 0; i < OBJECT_NUM_SORT; i++) {
		if (m_objectUIDatas[i].m_isDraw == true) {
			m_objectUIDatas[i].m_isDraw = false;
		}
	}

	for (int i = 0; i < OBJECT_NUM_SORT; i++) {
		if (m_objectUIDatas[i].m_modelName == objectData.m_name) {
			m_objectUIDatas[i].m_isDraw = true;
			//���ʂ̕`�掞�Ԃ�������
			m_drawTimer = DRAW_TIME;
		}
	}
}

void ObjectUI::Render(RenderContext& rc)
{
	for (int i = 0; i < OBJECT_NUM_SORT; i++) {
		//���f����`��o����Ȃ�
		if (m_objectUIDatas[i].m_isDraw == true) {
			m_objectUIDatas[i].m_objectModel.Draw(rc);
			m_objectUIDatas[i].m_jpName.Draw(rc);
		}
	}
}