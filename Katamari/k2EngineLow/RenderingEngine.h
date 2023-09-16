#pragma once
namespace nsK2EngineLow 
{
	class RenderingEngine:public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		

		/// <summary>
		/// インスタンスの作成
		/// </summary>
		static void CreateInstance()
		{
			if (m_instance != nullptr)
			{
				std::abort();
			}
			m_instance = new RenderingEngine;
			m_instance->Init();
		}

		/// <summary>
		/// インスタンスの削除
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			if (m_instance == nullptr) {
				CreateInstance();
			}

			return m_instance;
		}

		/// <summary>
		/// レンダリングパイプラインを初期化
		/// </summary>
		void Init();

		/// <summary>
		/// レンダリングパイプラインを実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// 描画オブジェクトを追加
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}
	public:
	private:
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
	    /// メインレンダリングターゲットのカラーバッファの内容を
	    /// フレームバッファにコピーするためのスプライトを初期化する
	    /// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();

		/// <summary>
		/// シャドウマップへの描画処理を初期化
		/// </summary>
		void InitShadowMapRenderTarget();

		/// <summary>
		/// G-Bufferを初期化
		/// </summary>
		void InitGBuffer();

		/// <summary>
		/// ディファードライティングの初期化
		/// </summary>
		void InitDeferredLighting();

		/// <summary>
	    /// 2D描画用のレンダ―ターゲットを初期化
	    /// </summary>
		void Init2DRenderTarget();

		/// <summary>
		/// シャドウマップに描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// G-Bufferへの描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
		/// ディファードレンダリング
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);

		/// <summary>
		/// 2D描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render2D(RenderContext& rc);

		/// <summary>
	    /// メインレンダリングターゲットの内容をフレームバッファにコピーする
	    /// </summary>
	    /// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		/// <summary>
		/// フォワードレンダリング
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ForwardRendering(RenderContext& rc);
	private:
		//GBuffer
		enum EnGBffer
		{
			enGBuffer_Albedo,				//アルベド
			enGBuffer_Normal,				//法線
			enGBuffer_WorldPos,				//ワールド座標
			enGBuffer_MetalSmooth,			//金属度と滑らかさ
			enGBuffer_ShadowParam,			//シャドウパラメータ	
			enGBuffer_Num,
		};

		static RenderingEngine* m_instance;								// 唯一のインスタンスのアドレスを記録する変数。
	
		RenderTarget m_mainRenderTarget;								//メインレンダーターゲット
		RenderTarget m_gBuffer[enGBuffer_Num];							//G-Buffer

		Sprite m_deferredLightInSpr;

		RenderTarget m_2DRenderTarget;                                  // 2D描画用のレンダ―ターゲット。
		Sprite m_2DSprite;                                              // 2D合成用のスプライト。
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;						    // メインレンダリングターゲットをフレームバッファにコピーするためのスプライト

		SpriteInitData m_deferredSpriteInitData;

		std::vector<IRenderer*> m_renderObjects;
	};
}


