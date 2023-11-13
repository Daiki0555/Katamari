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
	void Init(const Vector3& pos, const Quaternion& rot, const float speed, const float range) override;


	/// <summary>
	/// �ړ�����
	/// </summary>
	const Vector3& Move() override;
public:
private:
	/// <summary>
	/// �����_���ȍ��W�̏�����
	/// </summary>
	void InitRandomPosition();
private:
	
	Vector3 m_firstPosition = Vector3::Zero;										//�ŏ��̍��W
	Vector3 m_randomPosition = Vector3::Zero;										//�����_���Ȉړ��͈�
	Vector3 m_objectDiff = Vector3::Zero;
	float m_limitMoveRange = 0.0f;													//�ړ��͈͂̌��E
	float m_moveTime = 0.0f;														//�ړ��o���鎞��
	bool m_isStop = false;															//�~�܂��Ă��邩�̔���
	bool m_isInitRandom = false;

};
