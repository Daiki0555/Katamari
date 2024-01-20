#pragma once 
class GameManager : public Noncopyable
{
public:
	enum EnGameSceneState
	{
		m_enGameState_Title,			//タイトル
		m_enGameState_GameStart,		//ゲームスタート時
		m_enGameState_DuringGamePlay,	//ゲーム中
		m_enGameState_TimeUp,			//時間切れ
		m_enGameState_GameFade,			//ゲームフェード
		m_enGameState_GameClear,		//ゲームクリア
		m_enGameState_GameOver,			//ゲームオーバー
		m_enGameState_GameBuck,			//他のメニューに戻る
		m_enGameState_GameEnd,			//ゲーム終了
		m_enGameState_Result
	};
	~GameManager();

	bool Start();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// インスタンスの生成。
	/// </summary>
	static void CreateInstance()
	{
		//既に生成されているなら。
		if (m_instance != nullptr) {
			std::abort();
		}

		m_instance = new GameManager;
		m_instance->Start();
	}

	/// <summary>
	/// インスタンスの削除。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// インスタンスの取得。
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		if (m_instance == nullptr) {
			CreateInstance();
		}

		return m_instance;
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

	/// <summary>
	/// BGMを鳴らす。
	/// </summary>
	/// <param name="num">鳴らしたい音の番号</param>
	void SetBGM(const int num);

private:
private:
	EnGameSceneState m_gameSceneState = m_enGameState_Title;
	static GameManager* m_instance;						//インスタンス
	SoundSource* m_bgm = nullptr;						//BGM
};

