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
	//ゲームクリアの削除
	DeleteGO(m_gameClear);
	//モデルUIの削除
	DeleteGO(FindGO<ModelUI>("modelUI"));
	//タイマーUIの削除
	DeleteGO(m_timerUI);
	//オブジェクトUIの削除
	DeleteGO(FindGO<ObjectUI>("objectUI"));
	//フラワーUIの削除
	DeleteGO(m_flowerUI);
	//オブジェクトの削除
	for (auto object:m_objctList) {
		if (object->GetObjectState() == Object::m_enObject_NotInvolution) {
			object->IsDraw(false);
		}
	}
	//ゲームカメラの削除
	DeleteGO(FindGO<GameCamera>("gameCamera"));
	//プレイヤーを削除
	DeleteGO(m_player);
	//スティックを削除
	DeleteGO(FindGO<Stick>("stick"));
	//背景の削除
	DeleteGO(FindGO<BackGround>("backGround"));
	//コライダーの削除
	DeleteGO(FindGO<StageCollider>("stageCollider"));
	//スカイキューブの削除
	DeleteGO(FindGO<SkyCube>("skycube"));
	//リストのクリア
	m_objctList.clear();
	m_objectRenderList.clear();
	
}
bool Game::Start()
{
	//リストのクリア
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
	
	//スカイキューブの作成
	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetLuminance(1.0f);
	skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	skyCube->SetScale(600.0f);
	skyCube->Update();

	
	//ゲーム中にする
	GameManager::GetInstance()->SetGameSceneState(GameManager::m_enGameState_DuringGamePlay);
	//フェードイン処理
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void Game::InitLevel()
{
	int numObject = 0;
	LevelRender LevelRender;
	std::map<std::string, int>instanceCount; // 各m_nameの種類ごとのインスタンス番号管理
	LevelRender.Init("Assets/modelData/level/dagasi.tkl", [&](LevelObjecData& objdata) {
		if (objdata.ForwardMatchName(L"katamari")) {
			//塊(コア)の作成
			m_sphere = NewGO<Sphere>(0, "sphere");
			m_sphere->SetPosition(objdata.position);
			m_sphere->SetScale(objdata.scale);
			m_sphere->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"unityChan")) {
			//プレイヤーの作成
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition(objdata.position);
			m_player->SetScale(objdata.scale);
			m_player->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"stage")) {
			//ステージモデルの作成
			BackGround* backGround = NewGO<BackGround>(0, "backGround");
			backGround->SetPosition(objdata.position);
			backGround->SetScale(objdata.scale);
			backGround->SetRotation(objdata.rotation);
			return true;
		}
		else if (objdata.ForwardMatchName(L"o")) {	
			//オブジェトの作成
			//オブジェトデータのリストを持ってくる
			for (auto objectData : ObjectData::GetInstance()->GetObjectData())
			{
				//wchar_t型に変換した後
				//レベルと名前が一致するかを求める
				std::wstring wchar(objectData.m_name.begin(), objectData.m_name.end());
				//リストの名前とレベル側の名前が
				if (objdata.ForwardMatchName(wchar.c_str()))
				{
					Object* object = NewGO<Object>(0, "object");
					object->SetPosition(objdata.position);
					object->SetRotation(objdata.rotation);
					object->SetScale(objdata.scale);
					
					//名前から移動方法を求める
					StructMoveState moveState = SerchMove(objdata.name);
					object->InitMove(moveState.m_state, moveState.m_move, moveState.m_range);
					object->SetObjectData(objectData);

					// インスタンスの番号を付ける
					object->SetInstanceNo(instanceCount[objectData.m_name]);
					instanceCount[objectData.m_name]++; // インスタンス番号を更新
					m_objctList.emplace_back(object);
				}
			}
			return true;
		}
		else if (objdata.ForwardMatchName(L"Collider")) {
			//別の当たり判定を作成する
			StageCollider* stageCollider = NewGO<StageCollider>(0,"stageCollider");
			stageCollider->SetPosition(objdata.position);
			stageCollider->SetScale(objdata.scale);
			stageCollider->SetRotation(objdata.rotation);
			return true;
		}
		
		});
	//オブジェトデータのリストを持ってくる
	for (auto objectData : ObjectData::GetInstance()->GetObjectData()) {
		int numObject = 0;
		//オブジェトの数だけ回す
		for (int i = 0; i < m_objctList.size(); i++) {
			//データとオブジェクトリストの名前が一緒なら
			if (m_objctList[i]->GetObjectName() == objectData.m_name) {
				//インスタスの最大数を上げる
				numObject++;
			}
		}
		//インスタンシング描画用のレンダーを作成する
		ObjectRender* objectRender = NewGO<ObjectRender>(0, objectData.m_name.c_str());
		objectRender->SetObjectData(objectData);
		//インスタスの最大数を渡す
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

	//時間がなくなった時に
	if (GameManager::GetInstance()->GetGameSceneState()==GameManager::m_enGameState_TimeUp) {
		//クリアスケールなら
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
		//リストを渡す
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
