/*!
* @brief	球体コライダー。
*/

#pragma once

#include "ICollider.h"

namespace nsK2EngineLow {
	class SphereCollider : public ICollider
	{
	public:
		//デストラクタ
	~SphereCollider();
		void Create(const float radius);
		btCollisionShape* GetBody() const override
		{
			return m_shape.get();
		}
		/// <summary>
		/// 半径の取得	
		/// </summary>
		/// <returns></returns>
		float GetRadius() const
		{
			return m_radius;
		}
		/// <summary>
		/// 半径を設定
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