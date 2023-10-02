#pragma once
namespace nsK2EngineLow{
	class ModelRender :IRenderer
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// ディファードレンダリングの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClip">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションの数</param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadow"></param>
		/// <param name="isShadowReceiver"></param>
		void InitDeferredRendering(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis=enModelUpAxisZ,
			const bool isShadow=false,
			const bool isShadowReceiver=false
			);

		/// <summary>
		/// トゥーンシェーダー用モデルの初期化処理
		/// </summary>
		/// <param name="initData"></param>
		void InitToonModel(const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadow = false,
			const bool isShadowReceiver = false
		);

		/// <summary>
		/// 特殊なシェーディングを行いたい場合の初期化処理。
		/// </summary>
		/// <param name="initData">モデルデータ。</param>
		void InitForwardRendering(ModelInitData& initData);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// アニメーションの再生速度
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号</param>
		/// <param name="interpolateTime">補完時間(単位 : 秒)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションが再生中かどうか
		/// </summary>
		/// <returns></returns>
		bool IsPlayingAnimation()const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// モデルの取得
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			return m_model;
		}

		/// <summary>
		/// 座標、回転、拡大を全て設定。
		/// </summary>
		/// <remark>
		/// インスタンシング描画が有効の場合は、この設定は無視されます。
		/// </remark>
		/// <param name="pos">座標。</param>
		/// <param name="rotation">回転。</param>
		/// <param name="scale">拡大。</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
		}

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 座標の取得
		/// </summary>
		/// <returns></returns>
		const Vector3 GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rotaition">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 拡大率の設定
		/// </summary>
		/// <param name="scale">拡大率</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// アニメーション再生の速度を設定する
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする　</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// ボーン名前からボーン番号を検索
		/// </summary>
		/// <param name="boneName">ボーンの名前</param>
		/// <returns></returns>
		int FindBoneID(const wchar_t* boneName) const
		{
			return m_skeleton.FindBoneID(boneName);
		}
		/// <summary>
		/// ボーン番号からボーンを取得
		/// </summary>
		/// <param name="boneNo">ボーン番号</param>
		/// <returns></returns>
		Bone* GetBone(int boneNo)const
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// G-Buffer描画パスから呼ばれる処理
		/// </summary>
		void OnRenderToGBuffer(RenderContext& rc) override
		{
			if (m_renderToGBufferModel.IsInited()) {
				m_renderToGBufferModel.Draw(rc);
			}
		}

		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理
		/// </summary>
		void OnForwardRender(RenderContext& rc)override
		{
			if (m_forwardRenderModel.IsInited()) {
				m_forwardRenderModel.Draw(rc);
			}
		}

		/// <summary>
		/// トゥーンレンダーパスから呼ばれる処理
		/// </summary>
		void OnToonRender(RenderContext& rc)override
		{
			if (m_toonModel.IsInited()){
				m_toonModel.Draw(rc);
			}
		}

		
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="shadowMapNo">シャドウマップ番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		void OnRenderShadowMap(
			RenderContext& rc,
			int shadowMapNo,
			const Matrix& lvpMatrix) override;
		
			
		

	private:
		/// <summary>
		/// スケルトンの初期化
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// アニメーションの初期化
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上向き</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// ディファードレンダリング用のモデルの初期化
		/// </summary>
		/// <param name="tkmFilePath">ファイルパス</param>
		/// <param name="modelUpAxis">モデルの上方向</param>
		///<param name = "isShadow">trueなら影を与える< / param>
		/// <param name=isShadowReceiver">trueなら影を落とす</param>
		void InitDeferredModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const bool isShadow,
			const bool isShadowReceiver
		);

		/// <summary>
		/// フォワードレンダリング用のモデルの初期化
		/// </summary>
		/// <param name="tkmFilePath">ファイルパス</param>
		/// <param name="modelUpAxis">モデルの上方向</param>
		///<param name = "isShadow">trueなら影を与える< / param>
		/// <param name=isShadowReceiver">trueなら影を落とす</param>
		void InitModelOnToon(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const bool isShadow,
			const bool isShadowReceiver
		);


		/// <summary>
		/// シャドウ用のモデルの初期化
		/// </summary>
		/// <param name="tkmFilePath">ファイルパス</param>
		/// <param name="modelUpAxis">モデルの上方向</param>
		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);




		/// <summary>
		/// 各種モデルのワールド行列を更新する。
		/// </summary>
		void UpdateWorldMatrixInModes();
	private:


		Model			m_model;
		Model			m_renderToGBufferModel;
		Model			m_forwardRenderModel;
		Model			m_toonModel;
		Model			m_shadowModels[NUM_SHADOW_MAP];						//影があるモデル
		
		Skeleton		m_skeleton;							

		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;		//モデルの上方向

		AnimationClip*  m_animationClips = nullptr;			
		Animation		m_animation;

		Vector3			m_position = Vector3::Zero;
		Vector3			m_scale = Vector3::One;

		Quaternion		m_rotation = Quaternion::Identity;
	
		int				m_numAnimationClips = 0;			//アニメーションクリップの数
		
		float			m_animationSpeed = 1.0f;			//アニメーションスピード
		
		bool			m_isUpdateAnimation = true;			//アニメーションを更新するかの判定
	};
}


