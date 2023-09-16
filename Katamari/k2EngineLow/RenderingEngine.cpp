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

	
	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// �A���x�h�J���[���o�͗p�̃����_�����O�^�[�Q�b�g������������
		m_gBuffer[enGBuffer_Albedo].Create(
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

	void RenderingEngine::InitDeferredLighting()
	{
		// �|�X�g�G�t�F�N�g�I�Ƀf�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
		// ��ʑS�̂Ƀ����_�����O����̂ŕ��ƍ����̓t���[���o�b�t�@�[�̕��ƍ����Ɠ���
		m_deferredSpriteInitData.m_width = FRAME_BUFFER_W;
		m_deferredSpriteInitData.m_height = FRAME_BUFFER_H;

		// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
		m_deferredSpriteInitData.m_textures[0] = &m_gBuffer[enGBuffer_Albedo].GetRenderTargetTexture();
		m_deferredSpriteInitData.m_textures[1] = &m_gBuffer[enGBuffer_Normal].GetRenderTargetTexture();
		m_deferredSpriteInitData.m_textures[2] = &m_gBuffer[enGBuffer_WorldPos].GetRenderTargetTexture();
		m_deferredSpriteInitData.m_fxFilePath= "Assets/shader/deferredLighting.fx";
		
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
		spriteInitData.m_psEntryPoinFunc = "PSMain";
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


	void RenderingEngine::Execute(RenderContext& rc)
	{
		RenderToGBuffer(rc);
		DeferredLighting(rc);
		Render2D(rc);
		CopyMainRenderTargetToFrameBuffer(rc);
		// �o�^����Ă���`��I�u�W�F�N�g���N���A
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
		// �����_�����O������C�������_�����O�^�[�Q�b�g�ɂ���
	    // ���C�������_�����O�^�[�Q�b�g��ݒ�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget);
		//G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
		m_deferredLightInSpr.Draw(rc);

		// ���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		
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