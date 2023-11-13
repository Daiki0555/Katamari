#include "k2EngineLowPreCompile.h"
#include "PointLight.h"
namespace nsK2EngineLow {
	PointLight::~PointLight()
	{

	}

	void PointLight::Update()
	{
		SceneLight::GetSceneLightClass()->SetPointLight(GetNumber(), GetPosition(), GetColor(), GetRange());
	}
}