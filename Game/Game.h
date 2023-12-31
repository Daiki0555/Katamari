#pragma once

class Player;
class Sphere;
class Object;
class Stick;
class Game :public IGameObject
{
public:
	enum EnGameSceneState
	{
		m_enGameState_GameStart,		//ゲームスタート時
		m_enGameState_DuringGamePlay,	//ゲーム中
		m_enGameState_GameClearable,	//クリア可能
		m_enGameState_GameClear,		//クリア
		m_enGameState_GameFade,			//ゲームフェード
		m_enGameState_GameOver,			//ゲームオーバー
		m_enGameState_GameBuck			//他のメニューに戻る
	};

	enum EnGameClearState {
		m_enGameClearState_GameClearable,	//クリア可能
		m_enGameClearState_GameUnclearable, //クリア不可能
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
public:
private:
	/// <summary>
	/// レベルファイルの初期化
	/// </summary>
	void InitLevel();
private:
	EnGameClearState m_gameClearState = m_enGameClearState_GameUnclearable;
	Player* m_player=nullptr;
	Sphere* m_sphere = nullptr;

	std::vector<Object*>				m_objctList;
};

