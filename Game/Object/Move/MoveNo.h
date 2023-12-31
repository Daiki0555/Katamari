#pragma once
#include "IMove.h"
class MoveNo :public IMove
{
public:
	~MoveNo()override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot, const float speed, const float range) override;


	/// <summary>
	/// 移動処理
	/// </summary>
	const Vector3& Move() override;
private:
private:
};

