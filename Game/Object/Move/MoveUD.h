#pragma once
#include "IMove.h"
class MoveUD :public IMove
{
public:
	~MoveUD()override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot,const float speed, const float range) override;


	/// <summary>
	/// 移動処理
	/// </summary>
	const Vector3& Move() override;
public:
private:
private:
	Vector3 m_maxPosition=Vector3::Zero;
	Vector3 m_minPosition=Vector3::Zero;
	float m_moveSpeed = 0.0f;
	bool isVector = true;
};
