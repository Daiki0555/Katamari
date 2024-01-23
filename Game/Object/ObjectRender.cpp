#include "stdafx.h"
#include "ObjectRender.h"
bool ObjectRender::Start()
{
	char filepath[256];
	sprintf(filepath, "Assets/modelData/object/%s/%s.tkm", m_objData.m_name.c_str(), m_objData.m_name.c_str());
	m_modelRender.InitToonModel(
		filepath,
		0,
		0,
		enModelUpAxisZ,
		true,
		false,
		true,
		false,
		m_maxObject
	);
	return true;
}

void ObjectRender::Update()
{

}

void ObjectRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}