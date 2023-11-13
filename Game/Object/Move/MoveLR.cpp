#include "stdafx.h"
#include "MoveLR.h"
MoveLR::~MoveLR()
{

}

void MoveLR::Init(
	const Vector3& pos,
	const Quaternion& rot,
	const float speed,
	const float range
)
{
	m_moveState = m_enMove_LR;
}

const Vector3& MoveLR::Move()
{
	return m_position;
}
