#include "stdafx.h"
#include "Object.h"
#include "Sphere.h"
namespace {
	const float		HIT_OBJECT = 60.0f;					// ‰ò‚É“–‚½‚é”ÍˆÍ
}
Object::~Object()
{

}

bool Object::Start()
{
	m_sphere = FindGO<Sphere>("sphere");

	InitObject();
	InitCollision();
	return true;
}

void Object::InitObject()
{
	m_objectRender.InitDeferredRendering("Assets/modelData/object/donut.tkm", 0, 0, enModelUpAxisY, true, false);

	m_objectRender.SetPosition(m_position);
	m_objectRender.SetRotation(m_rotation);
	m_objectRender.SetScale(m_scale);
	m_objectRender.Update();
}

void Object::InitCollision()
{

}

void Object::Update()
{
	Hit();
}

void Object::Hit()
{
	Vector3 diff = m_sphere->GetPosition() - m_position;
	if (diff.Length() <= HIT_OBJECT)
	{
		Involution();
	}
}

void Object::Involution()
{

}

void Object::Render(RenderContext& rc)
{
	m_objectRender.Draw(rc);
}