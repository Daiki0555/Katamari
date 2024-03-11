#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"
class Sphere;
class LineSegment
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param>
	void Init(const Vector3& pos);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="linesegment"></param>
	void Execute(const Vector3& pos, const Vector3 linesegment);

	/// <summary>
	/// 塊の情報を渡す
	/// </summary>
	/// <param name="sphere"></param>
	void SetSphere(Sphere* sphere)
	{
		m_sphere = sphere;
	}
private:

private:
	RigidBody			m_rigidBody;						//剛体
	SphereCollider		m_collider;							//コライダー。
	Sphere*				m_sphere = nullptr;					//プレイヤー
	float				m_radius = 0.01f;					//半径
};

