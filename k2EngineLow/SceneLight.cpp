#include "k2EngineLowPreCompile.h"
#include "SceneLight.h"
namespace nsK2EngineLow
{
	SceneLight* SceneLight::m_Sinstance = nullptr;
	SceneLight::SceneLight()
	{
		m_light.directionLight.eyePos = g_camera3D->GetPosition();
	}

	SceneLight::~SceneLight()
	{

	}
	
	void SceneLight::Init()
	{
		//�f�B���N�V�������C�g�̏�����
		SetDirectionLight(
			Vector3(1,-1,1),
			Vector3(0.3f,0.3f,0.3f)
		);

		//�����̏�����
		SetAmbientLight(0.8f);

		//�������C�g�̏�����
		/*SetHemiSphereLight(
			Vector3(1.0f, 0.5f, 0.3f),
			Vector3(0.15f, 0.7f, 0.95f),
			Vector3(0.0f, 1.0f, 0.0f)
		);*/

		m_light.directionLight.eyePos = g_camera3D->GetPosition();

	}
}