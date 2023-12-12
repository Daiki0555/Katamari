#include "stdafx.h"
#include "MoveNo.h"
MoveNo::~MoveNo()
{

}

void MoveNo::Init(
	const Vector3& pos,
	const Quaternion& rot,
	const float speed,
	const float range
)
{
	m_position = pos;
	m_moveState = m_enMove_FB;
}

const Vector3& MoveNo::Move()
{
	return m_position;
}
