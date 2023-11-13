#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()	
	{

	}
	ModelRender::~ModelRender()	
	{

	}
	void ModelRender::InitDeferredRendering(
		const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		const bool isShadow,
		const bool isShadowReceiver
	)	
	{
		//�X�P���g���̏�����
		InitSkeleton(filePath);

		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//���f���̏�����
		InitDeferredModel(filePath, enModelUpAxis, isShadow, isShadowReceiver);

		//InitZPrepassModel(filePath,enModelUpAxis);

		//�e�탏�[���h�s����X�V����
		UpdateWorldMatrixInModes();
	}

	void ModelRender::InitToonModel(
		const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		const bool isShadow,
		const bool isShadowReceiver,
		const bool isOutline 
	)
	{
		//�X�P���g���̏�����
		InitSkeleton(filePath);

		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//���f���̏�����
		InitModelOnToon(filePath, enModelUpAxis, isShadow, isShadowReceiver,isOutline);

		//�e�탏�[���h�s����X�V����
		UpdateWorldMatrixInModes();
	}


	void ModelRender::InitForwardRendering(ModelInitData& initData)
	{
		//InitSkeleton(initData.m_tkmFilePath);

		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//�쐬�����������f�[�^�����ƂɃ��f��������������B
		m_forwardRenderModel.Init(initData);
	}


	void ModelRender::InitSkeleton(const char* filePath)
	{
		//�X�P���g���f�[�^�̓ǂݍ���
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}
	

	void ModelRender::InitAnimation(
		AnimationClip* animationClips, 
		int numAnimationClips, 
		EnModelUpAxis enModelUpAxis
	)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr)		{
			m_animation.Init(
				m_skeleton,
				m_animationClips,
				numAnimationClips
			);
		}
	}

	void ModelRender::InitDeferredModel(
		const char* tkmFilePath, 
		EnModelUpAxis modelUpAxis, 
		const bool isShadow, 
		const bool isShadowReceiver
	)
	{
		//�ʏ탂�f���̏�����
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBufferFor3DModel.fx";
		

		if (isShadowReceiver) {
			modelInitData.m_psEntryPointFunc = "PSMainShadow";
		}
		else {
			modelInitData.m_psEntryPointFunc = "PSMain";
		}

		//�A�j���[�V����������Ȃ�
		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[3] = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_renderToGBufferModel.Init(modelInitData);
		if (isShadow) {
			//�V���h�E�p�̃��f���̏�����
			InitShadowModel(tkmFilePath, modelUpAxis);
		}
	}

	void ModelRender::InitModelOnToon(
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis,
		const bool isShadow,
		const bool isShadowReceiver,
		const bool isOutline
	)
	{
		//�ʏ탂�f���̏�����
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_expandConstantBuffer = &RenderingEngine::GetInstance()->GetLightingCB();
		modelInitData.m_expandConstantBufferSize = sizeof(RenderingEngine::GetInstance()->GetLightingCB());
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";


		if (isShadowReceiver) {
			modelInitData.m_psEntryPointFunc = "PSMainShadow";
		}
		else {
			modelInitData.m_psEntryPointFunc = "PSMain";
		}

		//�A�j���[�V����������Ȃ�
		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		//�֊s����`�悷��Ȃ�
		if (isOutline)
		{
			InitZPrepassModel(tkmFilePath, modelUpAxis);
			modelInitData.m_expandShaderResoruceView[0] = &RenderingEngine::GetInstance()->GetZPrepassDepthTexture();
		}
		else
		{
			modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
			

		m_toonModel.Init(modelInitData);
		if (isShadow) {
			//�V���h�E�p�̃��f���̏�����
			InitShadowModel(tkmFilePath, modelUpAxis);
		}
	}


	void ModelRender::InitZPrepassModel(
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis
	)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		//�A�j���[�V����������Ȃ�
		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		m_zprepassModel.Init(modelInitData);

	}

	void ModelRender::InitShadowModel(
		const char* tkmFilePath, 
		EnModelUpAxis modelUpAxis
	)
	{
		ModelInitData shadowModelInitData;
		shadowModelInitData.m_fxFilePath= "Assets/shader/shadowMap.fx";
		shadowModelInitData.m_tkmFilePath = tkmFilePath;
		shadowModelInitData.m_modelUpAxis = modelUpAxis;
		shadowModelInitData.m_colorBufferFormat[0]= DXGI_FORMAT_R32_FLOAT;
		if (m_skeleton.IsInited()) {
			//�X�P���g�����w�肷��
			shadowModelInitData.m_skeleton = &m_skeleton;
			shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP; shadowMapNo++) {
			m_shadowModels[shadowMapNo].Init(shadowModelInitData);
		}
		
	}

	void ModelRender::Update()
	{
		//���f���̃A�b�v�f�[�g
		UpdateWorldMatrixInModes();
		

		UpdateModelSkeletons();
		

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		
	}

	void ModelRender::InvolutionModelsUpdate(
		Matrix matrix,
		EnModelUpAxis modelUpAxis
	)
	{
		SetWorldMatrixInModes(matrix);
		
		UpdateModelSkeletons();
	}

	void ModelRender::SetWorldMatrixInModes(Matrix matrix)
	{
		m_zprepassModel.SetWorldMatrix(matrix);
		//�f�B�t�@�[�h�����_�����O�p�̃��f���̍X�V����
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.SetWorldMatrix(matrix);
		}

		//�t�H���[�h�����_�����O�p�̃��f���̍X�V����
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.SetWorldMatrix(matrix);
		}

		//�g�D�[���V�F�[�_�[�p�̃��f���̍X�V����
		if (m_toonModel.IsInited()) {
			m_toonModel.SetWorldMatrix(matrix);
		}

		for (auto& models : m_shadowModels) {
			if (models.IsInited())
			{
				models.SetWorldMatrix(matrix);
			}

		}
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//�f�B�t�@�[�h�����_�����O�p�̃��f���̍X�V����
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

		//�t�H���[�h�����_�����O�p�̃��f���̍X�V����
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

		//�g�D�[���V�F�[�_�[�p�̃��f���̍X�V����
		if (m_toonModel.IsInited()) {
			m_toonModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

		for (auto& models : m_shadowModels) {
			if (models.IsInited())
			{
				models.UpdateWorldMatrix(m_position, m_rotation, m_scale);
			}
			
		}
	}

	void ModelRender::UpdateModelSkeletons()
	{
		//�A�j���[�V�������Đ�����Ă���Ȃ�

		if (m_skeleton.IsInited()) {
			//�f�B�t�@�[�h�����_�����O�p�̃X�P���g���̂̍X�V����
			if (m_renderToGBufferModel.IsInited())
			{
				m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
			}
			//�t�H���[�h�����_�����O�p�̃X�P���g���̍X�V����
			if (m_forwardRenderModel.IsInited()) {
				m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
			}

			//�g�D�[���V�F�[�_�[�p�̃X�P���g���̂̍X�V����
			if (m_toonModel.IsInited()) {
				m_skeleton.Update(m_toonModel.GetWorldMatrix());
			}

			if (m_zprepassModel.IsInited()){
				m_skeleton.Update(m_zprepassModel.GetWorldMatrix());
			}

		}
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}

	void ModelRender::OnRenderShadowMap(
		RenderContext& rc,
		int shadowMapNo,
		const Matrix& lvpMatrix)
	{
		if (m_shadowModels[shadowMapNo].IsInited()) {
			m_shadowModels[shadowMapNo].Draw(
				rc,
				g_matIdentity,
				lvpMatrix,
				1
			);
		}
	}
}

