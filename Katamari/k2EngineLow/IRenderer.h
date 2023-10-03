#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// レンダラーの基底クラス
	/// </summary>
	class IRenderer :public Noncopyable {
	public:
		/// <summary>
		/// G-Buffer描画パスから呼ばれる処理。
		/// </summary>
		virtual void OnRenderToGBuffer(RenderContext& rc)
		{

		}

		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理。
		/// </summary>
		virtual void OnForwardRender(RenderContext& rc)
		{

		}


		/// <summary>
		/// トゥーンレンダーパスから呼ばれる処理
		/// </summary>
		virtual void OnToonRender(RenderContext& rc)
		{
			
		}

		/// <summary>
		/// 2D描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}


		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		virtual void OnRenderShadowMap(
			RenderContext& rc,
			int shadowMapNo,
			const Matrix& lvpMatrix
		)
		{

		}
	};
}