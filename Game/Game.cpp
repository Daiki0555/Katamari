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
namespace {
	int a = 0;
}
Game::Game()
{
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}
Game::~Game()
{

}
bool Game::Start()
{
	NewGO<Stick>(0,"stick");

	InitLevel();
	
	NewGO<GameCamera>(0,"gameCamera");

	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetLuminance(1.0f);
	skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	skyCube->SetScale(100.0f);
	skyCube->Update();
	return true;
}

void Game::InitLevel()
{
	
	LevelRender LevelRender;
	LevelRender.Init("Assets/modelData/level/dagasi.tkl",[&](LevelObjecData& objdata) {
		if (objdata.ForwardMatchName(L"katamari")){
			//塊(コア)の作成
			m_sphere = NewGO<Sphere>(0, "sphere");
			m_sphere->SetPosition(objdata.position);
			m_sphere->SetScale(objdata.scale);
			m_sphere->SetRotation(objdata.rotation);

			//プレイヤーの作成
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objdata.position);
			m_player->SetScale(objdata.scale);
			m_player->SetRotation(objdata.rotation);
		
			return true;
		}
		else if (objdata.ForwardMatchName(L"stage")) {
			BackGround* backGround=NewGO<BackGround>(0, "backGround");
			backGround->SetPosition(objdata.position);
			backGround->SetScale(objdata.scale);
			backGround->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"o")) {
			for (auto objectData : ObjectData::GetInstance()->GetObjectData())
			{
				//wchar_t型に変換した後
				//レベルと名前が一致するかを求める
				std::wstring wchar(objectData.m_name.begin(), objectData.m_name.end());
				if (objdata.ForwardMatchName(wchar.c_str()))
				{
					Object* object = NewGO<Object>(0, "object");

					object->SetPosition(objdata.position);
					object->SetRotation(objdata.rotation);
					object->SetScale(objdata.scale);
					object->InitObject(objectData.m_name.c_str());

					//名前から移動方法を求める
					StructMoveState moveState = SerchMove(objdata.name);
					object->InitMove(moveState.m_state,moveState.m_move,moveState.m_range);
					object->SetObjectData(&objectData);
					
					m_objctList.emplace_back(object);

				}
			
			}
			return true;
		}
			
		});

}

void Game::Update()
{

}

