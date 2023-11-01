#include "stdafx.h"
#include "MoveRand.h"
#include "IMove.h"
MoveRand::~MoveRand()
{

}

void MoveRand::Init(
	const Vector3& pos,
	const Quaternion& rot
)
{

}

const Vector3& MoveRand::Move()
{
	return m_position;
}
