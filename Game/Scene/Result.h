#pragma once
class Fade;
class Result :public IGameObject
{
public:
	//リザルト用の構造体
	struct ResultStruct
	{
		float	m_sphereScale = 0.0f;		//塊の大きさ
		float	m_clearTime = 0.0f;			//目標達成時間
		int		m_objectCount = 0;			//オブジェクトの数
	};

	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// リザルト用の構造体の取得
	/// </summary>
	/// <returns></returns>
	const ResultStruct& GetResultStruct() const {
		return m_resultStruct;
	}
private:

private:
	ResultStruct			m_resultStruct;									//リザルト用の構造体
	Fade*					m_fade = nullptr;
	SpriteRender			m_backRender;
};

