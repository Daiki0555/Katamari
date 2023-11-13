#include "k2EngineLowPreCompile.h"
#include "Bloom.h"
namespace nsK2EngineLow {
	void Bloom::Init(
		RenderTarget& mainRenderTarget
	)
	{
		m_luminanceRenderTarget.Create(
			mainRenderTarget.GetWidth(),		//解像度はメインレンダーターゲットと同じ
			mainRenderTarget.GetHeight(),
			1,
			1,
			mainRenderTarget.GetColorBufferFormat(),
			DXGI_FORMAT_UNKNOWN
		);

		//輝度抽出用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//輝度抽出用のシェーダーのファイルパスを指定する
		spriteInitData.m_fxFilePath = "Assets/shader/PostEffect/bloom.fx";

		//頂点シェーダのエントリーポイントを指定する
		spriteInitData.m_vsEntryPointFunc = "VSMain";

		//ピクセルシェーダーのエントリーポイントを指定する
		spriteInitData.m_psEntryPoinFunc = "PSLuminance";

		//ブルームの情報をシェーダに渡す
		spriteInitData.m_expandConstantBuffer = &m_luminanceCB;
		spriteInitData.m_expandConstantBufferSize = sizeof(m_luminanceCB);

		//スプライトの幅と高さはluminaceRenderTargetと同じ
		spriteInitData.m_width = mainRenderTarget.GetWidth();
		spriteInitData.m_height = mainRenderTarget.GetHeight();

		//テクスチャはメインレンダリングターゲットのカラーバッファー
		spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();

		//書き込むレンダリングターゲットのフォーマットを指定する
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_luminanceSprite.Init(spriteInitData);

		//ガウシアンブラーを初期化
		//m_gussianBlur[0]は輝度テクスチャにガウシアンブラーを掛ける
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());

		//m_gussianBlur[1]はm_gussianBlur[0]のテクスチャにガウシアンブラーをを掛ける
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());

		//m_gussianBlur[2]はm_gussianBlur[1]のテクスチャにガウシアンブラーをを掛ける
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());

		//m_gussianBlur[3]はm_gussianBlur[2]のテクスチャにガウシアンブラーをを掛ける
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

		//ボケ画像を加算合成するスプライトを初期化
		//初期化情報を設定する
		SpriteInitData finalSpriteInitData;
		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

		//解像度はmainRenderTargetの幅と高さ
		finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
		finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

		//ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
		//2D用のシェーダーを使用する
		finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/bloom.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		//加算合成で描画するので、アルファブレンディングモードに加算する
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;

		//カラーバッファーのフォーマットは32ビット浮動小数点バッファー
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

		//初期化情報をもとに加算合成用のスプライトを初期化する
		m_finalSprite.Init(finalSpriteInitData);

		//テクスチャを貼り付けるためのスプライトを初期化する
		SpriteInitData initData;
		//テクスチャはmainRenderTargetのカラーバッファー
		initData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		initData.m_width = mainRenderTarget.GetWidth();
		initData.m_height = mainRenderTarget.GetHeight();
		//モノクロ用のシェーダーを指定する
		initData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyMainRtSprite.Init(initData);

	}

	void Bloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		BeginGPUEvent("Bloom");

		// 輝度抽出
		// 輝度抽出用のレンダリングターゲットに変更
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_luminanceRenderTarget);
		// 輝度抽出を行う
		m_luminanceSprite.Draw(rc);
		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

		//ガウシアンブラーを実行する
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);

		//ボケ画像をメインレンダリングターゲットに加算合成
	//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);

		//レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(mainRenderTarget);

		//最終合成
		m_finalSprite.Draw(rc);

		//レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

		EndGPUEvent();
	}
}