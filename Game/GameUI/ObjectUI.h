#pragma once
#include "Object/ObjectData.h"
namespace 
{
	const int OBJECT_NUM_SORT=15;
}
class ObjectUI : public IGameObject
{
public:
	~ObjectUI();
	bool Start();
	void Update();

	/// <summary>
	/// ワイプに表示するモデルを求める
	/// </summary>
	/// <param name="objectData"></param>
	void InitWipeModelUI(const StructObjectData& objectData);


public:

private:
	void Render(RenderContext& rc);

	/// <summary>
	/// ワイプ用カメラの回転
	/// </summary>
	void WipeCameraRotation();

	/// <summary>
	/// ワイプを描画出来る時間
	/// </summary>
	void WipeTimer();


	struct ObjectUIDatas
	{
		ModelRender m_objectModel;								//モデル
		FontRender m_jpName;									//モデルの日本語のフォント
		std::string m_modelName;								//モデルの名前
		bool m_isDraw = false;									//描画するかどうか
	};
private:
	Vector3 m_wipePos = Vector3::Zero;							//ワイプの座標
	Vector3 m_outlinePos = Vector3::Zero;						//輪郭線の座標
	Vector3 m_cameraPos = Vector3::Zero;						//カメラの座標
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	//エクセルでモデルの種類を確認可能
	ObjectUIDatas m_objectUIDatas[OBJECT_NUM_SORT];

	float m_drawTimer = 0.0f;
};

