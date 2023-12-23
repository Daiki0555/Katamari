#include "stdafx.h"
#include "StageCollider.h"
StageCollider::~StageCollider()
{
	
}

bool StageCollider::Start()
{
	m_modelRender.InitDeferredRendering(
		"Assets/modelData/stage/collider.tkm",
		0,
		0,
		enModelUpAxisZ
		);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.Update();

	m_physicsObject.CreateFromModel(
		m_modelRender.GetModel(),
		m_modelRender.GetModel().GetWorldMatrix(),
		0.0f
	);
	return true;
}