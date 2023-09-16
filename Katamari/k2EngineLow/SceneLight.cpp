#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"
namespace nsK2EngineLow
{
	SceneLight::SceneLight()
	{
		m_light.directionLight.eyePos = g_camera3D->GetPosition();
	}

	SceneLight::~SceneLight()
	{

	}
}