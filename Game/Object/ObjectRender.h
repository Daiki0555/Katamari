#pragma once
#include "ObjectData.h"
class ObjectRender : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// オブジェクトの最大数を設定する
	/// </summary>
	/// <param name="maxObject"></param>
	void SetMaxObject(int maxObject)
	{
		m_maxObject = maxObject;
	}

	/// <summary>
	/// インスタンスを除去
	/// </summary>
	/// <param name="instanceNo"></param>
	void RemoveInstance(int instanceNo)
	{
		m_modelRender.RemoveInstance(instanceNo);
	}

	/// <summary>
	/// オブジェクトデータを設定する
	/// </summary>
	/// <param name="objdata"></param>
	const void SetObjectData(const StructObjectData objdata)
	{
		m_objData = objdata;
	}

	/// <summary>
	/// インスタンシングデータの更新
	/// </summary>
	/// <param name="instanceNo"></param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	void UpdateInstancingData(
		int instanceNo,
		Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale
	) {
		m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
	}

	/// <summary>
	/// 巻き込み後の更新処理
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="enModelUpAxis"></param>
	void InvolutionModelsUpdate(
		int instanceNo,
		Matrix matrix,
		EnModelUpAxis enModelUpAxis
	) {
		m_modelRender.InvolutionModelsUpdate(instanceNo,matrix, enModelUpAxis);
	}

private:
private:
	StructObjectData m_objData;
	ModelRender	m_modelRender;		// モデルレンダラー。
	int			m_maxObject = 0;	// モデルの数
};

