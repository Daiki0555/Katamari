#pragma once
namespace nsK2EngineLow {
	class SpriteRender : public IRenderer
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="w">画像の横幅</param>
		/// <param name="h">画像の縦幅</param>
		/// <param name="alphaBlendMode">デフォルトは半透明</param>
		void Init(
			const char* filePath,
			const float w,
			const float h,
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans,
			bool isCircle=false
		);

		/// <summary>
		/// 座標を設定。zは0.0f
		/// </summary>
		/// <param name="pos">座標（zは0.0f）</param>
		/// <returns></returns>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// 座標を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPosition()const
		{
			return m_position;
		}
		/// <summary>
		/// 大きさを設定。zは1.0f
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 大きさを取得
		/// </summary>
		/// <returns></returns>
		const Vector3 GetScale()const
		{
			return m_scale;
		}
		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rot">回転</param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotaition = rot;
		}
		/// <summary>
		/// 回転を取得
		/// </summary>
		/// <returns>回転</returns>
		const Quaternion& GetRotation()const
		{
			return m_rotaition;
		}
		/// <summary>
		/// ピボットを設定
		/// </summary>
		/// <param name="pivot">ピボット</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを取得
		/// </summary>
		/// <returns></returns>
		const Vector2 GetPivot()const
		{
			return m_pivot;
		}
		/// <summary>
		/// 乗算カラーを設定
		/// </summary>
		/// <param name="mulColor">乗算カラー</param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		/// <summary>
		/// 乗算カラーを取得
		/// </summary>
		/// <returns></returns>
		const Vector4& GetMulColor()const
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotaition,
				m_scale,
				m_pivot
			);
		}
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

	private:
		/// <summary>
		/// 2D描画パスから呼ばれる処理
		/// </summary>
		/// <param name="rc"></param>
		void OnRender2D(RenderContext& rc) override
		{
			m_sprite.Draw(rc);
		}
	private:
		Sprite			m_sprite;
		Vector3			m_position = Vector3::Zero;
		Vector3			m_scale = Vector3::One;
		Quaternion		m_rotaition = Quaternion::Identity;
		Vector2			m_pivot = Sprite::DEFAULT_PIVOT;
	};

}

