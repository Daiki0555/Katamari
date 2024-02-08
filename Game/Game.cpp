#include "stdafx.h"
#include "Game.h"
#include "Player/Player.h"
#include "Player/Sphere.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Object/Object.h"
#include "Object/ObjectData.h"
#include "Stick.h"
#include "Object/WordManager.h"
#include "GameUI/ObjectUI.h"
#include "GameUI/TimerUI.h"
#include "GameUI/FlowerUI.h"
#include "GameUI/ModelUI.h"
#include "StageCollider.h"
#include "Scene/GameClear.h"
#include "Object/ObjectRender.h"
#include "Scene/Fade.h"
#include "Scene/Result.h"
#include <sstream>

Game::Game()
{
	
}
Game::~Game()
{
	//�Q�[���N���A�̍폜
	DeleteGO(m_gameClear);
	//���f��UI�̍폜
	DeleteGO(FindGO<ModelUI>("modelUI"));
	//�^�C�}�[UI�̍폜
	DeleteGO(m_timerUI);
	//�I�u�W�F�N�gUI�̍폜
	DeleteGO(FindGO<ObjectUI>("objectUI"));
	//�t�����[UI�̍폜
	DeleteGO(m_flowerUI);
	//�I�u�W�F�N�g�̍폜
	for (auto object:m_objctList) {
		if (object->GetObjectState() == Object::m_enObject_NotInvolution) {
			object->IsDraw(false);
		}
	}
	//�Q�[���J�����̍폜
	DeleteGO(FindGO<GameCamera>("gameCamera"));
	//�v���C���[���폜
	DeleteGO(m_player);
	//�X�e�B�b�N���폜
	DeleteGO(FindGO<Stick>("stick"));
	//�w�i�̍폜
	DeleteGO(FindGO<BackGround>("backGround"));
	//�R���C�_�[�̍폜
	DeleteGO(FindGO<StageCollider>("stageCollider"));
	//�X�J�C�L���[�u�̍폜
	DeleteGO(FindGO<SkyCube>("skycube"));
	//���X�g�̃N���A
	m_objctList.clear();
	m_objectRenderList.clear();
	
}
bool Game::Start()
{
	//���X�g�̃N���A
	m_objctList.clear();
	m_objectRenderList.clear();

	NewGO<Stick>(0,"stick");
	InitLevel();
	NewGO<ObjectUI>(0, "objectUI");
	NewGO<GameCamera>(0,"gameCamera");
	m_timerUI = NewGO<TimerUI>(0, "timerUI");
	m_flowerUI = NewGO<FlowerUI>(0, "flowerUI");
	NewGO<ModelUI>(0, "modelUI");
	m_gameClear=NewGO<GameClear>(0, "gameClear");
	
	//�X�J�C�L���[�u�̍쐬
	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetLuminance(1.0f);
	skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	skyCube->SetScale(600.0f);
	skyCube->Update();

	
	//�Q�[�����ɂ���
	GameManager::GetInstance()->SetGameSceneState(GameManager::m_enGameState_DuringGamePlay);
	//�t�F�[�h�C������
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Game::InitLevel()
{
	int numObject = 0;
	LevelRender LevelRender;
	std::map<std::string, int>instanceCount; // �em_name�̎�ނ��Ƃ̃C���X�^���X�ԍ��Ǘ�
	LevelRender.Init("Assets/modelData/level/dagasi.tkl", [&](LevelObjecData& objdata) {
		if (objdata.ForwardMatchName(L"katamari")) {
			//��(�R�A)�̍쐬
			m_sphere = NewGO<Sphere>(0, "sphere");
			m_sphere->SetPosition(objdata.position);
			m_sphere->SetScale(objdata.scale);
			m_sphere->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"unityChan")) {
			//�v���C���[�̍쐬
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objdata.position);
			m_player->SetScale(objdata.scale);
			m_player->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"stage")) {
			//�X�e�[�W���f���̍쐬
			BackGround* backGround = NewGO<BackGround>(0, "backGround");
			backGround->SetPosition(objdata.position);
			backGround->SetScale(objdata.scale);
			backGround->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"o")) {	
			//�I�u�W�F�g�̍쐬
			//�I�u�W�F�g�f�[�^�̃��X�g�������Ă���
			for (auto objectData : ObjectData::GetInstance()->GetObjectData())
			{
				//wchar_t�^�ɕϊ�������
				//���x���Ɩ��O����v���邩�����߂�
				std::wstring wchar(objectData.m_name.begin(), objectData.m_name.end());
				//���X�g�̖��O�ƃ��x�����̖��O��
				if (objdata.ForwardMatchName(wchar.c_str()))
				{
					Object* object = NewGO<Object>(0, "object");
					object->SetPosition(objdata.position);
					object->SetRotation(objdata.rotation);
					object->SetScale(objdata.scale);
					
					//���O����ړ����@�����߂�
					StructMoveState moveState = SerchMove(objdata.name);
					object->InitMove(moveState.m_state, moveState.m_move, moveState.m_range);
					object->SetObjectData(objectData);

					// �C���X�^���X�̔ԍ���t����
					object->SetInstanceNo(instanceCount[objectData.m_name]);
					instanceCount[objectData.m_name]++; // �C���X�^���X�ԍ����X�V
					m_objctList.emplace_back(object);
				}
			}
			return true;
		}
		else if (objdata.ForwardMatchName(L"Collider")) {
			//�ʂ̓����蔻����쐬����
			StageCollider* stageCollider = NewGO<StageCollider>(0,"stageCollider");
			stageCollider->SetPosition(objdata.position);
			stageCollider->SetScale(objdata.scale);
			stageCollider->SetRotation(objdata.rotation);
			return true;
		}
		
		});
	//�I�u�W�F�g�f�[�^�̃��X�g�������Ă���
	for (auto objectData : ObjectData::GetInstance()->GetObjectData()) {
		int numObject = 0;
		//�I�u�W�F�g�̐�������
		for (int i = 0; i < m_objctList.size(); i++) {
			//�f�[�^�ƃI�u�W�F�N�g���X�g�̖��O���ꏏ�Ȃ�
			if (m_objctList[i]->GetObjectName() == objectData.m_name) {
				//�C���X�^�X�̍ő吔���グ��
				numObject++;
			}
		}
		//�C���X�^���V���O�`��p�̃����_�[���쐬����
		ObjectRender* objectRender = NewGO<ObjectRender>(0, objectData.m_name.c_str());
		objectRender->SetObjectData(objectData);
		//�C���X�^�X�̍ő吔��n��
		objectRender->SetMaxObject(numObject);
		m_objectRenderList.emplace_back(objectRender);
	}
}


void Game::Update()
{
	if (!m_fade->IsFade()){
		if (!m_isStartBGM) {
			GameManager::GetInstance()->SetBGM(10);
			m_isStartBGM = true;
		}
		m_gameTime += g_gameTime->GetFrameDeltaTime();
	}

	//���Ԃ��Ȃ��Ȃ�������
	if (GameManager::GetInstance()->GetGameSceneState()==GameManager::m_enGameState_TimeUp) {
		//�N���A�X�P�[���Ȃ�
		if (m_isGameClearable) {
			m_gameClear->IsClearStart(true);
			m_gameState = m_enGameState_GameClear;
		}
		else {
			m_gameState = m_enGameState_GameOver;
		}
	}

	if (m_isWaitFadeOut) {
		ClearProcessing();
	}
	else {
		if (GameManager::GetInstance()->GetGameSceneState() == GameManager::m_enGameState_GameEnd) {
			if (m_gameState == m_enGameState_GameClear) {
				m_fade->StartFadeOut();
				m_fade->IsGameStart(false);
				m_isWaitFadeOut = true;
				GameManager::GetInstance()->GetResultStruct().SetSphereScale(m_sphere->GetRadius());
			}
			else {

			}
		}
	}

	int sphereRadius = m_sphere->GetRadius();
	if (sphereRadius >= GameManager::GetInstance()->GetGameDataStruct().GetTargetSize()&&
		!m_isClear) {
		GameManager::GetInstance()->GetResultStruct().SetClearTime(m_gameTime);
		m_isClear = true;
	}

	
	
	
}

void Game::ClearProcessing()
{
	m_volumeBGM -= g_gameTime->GetFrameDeltaTime();
	m_volumeBGM = max(m_volumeBGM, 0.0f);
	GameManager::GetInstance()->GetBGM()->SetVolume(m_volumeBGM);
	if (m_volumeBGM <= 0.0f) {
		m_isStopBGM = true;
	}
	if (!m_fade->IsFade()&&
		m_isStopBGM) {

		Result* result = NewGO<Result>(0, "result");
		//���X�g��n��
		result->SetObjectList(m_objctList);
		result->SetObjectRenderList(m_objectRenderList);
		DeleteGO(this);
	}
}

void Game::GameOverProcessing()
{

}

void Game::Render(RenderContext& rc)
{

}
