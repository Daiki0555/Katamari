#include "stdafx.h"
#include "MoveRand.h"
namespace {
	const float MOVESPEED = 15.0f;
	const float MOVETIME = 1.0f;
}
MoveRand::~MoveRand()
{

}

void MoveRand::Init(
	const Vector3& pos,
	const Quaternion& rot,
	const float speed,
	const float range
)
{
	m_position = pos;
	m_firstPosition = pos;
	m_rotation = rot;
	m_limitMoveRange = range;
	m_moveState = m_enMove_Rand;
	m_moveTime = MOVETIME;
	InitRandomPosition();
}

const Vector3& MoveRand::Move()
{

		
	if (m_moveTime <= 0.0f) {
		InitRandomPosition();
		//���Ԃ�߂�
		m_moveTime = MOVETIME;
	}
	else {
		//�I�u�W�F�N�g���烉���_���ȍ��W�ɐL�т�x�N�g�������߂�
		m_objectDiff = m_randomPosition - m_position;
		//�x�N�g���̒��������܂�ɂ��������Ȃ瓮�����Ȃ�
		if (m_objectDiff.Length()<0.0001f) {
			return m_position;
		}

		m_objectDiff.y = 0.0f;
		//���K������
		m_objectDiff.Normalize();

		Vector3 moveSpeed = m_objectDiff * MOVESPEED*g_gameTime->GetFrameDeltaTime();
		m_position += moveSpeed;
		m_moveTime -= g_gameTime->GetFrameDeltaTime();
	}

	return m_position;
}

void MoveRand::InitRandomPosition()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	//X���W
	float maxRangeX = m_firstPosition.x + m_limitMoveRange;
	float minRangeX = m_firstPosition.x - m_limitMoveRange;
	std::uniform_real_distribution<float> randomPositionX(minRangeX, maxRangeX);
	
	//Z���W
	float maxRangeZ = m_firstPosition.z + m_limitMoveRange;
	float minRangeZ = m_firstPosition.z - m_limitMoveRange;
	std::uniform_real_distribution<float> randomPositionZ(minRangeZ, maxRangeZ);

	//���W�̐���
	m_randomPosition.x = randomPositionX(gen);
	m_randomPosition.z = randomPositionZ(gen);

	m_isInitRandom = true;
}
