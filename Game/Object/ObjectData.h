#pragma once
/// <summary>
/// エクセルのオブジェクトデータ
/// </summary>
struct StructObjectData
{
	std::string m_name;			//オブジェクトデータの名前
	std::string m_jName;		//日本語での名前
	float m_volume;				//オブジェクトの体積
	float m_involutionScale;	//巻き込まれる大きさ
	int m_soundNumber;			//SEの番号
};

class ObjectData
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// オブジェクトデータを返す
	/// </summary>
	/// <returns></returns>
	std::vector<StructObjectData> GetObjectData()
	{
		return objects;
	};

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	/// </summary>
	static void CreateInstance()
	{
		if (m_instance != nullptr)
		{
			std::abort();
		}
		m_instance = new ObjectData;
		m_instance->Init();
	}

	/// <summary>
	/// インスタンスの削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static ObjectData* GetInstance()
	{
		if (m_instance == nullptr) {
			CreateInstance();
		}

		return m_instance;
	}
public:
private:

private:
	std::vector<StructObjectData> objects;
	static ObjectData* m_instance;

};

