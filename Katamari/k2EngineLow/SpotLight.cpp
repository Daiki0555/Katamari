#include "k2EngineLowPreCompile.h"
#include "SpotLight.h"
namespace nsK2EngineLow {
	SpotLight::~SpotLight()
	{

	}

	void SpotLight::Update()
	{
		SceneLight::GetSceneLightClass()->SetSpotLight(GetNumber(), GetPosition(), GetColor(), GetRange(), GetDirection(), GetAngle());
	}
}