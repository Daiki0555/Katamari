#include "stdafx.h"
#include "MoveFB.h"

MoveFB::~MoveFB()
{

}

void MoveFB::Init(
	const Vector3& pos,
	const Quaternion& rot,
	const float speed,
	const float range
)
{
	m_moveState = m_enMove_FB;
}

const Vector3& MoveFB::Move()
{
	return m_position;
}
