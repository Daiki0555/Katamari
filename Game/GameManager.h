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

	//リザルト用の構造体
	struct ResultStruct
	{
	public:
		/// <summary>
		/// オブジェクトの数加算させる
		/// </summary>
		void AddObjectCount()
		{
			m_objectCount++;
		}

		/// <summary>
		/// オブジェクトの数の取得
		/// </summary>
		/// <returns></returns>
		const int GetObjectCount()const
		{
			return m_objectCount;
		}

		/// <summary>
		/// 塊の大きさを設定
		/// </summary>
		/// <param name="scale"></param>
		/// <returns></returns>
		const void SetSphereScale(const float scale)
		{
			m_sphereScale = scale;
		}

		/// <summary>
		/// 塊の大きさの取得
		/// </summary>
		/// <returns></returns>
		const float GetSphereScale() const
		{
			return m_sphereScale;
		}


		/// <summary>
		/// 目標達成時間の設定
		/// </summary>
		/// <param name="time"></param>
		/// <returns></returns>
		const void SetClearTime(const float time)
		{
			m_clearTime = time;
		}

		/// <summary>
		/// 目標達成時間の取得
		/// </summary>
		/// <returns></returns>
		const float GetClearTime()const
		{
			return m_clearTime;
		}

		/// <summary>
		/// リザルトを初期化
		/// </summary>
		void ResultReset()
		{
			m_sphereScale = 0.0f;
			m_clearTime = 0.0f;
			m_objectCount = 0;
		}
		
	private:
		float					m_sphereScale = 0.0f;		//塊の大きさ
		float					m_clearTime = 0.0f;			//目標達成時間
		int						m_objectCount = 0;			//オブジェクトの数
	};

	//ゲームの情報用の構造体
	struct GameDataStruct
	{
	public:
		/// <summary>
		/// 目標の大きさを取得
		/// </summary>
		/// <returns></returns>
		const int GetTargetSize()const
		{
			return TARGET_SIZE;
		}

		/// <summary>
		/// 制限時間を取得
		/// </summary>
		/// <returns></returns>
		const float GetTimeLimit()const
		{
			return TIME_LIMIT;
		}

	private:
		const int		TARGET_SIZE = 18;					//目標サイズ
		const float		TIME_LIMIT = 3.0f;					//タイムリミット(分)
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
	/// BGM情報の取得
	/// </summary>
	/// <returns></returns>
	SoundSource* GetBGM()
	{
		return m_bgm;
	}

	/// <summary>
	/// BGMを鳴らす。
	/// </summary>
	/// <param name="num">鳴らしたい音の番号</param>
	void SetBGM(const int num);

	/// <summary>
	/// リザルト用の構造体の取得
	/// </summary>
	/// <returns></returns>
	ResultStruct& GetResultStruct()
	{
		return m_resultStruct;
	}

	/// <summary>
	/// ゲームデータ用の構造体の取得
	/// </summary>
	/// <returns></returns>
	GameDataStruct& GetGameDataStruct()
	{
		return m_gameDataStruct;
	}

private:
	
private:
	ResultStruct			m_resultStruct;
	GameDataStruct			m_gameDataStruct;
	EnGameSceneState		m_gameSceneState = m_enGameState_Title;			//ゲームシーンステート
	static GameManager*		m_instance;										//インスタンス
	SoundSource*			m_bgm = nullptr;								//BGM
};

