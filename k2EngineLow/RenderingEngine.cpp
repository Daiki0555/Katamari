#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"
namespace nsK2EngineLow
{
	namespace {
		const Vector3 WIPE_CAMERA_POSITION = { 11000.0f, 50.0f, -200.0f };
		const Vector3 WIPE_CAMERA_TARGET = { 11000.0f, 20.0f, 0.0f };
		const float WIPE_CAMERA_NEAR = 150.0f;
		const float WIPE_CAMERA_FAR = 500.0f;
	}

	RenderingEngine* RenderingEngine::m_instance = nullptr;
	RenderingEngine::RenderingEngine()
	{

	}
	RenderingEngine::~RenderingEngine()
	{

	}

	void RenderingEngine::Init()
	{
		InitZPrepassRenderTarget();

		InitMainRenderTarget();

		InitGBuffer();

		InitCopyMainRenderTargetToFrameBufferSprite();

		InitShadowMapRenderTarget();

		InitDeferredLighting();

		Init2DRenderTarget();

		InitUICamera();

		m_postEffect.Init(m_mainRenderTarget);
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

	void RenderingEngine::InitZPrepassRenderTarget()
	{
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}
	
	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// アルベドカラーを出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBuffer_AlbedoDepth].Create(
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

		// 金属度と滑らかさマップ出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBuffer_MetaricShadowSmooth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM, //メモリ使用量メモリ書き込み速度優先で、8bitの符号なし整数バッファを使用する。。
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

	void RenderingEngine::InitShadowMapRenderTarget()
	{
		m_shadowMapRenders.Init();
	}

	void RenderingEngine::InitDeferredLighting()
	{
		
		// シーンライトを初期化する。
		SceneLight::GetSceneLightClass()->Init();

		// シーンライトのデータをコピー。
		m_lightingCB.m_light = SceneLight::GetSceneLightClass()->GetSceneLight();


		// ポストエフェクト的にディファードライティングを行うためのスプライトを初期化
		SpriteInitData m_deferredSpriteInitData;

		// ポストエフェクト的にディファードライティングを行うためのスプライトを初期化
		// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
		m_deferredSpriteInitData.m_width = FRAME_BUFFER_W;
		m_deferredSpriteInitData.m_height = FRAME_BUFFER_H;

		// ディファードライティングで使用するテクスチャを設定
		int texNo = 0;
		for (auto& gBuffer : m_gBuffer)
		{
			m_deferredSpriteInitData.m_textures[texNo++] = &gBuffer.GetRenderTargetTexture();
		}
		m_deferredSpriteInitData.m_fxFilePath= "Assets/shader/deferredLighting.fx";
		m_deferredSpriteInitData.m_expandConstantBuffer = &GetLightingCB();
		m_deferredSpriteInitData.m_expandConstantBufferSize = sizeof(GetLightingCB());
		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			m_deferredSpriteInitData.m_textures[texNo++] = &m_shadowMapRenders.GetShadowMap(areaNo);
		}



		m_deferredSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

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

	void RenderingEngine::InitUICamera()
	{
		//ワイプカメラを初期化
		m_wipeCamera.SetPosition(WIPE_CAMERA_POSITION);
		m_wipeCamera.SetTarget(WIPE_CAMERA_TARGET);
		m_wipeCamera.SetNear(WIPE_CAMERA_NEAR);
		m_wipeCamera.SetFar(WIPE_CAMERA_FAR);
		m_wipeCamera.Update();
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{


		RenderToShadowMap(rc);		
		
		ZPrepass(rc);

		RenderToGBuffer(rc);

		DeferredLighting(rc);

		ForwardRendering(rc);

		m_postEffect.OnRender(rc, m_mainRenderTarget);

		Render2D(rc);

		CopyMainRenderTargetToFrameBuffer(rc);
		// 登録されている描画オブジェクトをクリア
		m_renderObjects.clear();
	}

	void RenderingEngine::RenderToShadowMap(RenderContext& rc)
	{
		BeginGPUEvent("RenderToShadowMap");
		m_shadowMapRenders.Render(
			rc,
			m_lightingCB.m_light.directionLight.ligDirection,
			m_renderObjects
		);
		EndGPUEvent();
	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		BeginGPUEvent("ZPrepass");
		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& model : m_renderObjects)
		{
			model->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");
		RenderTarget* rts[] = {
			&m_gBuffer[enGBuffer_AlbedoDepth],
			&m_gBuffer[enGBuffer_Normal],
			&m_gBuffer[enGBuffer_WorldPos],
			&m_gBuffer[enGBuffer_MetaricShadowSmooth]
		};
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		rc.SetRenderTargetsAndViewport(ARRAYSIZE(rts), rts);
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRenderToGBuffer(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

		EndGPUEvent();
	}

	void RenderingEngine::DeferredLighting(RenderContext& rc)
	{

		//ライティングに必要なライト情報を更新する
		m_lightingCB.m_light = SceneLight::GetSceneLightClass()->GetSceneLight();

		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			m_lightingCB.mlvp[areaNo] = m_shadowMapRenders.GetLVPMatrix(areaNo);
		}

		BeginGPUEvent("DeferredLighting");
		// レンダリング先をメインレンダリングターゲットにする
	    // メインレンダリングターゲットを設定
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);
		
		//G-Bufferの内容を元にしてディファードライティング
		m_deferredLightInSpr.Draw(rc);

		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		EndGPUEvent();
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTarget(
			m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
			m_gBuffer[enGBuffer_AlbedoDepth].GetDSVCpuDescriptorHandle()
		);
		for (auto& forwardObj : m_renderObjects) {
			forwardObj->OnForwardRender(rc);
		}

		//トゥーンシェーダーのモデルを描画
		for (auto& toonObj : m_renderObjects) {
			toonObj->OnToonRender(rc);
		}

		for (auto& wipeModel : m_renderObjects) {
			wipeModel->OnUIModelRender(rc, m_wipeCamera);
		}
		// メインレンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
		
		EndGPUEvent();
	}


	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");

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
	
		EndGPUEvent();
	}

	void RenderingEngine::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		BeginGPUEvent("CopyMainRenderTargetToFrameBuffer");
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyMainRtToFrameBufferSprite.Draw(rc);

		EndGPUEvent();
	}
}