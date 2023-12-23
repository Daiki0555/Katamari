#pragma once
namespace 
{
	const int FLOWER_NUMBER = 5;
}
class Sphere;
class FlowerUI : public IGameObject
{

public:
	~FlowerUI();
	bool Start();
	void Update();
	/// <summary>
	/// 共通の拡大
	/// </summary>
	void CommonMagnification();

	/// <summary>
	/// 目標の大きさかどうかの取得
	/// </summary>
	/// <returns></returns>
	const bool GetIsObjectiveScale() const
	{
		return m_isObjectiveScale;
	}

private:
	void Render(RenderContext& rc);
	void Rotation();
	
	/// <summary>
	/// サイズの拡大
	/// </summary>
	void Scaling();

	/// <summary>
	/// フォントの処理
	/// </summary>
	void Font();



	struct StructFlowerUI
	{
		SpriteRender m_flowerRender;
		Vector3 m_scale = Vector3::One;			
		bool m_isUpState = true;				//拡大するならtrue縮小するならfalse
	};
private:
	Sphere* m_sphere;
	StructFlowerUI m_flowerUI[FLOWER_NUMBER];
	SpriteRender m_ringRender;
	FontRender m_fontRender;
	FontRender m_objectiveRender;								//目標を描画する

	float m_commonScale = 1.0f;									//共通の拡大率
	
	float m_initialRadius = 0.0f;								//初期半径

	Quaternion m_rotation = Quaternion::Identity;

	bool m_isObjectiveScale = false;							//大きさが目標かどうか
};

