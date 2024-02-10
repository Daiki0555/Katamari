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

		//���C�������_�[�^�[�Q�b�g
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

		// �A���x�h�J���[���o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_gBuffer[enGBuffer_AlbedoDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
		
		// �@���o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_gBuffer[enGBuffer_Normal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN
		);

		// ���[���h���W�o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_gBuffer[enGBuffer_WorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);

		// �����x�Ɗ��炩���}�b�v�o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_gBuffer[enGBuffer_MetaricShadowSmooth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM, //�������g�p�ʃ������������ݑ��x�D��ŁA8bit�̕����Ȃ������o�b�t�@���g�p����B�B
			DXGI_FORMAT_UNKNOWN
		);
	}

	void RenderingEngine::InitCopyMainRenderTargetToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		// �e�N�X�`����yBlurRenderTarget�̃J���[�o�b�t�@�[
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		// �����_�����O�悪�t���[���o�b�t�@�[�Ȃ̂ŁA�𑜓x�̓t���[���o�b�t�@�[�Ɠ���
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_psEntryPoinFunc = "PSMain";
		spriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �������I�u�W�F�N�g���g���āA�X�v���C�g������������
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitShadowMapRenderTarget()
	{
		m_shadowMapRenders.Init();
	}

	void RenderingEngine::InitDeferredLighting()
	{
		
		// �V�[�����C�g������������B
		SceneLight::GetSceneLightClass()->Init();

		// �V�[�����C�g�̃f�[�^���R�s�[�B
		m_lightingCB.m_light = SceneLight::GetSceneLightClass()->GetSceneLight();


		// �|�X�g�G�t�F�N�g�I�Ƀf�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
		SpriteInitData m_deferredSpriteInitData;

		// �|�X�g�G�t�F�N�g�I�Ƀf�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
		// ��ʑS�̂Ƀ����_�����O����̂ŕ��ƍ����̓t���[���o�b�t�@�[�̕��ƍ����Ɠ���
		m_deferredSpriteInitData.m_width = FRAME_BUFFER_W;
		m_deferredSpriteInitData.m_height = FRAME_BUFFER_H;

		// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
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

		// �������f�[�^���g���ăX�v���C�g���쐬
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

		// �ŏI�����p�̃X�v���C�g������������
		SpriteInitData spriteInitData;
		//�e�N�X�`����2D�����_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_2DRenderTarget.GetRenderTargetTexture();
		// �𑜓x��mainRenderTarget�̕��ƍ���
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();
		// 2D�p�̃V�F�[�_�[���g�p����
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_vsEntryPointFunc = "VSMain";
		//�㏑���B
		spriteInitData.m_alphaBlendMode = AlphaBlendMode_None;
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_mainRenderTarget.GetColorBufferFormat();

		m_2DSprite.Init(spriteInitData);

		//�e�N�X�`���̓��C�������_�\�^�[�Q�b�g�B
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();

		//�𑜓x��2D�����_�\�^�[�Q�b�g�̕��ƍ���
		spriteInitData.m_width = m_2DRenderTarget.GetWidth();
		spriteInitData.m_height = m_2DRenderTarget.GetHeight();
		//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
		spriteInitData.m_colorBufferFormat[0] = m_2DRenderTarget.GetColorBufferFormat();

		m_mainSprite.Init(spriteInitData);
	}

	void RenderingEngine::InitUICamera()
	{
		//���C�v�J������������
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
		// �o�^����Ă���`��I�u�W�F�N�g���N���A
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
		// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
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

		//���C�e�B���O�ɕK�v�ȃ��C�g�����X�V����
		m_lightingCB.m_light = SceneLight::GetSceneLightClass()->GetSceneLight();

		for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
		{
			m_lightingCB.mlvp[areaNo] = m_shadowMapRenders.GetLVPMatrix(areaNo);
		}

		BeginGPUEvent("DeferredLighting");
		// �����_�����O������C�������_�����O�^�[�Q�b�g�ɂ���
	    // ���C�������_�����O�^�[�Q�b�g��ݒ�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);
		
		//G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
		m_deferredLightInSpr.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
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

		//�g�D�[���V�F�[�_�[�̃��f����`��
		for (auto& toonObj : m_renderObjects) {
			toonObj->OnToonRender(rc);
		}

		for (auto& wipeModel : m_renderObjects) {
			wipeModel->OnUIModelRender(rc, m_wipeCamera);
		}
		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
		
		EndGPUEvent();
	}


	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");

		//PRESENT����RENDERTARGET�ցB
		rc.WaitUntilToPossibleSetRenderTarget(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_2DRenderTarget);

		// �����_�����O�^�[�Q�b�g���N���A
		rc.ClearRenderTargetView(m_2DRenderTarget);

		m_mainSprite.Draw(rc);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnRender2D(rc);
		}

		//RENDERTARGET����PRESENT�ցB
		rc.WaitUntilFinishDrawingToRenderTarget(m_2DRenderTarget);
		//PRESENT����RENDERTARGET�ցB
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		// �����_�����O�^�[�Q�b�g��ݒ�
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		m_2DSprite.Draw(rc);
		//RENDERTARGET����PRESENT�ցB
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