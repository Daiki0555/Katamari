#pragma once
namespace nsK2EngineLow{
	class Bloom
	{
	public:
		struct LuminanceCB
		{
			float threshold = 1.0f;	//閾値
		};
	public:
		/// <summary>
		/// ブルームが発生する閾値を設定
		/// </summary>
		/// <param name="threshold">閾値</param>
		void SetThreshold(float threshold)
		{
			m_luminanceCB.threshold = threshold;
		}

		/// <summary>
		/// ブルームが発生する閾値を取得
		/// </summary>
		/// <returns></returns>
		float GetThreshold() const
		{
			return m_luminanceCB.threshold;
		}

		/// <summary>
		/// ポストエフェクトを実行した結果となるテクスチャを取得。
		/// </summary>
		/// <returns></returns>
		Texture& GetResultTexture()
		{
			return m_luminanceRenderTarget.GetRenderTargetTexture();
		}

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="mainRenderTarget"></param>
		void Init(RenderTarget& mainRenderTarget);


		void OnRender(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		RenderTarget m_luminanceRenderTarget;
		LuminanceCB m_luminanceCB;
		GaussianBlur m_gaussianBlur[4];
		Sprite m_finalSprite;
		Sprite m_luminanceSprite;
		Sprite m_copyMainRtSprite;				//メインレンダリングターゲットに描画するためのスプライト

	};
}

