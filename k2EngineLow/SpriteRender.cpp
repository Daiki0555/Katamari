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
		//DDSファイルパスのファイルパスを指定する
		initData.m_ddsFilePath[0] = filePath;
		//Sprite表示用のシェーダーのファイルパスを指定する。
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		//スプライトの幅と高さを指定する
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