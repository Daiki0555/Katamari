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
#include <sstream>

Game::Game()
{
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}
Game::~Game()
{

}
bool Game::Start()
{
	
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
	skyCube->SetScale(500.0f);
	skyCube->Update();

	//BGM�̐ݒ�
	//SoundSource* m_bgm = NewGO<SoundSource>(0);
	//m_bgm->Init(10);
	//m_bgm->SetVolume(0.0f);
	//m_bgm->Play(true);
	//�Q�[�����ɂ���
	m_gameSceneState = m_enGameState_DuringGamePlay;
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
			BackGround* backGround = NewGO<BackGround>(0, "backGround");
			backGround->SetPosition(objdata.position);
			backGround->SetScale(objdata.scale);
			backGround->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"o")) {
			for (auto objectData : ObjectData::GetInstance()->GetObjectData())
			{
				//wchar_t�^�ɕϊ�������
				//���x���Ɩ��O����v���邩�����߂�
				std::wstring wchar(objectData.m_name.begin(), objectData.m_name.end());
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

					// �C���X�^���X�ԍ���t����
					object->SetInstanceNo(instanceCount[objectData.m_name]);
					instanceCount[objectData.m_name]++; // �C���X�^���X�ԍ����X�V
					m_objctList.emplace_back(object);
				}
			}
			return true;
		}
		else if (objdata.ForwardMatchName(L"Collider")) {
			StageCollider* stageCollider = NewGO<StageCollider>(0,"stageCollider");
			stageCollider->SetPosition(objdata.position);
			stageCollider->SetScale(objdata.scale);
			stageCollider->SetRotation(objdata.rotation);
			return true;
		}
		
		});
	for (auto objectData : ObjectData::GetInstance()->GetObjectData()) {
		int numObject = 0;
		for (int i = 0; i < m_objctList.size(); i++) {
			if (m_objctList[i]->GetObjectName() == objectData.m_name) {
				numObject++;
			}
		}
		ObjectRender* objectRender = NewGO<ObjectRender>(0, objectData.m_name.c_str());
		objectRender->SetObjectData(objectData);
		objectRender->SetMaxObject(numObject);
	}
}

void Game::InitSound()
{
	//BGM
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/BGM/BGM.wav");
}

void Game::Update()
{
	//���Ԃ��Ȃ��Ȃ�������
	if (m_gameSceneState==m_enGameState_TimeUp) {
		//�N���A�X�P�[���Ȃ�
		if (m_isGameClearable) {
			m_gameClear->IsClearStart(true);
			m_gameSceneState = m_enGameState_GameClear;
		}
		else {
			m_gameSceneState = m_enGameState_GameOver;
		}
	}

	if (m_gameSceneState == m_enGameState_GameEnd) {
		int hoge = 0;
	}
}

void Game::ClearProcessing()
{

}

void Game::GameOverProcessing()
{

}

void Game::Render(RenderContext& rc)
{

}
