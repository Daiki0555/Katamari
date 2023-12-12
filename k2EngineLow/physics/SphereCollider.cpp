/*!
* @brief	球体コライダー。
*/

#include "k2EngineLowPreCompile.h"
#include "Physics.h"
#include "SphereCollider.h"

namespace nsK2EngineLow {

	SphereCollider::~SphereCollider()
	{
		delete &m_shape;
	}
	void SphereCollider::Create(const float radius)
	{
		m_shape = std::make_unique<btSphereShape>(radius);
		this->m_radius = radius;
	}
}
