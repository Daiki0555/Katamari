#include "k2EngineLowPreCompile.h"
#include "SpotLight.h"
namespace nsK2EngineLow {
	SpotLight::~SpotLight()
	{

	}

	void SpotLight::Update()
	{
		m_sceneLight.SetSpotLight(GetNumber(), GetPosition(), GetColor(), GetRange(), GetDirection(), GetAngle());
	}
}