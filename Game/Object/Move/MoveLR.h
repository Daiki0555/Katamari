#pragma once
#include "IMove.h"
class MoveLR :public IMove
{
public:
	~MoveLR()override;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot,const float speed, const float range) override;


	/// <summary>
	/// �ړ�����
	/// </summary>
	const Vector3& Move() override;
public:
private:
private:
};
