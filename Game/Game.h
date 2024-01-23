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
class Result;
class Game :public IGameObject
{
public:
	enum EnGameState
	{
		m_enGameState_GameClear,		//ゲームクリア
		m_enGameState_GameOver,			//ゲームオーバー
	};

	Game();
	~Game();
	bool Start();
	void Update();

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
	const void SetGameSceneState(const EnGameState state)
	{
		m_gameState = state;
	}

	/// <summary>
	/// ゲームステートの取得
	/// </summary>
	/// <returns></returns>
	const EnGameState GetGameSceneState()const
	{
		return m_gameState;
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
	EnGameState							m_gameState = m_enGameState_GameOver;		//ゲームステート
	SpriteRender						m_clearRender;
	Player*								m_player = nullptr;
	Sphere*								m_sphere = nullptr;
	FlowerUI*							m_flowerUI = nullptr;
	TimerUI*							m_timerUI = nullptr;
	GameClear*							m_gameClear = nullptr;
	Fade*								m_fade = nullptr;
	Result* m_result = nullptr;
	bool								m_isGameClearable = false;						//クリア可能かどうか
	bool								m_isStartBGM = false;							//BGMが始まったかどうか
	bool								m_isWaitFadeOut = false;						//フェード中かどうか								
	std::vector<Object*>				m_objctList;
};

