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
	const Vector3 FONT_POSITION = { -800.0f,-400.0f,0.0f };
	const float FONT_SCALE = 2.0f;

	const float DRAW_TIME = 2.0f;									//�`�掞��

	const Vector3	MODEL_POSITION = { 10860.0f ,-60.0f,0.0f };
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
	LevelRender LevelRender;
	LevelRender.Init("Assets/modelData/level/UILevel/UILevel.tkl", [&](LevelObjecData& objdata) {
		if (objdata.ForwardMatchName(L"o")) {
			for (auto objectData : ObjectData::GetInstance()->GetObjectData())
			{
				//wchar_t�^�ɕϊ�������
				//���x���Ɩ��O����v���邩�����߂�
				std::wstring wchar(objectData.m_name.begin(), objectData.m_name.end());
				if (objdata.ForwardMatchName(wchar.c_str())) {
					//�I�u�W�F�N�g�̖��O�������Ă���
					m_objectUIDatas[m_levelNumber].m_modelName = objectData.m_name;
					const char* objName;
					objName = m_objectUIDatas[m_levelNumber].m_modelName.c_str();
					//�t�@�C���p�X�����߂�
					char filepath[256];
					sprintf(filepath, "Assets/modelData/object/%s/%s.tkm", objName, objName);
					m_objectUIDatas[m_levelNumber].m_objectModel.InitDeferredRendering(
						filepath,
						0,
						0,
						enModelUpAxisZ,
						false,
						false,
						true
					);
					m_objectUIDatas[m_levelNumber].m_objectModel.SetPosition(MODEL_POSITION);
					m_objectUIDatas[m_levelNumber].m_objectModel.SetScale(objdata.scale);
					m_objectUIDatas[m_levelNumber].m_objectModel.SetRotation(objdata.rotation);
					m_objectUIDatas[m_levelNumber].m_objectModel.Update();
					//wchar_t�^�ɕϊ�
					std::string jpName8Str = objectData.m_jName;
					// �V�t�gJIS����UTF-16�ւ̕ϊ�
					int jpName16Int = MultiByteToWideChar(CP_ACP, 0, jpName8Str.c_str(), -1, nullptr, 0);
					std::wstring jpName16Str(jpName16Int, 0);
					MultiByteToWideChar(CP_ACP, 0, jpName8Str.c_str(), -1, &jpName16Str[0], jpName16Int);
					m_objectUIDatas[m_levelNumber].m_jpName.SetText(jpName16Str.c_str());
					m_objectUIDatas[m_levelNumber].m_jpName.SetPivot(Vector2(0.5f, 0.5f));
					m_objectUIDatas[m_levelNumber].m_jpName.SetPosition(FONT_POSITION);
					m_objectUIDatas[m_levelNumber].m_jpName.SetScale(FONT_SCALE);
					

					m_levelNumber++;
				}
			}
		}
		else {
			return true;
		}
		});
	return true;
}

void ObjectUI::Update()
{
	WipeTimer();
	Rotation();
}

void ObjectUI::WipeTimer()
{
	m_drawTimer -= g_gameTime->GetFrameDeltaTime();
	//��莞�Ԍo������
	if (m_drawTimer <= 0.0f) {
		for (int i = 0; i < m_levelNumber; i++) {
			//�`�悵�Ȃ��悤�ɂ���
			m_objectUIDatas[i].m_isDraw = false;
		}
	}
}

void ObjectUI::Rotation()
{
	//�`�悳��Ă��郂�f������
	for (int i = 0; i < m_levelNumber; i++) {
		if (m_objectUIDatas[i].m_isDraw == true) {
			m_rotation.AddRotationDegY(10.0f);
			m_objectUIDatas[i].m_objectModel.SetRotation(m_rotation);
			m_objectUIDatas[i].m_objectModel.Update();
		}
	}
}

void ObjectUI::InitWipeModelUI(StructObjectData objectData)
{
	//��ɕ`�悷�郂�f������ɂ���
	for (int i = 0; i < m_levelNumber; i++) {
		if (m_objectUIDatas[i].m_isDraw == true) {
			m_objectUIDatas[i].m_isDraw = false;
		}
	}

	for (int i = 0; i < m_levelNumber; i++) {
		if (m_objectUIDatas[i].m_modelName == objectData.m_name) {
			m_objectUIDatas[i].m_isDraw = true;
			//���ʂ̕`�掞�Ԃ�������
			m_drawTimer = DRAW_TIME;
			m_rotation = Quaternion::Identity;
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