#pragma once
#include "GameManager.h"
class Fade;
class Sphere;
class Object;
class ObjectRender;
class Result :public IGameObject
{
public:
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// オブジェトリストの設定
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetObjectList(const std::vector<Object*> list)
	{
		m_objctList = list;
	}

	/// <summary>
	/// オブジェトレンダーリストの設定
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetObjectRenderList(const std::vector<ObjectRender*> list)
	{
		m_objectRenderList = list;
	}

private:
	/// <summary>
	/// リザルトの種類
	/// </summary>
	enum EnResult
	{
		m_enResult_Scale,						//大きさ
		m_enResult_ClearTime,					//目標達成時間
		m_enResult_ObjectNum,					//オブジェクトの数
		m_enResult_Num							//リザルトの数
	};

	/// <summary>
	/// フラッシュの更新処理
	/// </summary>
	void flashUpdate();

	/// <summary>
	/// スフィアの塊
	/// </summary>
	void SphereUpdate();
private:
	Fade*								m_fade = nullptr;
	Sphere*								m_sphere = nullptr;
	FontRender							m_textRender[m_enResult_Num];
	FontRender							m_resultRender[m_enResult_Num];
	SpriteRender						m_flashRender;
	ModelRender							m_starRender;														//スターモデル
	ModelRender							m_earthRender;														//地球モデル
	Vector3								m_spherePos = Vector3::Zero;
	float								m_sphereTime = 0.0f;
	float								m_DirectionStartTime = 0.0f;										//演出を始める時間
	float								m_alpha = 1.0f;
	int									m_fontNumber = 1;
	bool								m_isFontDraw = true;												//フォントを描画するかどうか
	bool								m_isFlash = false;													//光らせるかどうか
	bool								m_isDirectionEnd = false;											//演出が終わったかどうか
	bool								m_isWaitFadeOut = false;											//フェード中かどうか
	bool								m_isStartBGM = false;												//BGMが始まったかどうか
	std::vector<Object*>				m_objctList;
	std::vector<ObjectRender*>			m_objectRenderList;
};

