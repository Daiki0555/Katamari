#pragma once
#include "GameManager.h"
class Player;
class Sphere;
class Object;
class Stick;
class FlowerUI;
class TimerUI;
class GameClear;
class Fade;
class Game :public IGameObject
{
public:
	enum EnGameSceneState
	{
		m_enGameState_GameStart,		//ゲームスタート時
		m_enGameState_DuringGamePlay,	//ゲーム中
		m_enGameState_TimeUp,			//時間切れ
		m_enGameState_GameFade,			//ゲームフェード
		m_enGameState_GameClear,		//ゲームクリア
		m_enGameState_GameOver,			//ゲームオーバー
		m_enGameState_GameBuck,			//他のメニューに戻る
		m_enGameState_GameEnd			//ゲーム終了
	};

	Game();
	~Game();
	bool Start();
	void Update();

	void InitSound();

	/// <summary>
	/// オブジェクトリストの設定
	/// </summary>
	/// <param name="object"></param>
	/// <returns></returns>
	const void SetObjectList(Object* object)
	{
		m_objctList.emplace_back(object);
	}

	/// <summary>
	/// オブジェクトリストの設定
	/// </summary>
	/// <returns></returns>
	const std::vector<Object*> GetObjectList()
	{
		return m_objctList;
	}

	/// <summary>
	/// クリア可能かどうかの設定
	/// </summary>
	/// <returns></returns>
	const void SetClearState(const bool state) 
	{
		m_isGameClearable = state;
	}

	/// <summary>
	/// ゲームステートの設定
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetGameSceneState(const EnGameSceneState state)
	{
		m_gameSceneState = state;
	}

	/// <summary>
	/// ゲームステートの取得
	/// </summary>
	/// <returns></returns>
	const EnGameSceneState GetGameSceneState()const
	{
		return m_gameSceneState;
	}


private:
	/// <summary>
	/// レベルファイルの初期化
	/// </summary>
	void InitLevel();
	
	/// <summary>
	/// クリア処理
	/// </summary>
	void ClearProcessing();

	/// <summary>
	/// ゲームオーバー処理
	/// </summary>
	void GameOverProcessing();

	void Render(RenderContext& rc);
private:
	EnGameSceneState m_gameSceneState = m_enGameState_GameStart;
	SpriteRender m_clearRender;
	Player* m_player=nullptr;
	Sphere* m_sphere = nullptr;
	FlowerUI* m_flowerUI = nullptr;
	TimerUI* m_timerUI = nullptr;
	GameClear* m_gameClear = nullptr;
	Fade* m_fade = nullptr;
	bool m_isGameClearable = false;
	bool m_isStartBGM = false;			//BGMが始まったかどうか
	std::vector<Object*>				m_objctList;
};

