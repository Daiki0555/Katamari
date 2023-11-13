#include "k2EngineLowPreCompile.h"
#include "CollisionObject.h"
namespace nsK2EngineLow {

	CollisionObjectManager* CollisionObjectManager::m_instance = nullptr;

	CollisionObject::CollisionObject()
	{
	}

	CollisionObject::~CollisionObject()
	{
		CollisionObjectManager::GetInstance()->RemoveCollisionObject(this);
	}

	bool CollisionObject::Start()
	{
		CollisionObjectManager::GetInstance()->AddCollisionObject(this);
		return true;
	}

	void CollisionObject::Update()
	{
		if (m_isEnableAutoDelete == false)
		{
			return;
		}


		if (m_time > m_timeLimit + 0.1f)
		{
			DeleteGO(this);
		}
		m_time += g_gameTime->GetFrameDeltaTime();
	}


	CollisionObjectManager::CollisionObjectManager()
	{
	}

	CollisionObjectManager::~CollisionObjectManager()
	{
		m_collisionObjectVector.clear();
		m_findsCollisionObjectVector.clear();
		m_findMatchForwardNameCollisionObjectVector.clear();
	}
}