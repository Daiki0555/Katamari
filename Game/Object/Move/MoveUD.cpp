#include "stdafx.h"
#include "MoveUD.h"
namespace {
	const Vector3 LIMITPOSITION{0.0f,5.0f,0.0f};
}
MoveUD::~MoveUD()
{

}

void MoveUD::Init(
	const Vector3& pos,
	const Quaternion& rot,
	const float speed,
	const float range
)
{
	m_position = pos;
	m_rotation = rot;
	m_moveSpeed = speed;
	m_maxPosition = m_position + LIMITPOSITION;
	m_minPosition = m_position - LIMITPOSITION;
	m_moveState = m_enMove_UD;
}

const Vector3& MoveUD::Move()
{
	if (m_maxPosition.y == m_position.y) {
		isVector = false;
	}
	else if (m_minPosition.y == m_position.y) {
		isVector = true;
	}

	if (isVector == true) {
		m_position.y += m_moveSpeed*g_gameTime->GetFrameDeltaTime();
		m_position.y = min(m_position.y, m_maxPosition.y);
	}
	else {
		m_position.y -= m_moveSpeed*g_gameTime->GetFrameDeltaTime();
		m_position.y = max(m_position.y, m_minPosition.y);
	}
	return m_position;
}

