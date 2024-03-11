#pragma once
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"
class Sphere;
class LineSegment
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos"></param>
	void Init(const Vector3& pos);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="linesegment"></param>
	void Execute(const Vector3& pos, const Vector3 linesegment);

	/// <summary>
	/// ��̏���n��
	/// </summary>
	/// <param name="sphere"></param>
	void SetSphere(Sphere* sphere)
	{
		m_sphere = sphere;
	}
private:

private:
	RigidBody			m_rigidBody;						//����
	SphereCollider		m_collider;							//�R���C�_�[�B
	Sphere*				m_sphere = nullptr;					//�v���C���[
	float				m_radius = 0.01f;					//���a
};

