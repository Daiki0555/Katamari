#pragma once
#include "IMove.h"
class MoveNo :public IMove
{
public:
	~MoveNo()override;

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot, const float speed, const float range) override;


	/// <summary>
	/// ˆÚ“®ˆ—
	/// </summary>
	const Vector3& Move() override;
private:
private:
};

