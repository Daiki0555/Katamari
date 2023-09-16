#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()	
	{

	}
	ModelRender::~ModelRender()	
	{

	}
	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		const bool isShadow,
		const bool isShadowReceiver
	)	
	{
		//スケルトンの初期化
		InitSkeleton(filePath);

		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//モデルの初期化
		InitModel(filePath, enModelUpAxis, isShadow, isShadowReceiver);

		//各種ワールド行列を更新する
		UpdateWorldMatrixInModes();
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンデータの読み込み
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
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

	void ModelRender::InitModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis, const bool isShadow, const bool isShadowReceiver)
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

		m_renderToGBufferModel.Init(modelInitData);
		if (isShadow) {
			//シャドウ用のモデルの初期化
			//InitShadowModel(tkmFilePath, modelUpAxis);
		}
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
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

		m_shadowModel.Init(shadowModelInitData);
	}

	void ModelRender::Update()
	{
		//モデルのアップデート
		UpdateWorldMatrixInModes();

		//アニメーションが再生されているなら
		if (m_skeleton.IsInited()) {
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{

		//ディファードレンダリング用のモデルの更新処理
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		//フォワードレンダリング用のモデルの更新処理
		if (m_fowardRenderModel.IsInited()) {
			m_fowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		if (m_shadowModel.IsInited()) {
			m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		RenderingEngine::GetInstance()->AddRenderObject(this);
	}

	void ModelRender::OnRenderToGBuffer(RenderContext& rc)
	{
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.Draw(rc);
		}
	}

}

