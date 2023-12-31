#pragma once
#include "Move/IMove.h"
#include "ObjectData.h"
class Sphere;
class Game;
class IMove;
class ObjectUI;
class FlowerUI;
class Object :public IGameObject
{
public:

	//オブジェクトの状態
	enum EnObjectState {
		m_enObject_NotInvolution,								//オブジェクトが巻き込まれていない状態
		m_enObject_Involution,									//オブジェクトが巻き込まれている状態
	};

	

	~Object();
	bool Start();

	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 動きの初期化
	/// </summary>
	void InitMove(EnMoveState state,const float speed,const float range);

	/// <summary>
	/// オブジェクトの読み込み処理
	/// </summary>
	void InitObject(const char* objName);

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	const void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()const
	{
		return m_scale;
	}

	/// <summary>
	/// オブジェクトの状態を取得
	/// </summary>
	/// <returns></returns>
	const EnObjectState GetObjectState() const
	{
		return m_objectState;
	}

	/// <summary>
	/// ワールド行列の計算
	/// </summary>
	void CalcMatrix();

	/// <summary>
	/// オブジェクトデータを設定する
	/// </summary>
	/// <param name="objdata"></param>
	const void SetObjectData(const StructObjectData objdata)
	{
		m_objData = objdata;
	}

	/// <summary>
	/// オブジェクトの名前を返す
	/// </summary>
	/// <returns></returns>
	const std::string GetObjectName()const
	{
		return m_objData.m_name;
	}

private:

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	

	/// <summary>
	/// コリジョンの初期化
	/// </summary>
	void InitCollision();
	

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Hit();

	/// <summary>
	/// 巻き込み処理
	/// </summary>
	void Involution();

	

private:
	ModelRender m_objectRender;
	CollisionObject m_collisionObject;
	PhysicsStaticObject m_physicsObj;
	
	StructObjectData m_objData;											//オブジェクトのデータ
	
	Sphere* m_sphere;
	FlowerUI* m_flowerUI;

	ObjectUI* m_objectUI;
	
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	
	Quaternion m_rotation = Quaternion::Identity;

	Matrix m_matInCore;														//塊（コア）を含む行列
	Matrix m_objectWorldMatrix;												//巻き込み後のオブジェクトのワールド行列
	
	EnObjectState m_objectState = m_enObject_NotInvolution;
	EnMoveState m_moveState = m_enMove_No;

	IMovePtr m_objectMove;
};

