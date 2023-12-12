/*!
* @brief	���̃R���C�_�[�B
*/

#pragma once

#include "ICollider.h"

namespace nsK2EngineLow {
	class SphereCollider : public ICollider
	{
	public:
		//�f�X�g���N�^
	~SphereCollider();
		void Create(const float radius);
		btCollisionShape* GetBody() const override
		{
			return m_shape.get();
		}
		/// <summary>
		/// ���a�̎擾	
		/// </summary>
		/// <returns></returns>
		float GetRadius() const
		{
			return m_radius;
		}
		/// <summary>
		/// ���a��ݒ�
		/// </summary>
		void SetRadius(const float& radius)
		{
			m_shape->setUnscaledRadius(radius);
			m_radius = radius;
		}

	private:
		std::unique_ptr<btSphereShape>	m_shape;
		float m_radius;
	};
}