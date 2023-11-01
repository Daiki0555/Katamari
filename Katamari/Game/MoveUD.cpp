#include "stdafx.h"
#include "MoveUD.h"
#include "IMove.h"
MoveUD::~MoveUD()
{

}

void MoveUD::Init(
	const Vector3& pos,
	const Quaternion& rot
)
{

}

const Vector3& MoveUD::Move()
{
	return m_position;
}

