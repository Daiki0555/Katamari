#pragma once 
class GameManager : public Noncopyable
{
public:
	/// <summary>
	/// ゲーム状態の列挙型
	/// </summary>
	enum EnGameState
	{
		m_enState_Title,				//タイトル	
		m_enState_Game,					//ゲームシーン
		m_enState_Result				//リザルト
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
	/// BGMを鳴らす。
	/// </summary>
	/// <param name="num">鳴らしたい音の番号</param>
	void SetBGM(const int num);

private:
private:
	static GameManager* m_instance;						//インスタンス
	SoundSource* m_bgm = nullptr;						//BGM
};

