#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"
namespace nsK2EngineLow {
	void ShadowMapRender::Init()
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

		//�ߌi�p�̃V���h�E�}�b�v
		m_shadowMaps[0].Create(
			4096,
			4096,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//���i�p�̃V���h�E�}�b�v
		m_shadowMaps[1].Create(
			2048,
			2048,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		//���i�p�̃V���h�E�}�b�v
		m_shadowMaps[2].Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void ShadowMapRender::Render(
		RenderContext& rc, 
		Vector3& lightDirection, 
		std::vector< IRenderer* >& renderObjects
	)
	{
		if (lightDirection.LengthSq() < 0.001f) {
			return;
		}
		m_cascadeShadowMapMatrix.CalcLightViewProjectionCropMatrix(lightDirection);

		int shadowMapNo = 0;
		for (auto& shadowMap : m_shadowMaps) {
			rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
			rc.SetRenderTargetAndViewport(shadowMap);
			rc.ClearRenderTargetView(shadowMap);

			for (auto& model : renderObjects) {
				model->OnRenderShadowMap(
					rc,
					shadowMapNo,
					m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(shadowMapNo)
				);
			}
			//�`�悪�I�������N���A
			//m_modelsArray.clear();

			 // �������݊����҂�
			rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
			shadowMapNo++;
		}
	}
}
