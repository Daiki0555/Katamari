#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"
namespace nsK2EngineLow {
	void SpriteRender::Init(
		const char* filePath,
		const float w,
		const float h,
		AlphaBlendMode alphaBlendMode,
		bool isCircle
	)
	{
		SpriteInitData initData;
		//DDS�t�@�C���p�X�̃t�@�C���p�X���w�肷��
		initData.m_ddsFilePath[0] = filePath;
		//Sprite�\���p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		//�X�v���C�g�̕��ƍ������w�肷��
		initData.m_width = static_cast<UINT>(w);
		initData.m_height = static_cast<UINT>(h);
		initData.m_alphaBlendMode = alphaBlendMode;
		if (isCircle){
			initData.m_psEntryPoinFunc = "PSCircleGauge";
			initData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetSpriteCB();
			initData.m_expandConstantBufferSize= sizeof(RenderingEngine::GetInstance()->GetSpriteCB());
		}
		m_sprite.Init(initData);
	}
	void SpriteRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}
}