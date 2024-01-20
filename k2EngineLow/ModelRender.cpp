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
		//スケルトンの初期化
		InitSkeleton(filePath);

		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//モデルの初期化
		InitDeferredModel(filePath, enModelUpAxis, isShadow, isShadowReceiver,isWipeModel);

		//各種ワールド行列を更新する
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
		//インスタンシング描画のデータを初期化
		InitInstancingDraw(maxInstance);

		//スケルトンの初期化
		InitSkeleton(filePath);

		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//モデルの初期化
		InitModelOnToon(filePath, enModelUpAxis, isShadow, isShadowReceiver,isOutline, isWipeModel);

		//各種ワールド行列を更新する
		UpdateWorldMatrixInModes();
	}


	void ModelRender::InitForwardRendering(ModelInitData& initData)
	{
		//InitSkeleton(initData.m_tkmFilePath);

		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//作成した初期化データをもとにモデルを初期化する。
		m_forwardRenderModel.Init(initData);
	}


	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンデータの読み込み
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
		//通常モデルの初期化
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

		//アニメーションがあるなら
		if (m_skeleton.IsInited()) {
			//スケルトンを指定する
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[3] = DXGI_FORMAT_R8G8B8A8_UNORM;

		m_renderToGBufferModel.Init(modelInitData);
		if (isShadow) {
			//シャドウ用のモデルの初期化
			InitShadowModel(tkmFilePath, modelUpAxis);
		}

		if (isWipeModel) {
			//ワイプ用モデルの初期化
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
		//通常モデルの初期化
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

		//アニメーションがあるなら
		if (m_skeleton.IsInited()) {
			//スケルトンを指定する
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}

		//輪郭線を描画するなら
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
			//インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
			modelInitData.m_expandShaderResoruceView[1] = &m_worldMatrixArraySB;
			modelInitData.m_vsEntryPointFunc = "VSMainInstancing";
		}
			

		m_toonModel.Init(modelInitData);
		if (isShadow) {
			//シャドウ用のモデルの初期化
			InitShadowModel(tkmFilePath, modelUpAxis);
		}

		if (isWipeModel) {
			//ワイプ用モデルの初期化
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

		//アニメーションがあるなら
		if (m_skeleton.IsInited()) {
			//スケルトンを指定する
			modelInitData.m_skeleton = &m_skeleton;
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		if (m_isEnableInstancingDraw) {
			//インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
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
			//スケルトンを指定する
			shadowModelInitData.m_skeleton = &m_skeleton;
			shadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		if (m_isEnableInstancingDraw) {
			//インスタンシング描画を行う場合は、拡張SRVにインスタンシング描画用のデータを設定する。
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
			// インスタンシング描画を行うので、
			// それ用のデータを構築する。
			// ワールド行列の配列のメモリを確保する。
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
			// ワールド行列をGPUに転送するためのストラクチャードバッファを確保。
			m_worldMatrixArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr
			);

			m_isEnableInstancingDraw = true;

			// インスタンス番号からワールド行列の配列のインデックスに変換するテーブルを初期化する。
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

		//モデルのアップデート
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
		K2_ASSERT(instanceNo < m_maxInstance, "インスタンス番号が不正です。");
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
		// インスタンス番号から行列のインデックスを取得する
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// インスタンシング描画を行う。
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//インスタンス数が0の場合のみアニメーション関係の更新を行う。
			// スケルトンを更新。
			// 各インスタンスのワールド空間への変換は、
			// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
			// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
			m_skeleton.Update(g_matIdentity);
			//アニメーションを進める
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
		K2_ASSERT(instanceNo < m_maxInstance, "インスタンス番号が不正です。");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		Matrix worldMatrix;
		worldMatrix = matrix;
		// インスタンス番号から行列のインデックスを取得する
		int matrixArrayIndex = m_instanceNoToWorldMatrixArrayIndexTable[instanceNo];
		// インスタンシング描画を行う。
		m_worldMatrixArray[matrixArrayIndex] = worldMatrix;
		if (m_numInstance == 0) {
			//インスタンス数が0の場合のみアニメーション関係の更新を行う。
			// スケルトンを更新。
			// 各インスタンスのワールド空間への変換は、
			// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
			// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
			m_skeleton.Update(g_matIdentity);
			//アニメーションを進める
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::SetWorldMatrixInModes(Matrix matrix)
	{
		m_zprepassModel.SetWorldMatrix(matrix);
		//ディファードレンダリング用のモデルの更新処理
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.SetWorldMatrix(matrix);;
		}

		//フォワードレンダリング用のモデルの更新処理
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.SetWorldMatrix(matrix);;
		}

		//トゥーンシェーダー用のモデルの更新処理
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
		//ディファードレンダリング用のモデルの更新処理
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

		//フォワードレンダリング用のモデルの更新処理
		if (m_forwardRenderModel.IsInited()) {
			m_forwardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

		//トゥーンシェーダー用のモデルの更新処理
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
		//アニメーションが再生されているなら

		if (m_skeleton.IsInited()) {
			//ディファードレンダリング用のスケルトンのの更新処理
			if (m_renderToGBufferModel.IsInited())
			{
				m_skeleton.Update(m_renderToGBufferModel.GetWorldMatrix());
			}
			//フォワードレンダリング用のスケルトンの更新処理
			if (m_forwardRenderModel.IsInited()) {
				m_skeleton.Update(m_forwardRenderModel.GetWorldMatrix());
			}

			//トゥーンシェーダー用のスケルトンのの更新処理
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

