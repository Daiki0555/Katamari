#pragma once
#include "IMove.h"
class MoveRand :public IMove
{
public:
	~MoveRand()override;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot) override;


	/// <summary>
	/// �ړ�����
	/// </summary>
	const Vector3& Move() override;
public:
private:
private:
};
