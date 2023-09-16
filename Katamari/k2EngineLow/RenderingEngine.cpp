#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"
namespace nsK2EngineLow
{
	RenderingEngine* RenderingEngine::m_instance = nullptr;
	RenderingEngine::RenderingEngine()
	{

	}
	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init()
	{
		InitMainRenderTarget();

		InitGBuffer();

		InitCopyMainRenderTargetToFrameBufferSprite();

		InitDeferredLighting();

		Init2DRenderTarget();
	}

	void RenderingEngine::InitMainRenderTarget()
	{

		//メインレンダーターゲット
		float clearColor[4] = { 0.5f,0.5f,0.5f,1.0f };
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);
	}

	
	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// アルベドカラーを出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBuffer_Albedo].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
		
		// 法線出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBuffer_Normal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		// ワールド座標出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBuffer_WorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		// テクスチャはyBlurRenderTargetのカラーバッファー
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// レンダリング先がフレームバッファーなので、解像度はフレームバッファーと同じ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 初期化オブジェクトを使って、スプライトを初期化する
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitDeferredLighting()
	{
		// ポストエフェクト的にディファードライティングを行うためのスプライトを初期化
		// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
		m_deferredSpriteInitData.m_width = FRAME_BUFFER_W;
		m_deferredSpriteInitData.m_height = FRAME_BUFFER_H;

		// ディファードライティングで使用するテクスチャを設定
		m_deferredSpriteInitData.m_textures[0] = &m_gBuffer[enGBuffer_Albedo].GetRenderTargetTexture();
		m_deferredSpriteInitData.m_textures[1] = &m_gBuffer[enGBuffer_Normal].GetRenderTargetTexture();
		m_deferredSpriteInitData.m_textures[2] = &m_gBuffer[enGBuffer_WorldPos].GetRenderTargetTexture();
		m_deferredSpriteInitData.m_fxFilePath= "Assets/shader/deferredLighting.fx";
		
		// 初期化データを使ってスプライトを作成
		m_deferredLightInSpr.Init(m_deferredSpriteInitData);
	}
	
	void RenderingEngine::Init2DRenderTarget()
	{
		float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		m_2DRenderTarget.Create(
			UI_SPACE_WIDTH,
			UI_SPACE_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			clearColor
		);

		// 最終合成用のスプライトを初期化する
		SpriteInitData spriteInitData;
		//テクスチャは2Dレンダ―ターゲット。
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// 解像度はmainRenderTargetの幅と高さ
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D用のシェーダーを使用する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		//上書き。
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		//テクスチャはメインレンダ―ターゲット。
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		//解像度は2Dレンダ―ターゲットの幅と高さ
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//レンダリングターゲットのフォーマット。
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}


	void RenderingEngine::Execute(RenderContext& rc)
	{
		RenderToGBuffer(rc);
		DeferredLighting(rc);
		Render2D(rc);
		CopyMainRenderTargetToFrameBuffer(rc);
		// 登録されている描画オブジェクトをクリア
		m_renderObjects.clear();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		RenderTarget* rts[] = {
			&m_gBuffer[enGBuffer_Albedo],
			&m_gBuffer[enGBuffer_Normal],
			&m_gBuffer[enGBuffer_WorldPos]
		};
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		rc.SetRenderTargetsAndViewport(ARRAYSIZE(rts), rts);
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderToGBuffer(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{
		// レンダリング先をメインレンダリングターゲットにする
	    // メインレンダリングターゲットを設定
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);
		//G-Bufferの内容を元にしてディファードライティング
		m_deferredLightInSpr.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		
		//PRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}

		//RENDERTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);
		//PRESENTからRENDERTARGETへ。
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		m_2DSprite.Draw(rc);
		//RENDERTARGETからPRESENTへ。
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	
		
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}
}