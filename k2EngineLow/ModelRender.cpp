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
		const bool isShadowReceiver,
		const bool isWipeModel
	)	
	{
		//�X�P���g���̏�����
		InitSkeleton(filePath);

		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//���f���̏�����
		InitDeferredModel(filePath, enModelUpAxis, isShadow, isShadowReceiver,isWipeModel);

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
		const bool isOutline,
		const bool isWipeModel,
		int maxInstance
	)
	{
		//�C���X�^���V���O�`��̃f�[�^��������
		InitInstancingDraw(maxInstance);

		//�X�P���g���̏�����
		InitSkeleton(filePath);

		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//���f���̏�����
		InitModelOnToon(filePath, enModelUpAxis, isShadow, isShadowReceiver,isOutline, isWipeModel);

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
		const bool isShadowReceiver,
		const bool isWipeModel
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

		if (isWipeModel) {
			//���C�v�p���f���̏�����
			m_wipeModel.Init(modelInitData);
		}
	}

	void ModelRender::InitModelOnToon(
		const char* tkmFilePath,
		EnModelUpAxis modelUpAxis,
		const bool isShadow,
		const bool isShadowReceiver,
		const bool isOutline,
		const bool isWipeModel
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

		if (m_isEnableInstancingDraw) {
			//�C���X�^���V���O�`����s���ꍇ�́A�g��SRV�ɃC���X�^���V���O�`��p�̃f�[�^��ݒ肷��B
			modelInitData.m_expandShaderResoruceView[1] = &m_worldMatrixArraySB;
			modelInitData.m_vsEntryPointFunc = "VSMainInstancing";
		}
			

		m_toonModel.Init(modelInitData);
		if (isShadow) {
			//�V���h�E�p�̃��f���̏�����
			InitShadowModel(tkmFilePath, modelUpAxis);
		}

		if (isWipeModel) {
			//���C�v�p���f���̏�����
			m_wipeModel.Init(modelInitData);
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
		if (m_isEnableInstancingDraw) {
			//�C���X�^���V���O�`����s���ꍇ�́A�g��SRV�ɃC���X�^���V���O�`��p�̃f�[�^��ݒ肷��B
			modelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
			modelInitData.m_vsEntryPointFunc = "VSMainInstancing";
		}
		
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
		if (m_isEnableInstancingDraw) {
			//�C���X�^���V���O�`����s���ꍇ�́A�g��SRV�ɃC���X�^���V���O�`��p�̃f�[�^��ݒ肷��B
			shadowModelInitData.m_expandShaderResoruceView[0] = &m_worldMatrixArraySB;
			shadowModelInitData.m_vsEntryPointFunc = "VSMainInstancing";
		}

		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP; shadowMapNo++) {
			m_shadowModels[shadowMapNo].Init(shadowModelInitData);
		}
		
	}

	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;
		if (m_maxInstance > 1) {
			// �C���X�^���V���O�`����s���̂ŁA
			// ����p�̃f�[�^���\�z����B
			// ���[���h�s��̔z��̃��������m�ۂ���B
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
			// ���[���h�s���GPU�ɓ]�����邽�߂̃X�g���N�`���[�h�o�b�t�@���m�ہB
			m_worldMatrixArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr
			);

			m_isEnableInstancingDraw = true;

			// �C���X�^���X�ԍ����烏�[���h�s��̔z��̃C���f�b�N�X�ɕϊ�����e�[�u��������������B
			m_instanceNoToWorldMatrixArrayIndexTable = std::make_unique<int[]>(m_maxInstance);
			for (int instanceNo = 0; instanceNo < m_maxInstance; instanceNo++) {
				m_instanceNoToWorldMatrixArrayIndexTable[instanceNo] = instanceNo;
			}
		}
	}


	void ModelRender::Update()
	{
		if (m_isEnableInstancingDraw) {
			return;
		}

		//���f���̃A�b�v�f�[�g
		UpdateWorldMatrixInModes();
		

		UpdateModelSkeletons();
		

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		
	}

	void ModelRender::UpdateInstancingData(
		int instanceNo,
		Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale
	)
	{
		K2_ASSERT(instanceNo < m_maxInstance, "�C���X�^���X�ԍ����s���ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		Matrix worldMatrix;
		if (m_toonModel.IsInited()) {
			worldMatrix = m_toonModel.CalcWorldMatrix(pos, rot, scale);
		}
		else {
			worldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);
		}
		// �C���X�^���X�ԍ�����s��̃C���f�b�N�X���擾����
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::InvolutionModelsUpdate(
		int instanceNo,
		Matrix matrix,
		EnModelUpAxis modelUpAxis
	)
	{
		SetInstanceWorldMatrixInModes(instanceNo,matrix);
		
		UpdateModelSkeletons();
	}

	void ModelRender::InvolutionTitleModelsUpdate(
		Matrix matrix
	)
	{
		SetWorldMatrixInModes(matrix);
		UpdateModelSkeletons();
	}

	void ModelRender::SetInstanceWorldMatrixInModes(
		int instanceNo,
		Matrix matrix
	)
	{
		K2_ASSERT(instanceNo < m_maxInstance, "�C���X�^���X�ԍ����s���ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		Matrix worldMatrix;
		worldMatrix = matrix;
		// �C���X�^���X�ԍ�����s��̃C���f�b�N�X���擾����
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::SetWorldMatrixInModes(Matrix matrix)
	{
		m_zprepassModel.SetWorldMatrix(matrix);
		//�f�B�t�@�[�h�����_�����O�p�̃��f���̍X�V����
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.SetWorldMatrix(matrix);;
		}

		//�t�H���[�h�����_�����O�p�̃��f���̍X�V����
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.SetWorldMatrix(matrix);;
		}

		//�g�D�[���V�F�[�_�[�p�̃��f���̍X�V����
		if (m_toonModel.IsInited()) {
			m_toonModel.SetWorldMatrix(matrix);;
		}

		if (m_wipeModel.IsInited()) {
			m_wipeModel.SetWorldMatrix(matrix);;
		}

		for (auto& models : m_shadowModels) {
			if (models.IsInited())
			{
				models.SetWorldMatrix(matrix);;
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

		if (m_wipeModel.IsInited()) {
			m_wipeModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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
		if (m_isEnableInstancingDraw) {
			RenderingEngine::GetInstance()->AddRenderObject(this);
			m_worldMatrixArraySB.Update(m_worldMatrixArray.get());
			m_numInstance = 0;
		}
		else {
			RenderingEngine::GetInstance()->AddRenderObject(this);
		}
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
				m_maxInstance
			);
		}
	}
}

