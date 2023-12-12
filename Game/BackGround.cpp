#include "stdafx.h"
#include "BackGround.h" 
BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	m_backRender.InitDeferredRendering(
		"Assets/modelData/stage/Stage.tkm",
		0,
		0, 
		enModelUpAxisZ, 
		false, 
		true
	);

	m_backRender.SetPosition(m_position);
	m_backRender.SetRotation(m_rotation);
	m_backRender.SetScale(m_scale);
	m_backRender.Update();


	m_physicsObj.CreateFromModel(
		m_backRender.GetModel(), 
		m_backRender.GetModel().GetWorldMatrix()
	);
	return true;
}

void BackGround::Update()
{

}

void BackGround::Render(RenderContext& rc)
{
	m_backRender.Draw(rc);
}