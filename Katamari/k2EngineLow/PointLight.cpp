#include "k2EngineLowPreCompile.h"
#include "PointLight.h"
namespace nsK2EngineLow {
	PointLight::~PointLight()
	{

	}

	void PointLight::Update()
	{
		m_sceneLight.SetPointLight(GetNumber(), GetPosition(), GetColor(), GetRange());
	}
}