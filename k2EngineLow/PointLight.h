#pragma once

namespace nsK2EngineLow {

	/// <summary>
	///	ポイントライト
	/// </summary>
	class PointLight : Noncopyable {
	public:
		//デストラクタ
		~PointLight();
		void Update();

		/// <summary>
		/// ポイントライトの設定
		/// </summary>
		/// <param name="num">番号</param>
		/// <param name="pos">座標</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		void SetPointLight(const int num, const Vector3& pos, const Vector3 color, const float range)
		{
			SetNumber(num);
			SetPosition(pos);
			SetColor(color);
			SetRange(range);
			Update();
		}


		/// <summary>
	    /// 座標を設定。
	    /// </summary>
	    /// <param name="position"></param>
		void SetPosition(const Vector3& position)
		{
			m_position =position;
		}

		/// <summary>
	    /// 座標を取得。
	    /// </summary>
	    /// <returns></returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// 番号の設定
		/// </summary>
		/// <param name="num"></param>
		void SetNumber(const int num)
		{
			m_ptNum = num;
		}

		/// <summary>
		/// 番号の取得
		/// </summary>
		/// <returns></returns>
		const int GetNumber()const
		{
			return m_ptNum;
		}
		/// <summary>
	    /// カラーを設定。
	    /// </summary>
	    /// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color =color;
		}

		/// <summary>
		/// カラーを取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetColor() const
		{
			return m_color;
		}

		/// <summary>
		/// 範囲を設定。
		/// </summary>
		/// <param name="range"></param>
		void SetRange(float range)
		{
			m_range = range;
		}

		/// <summary>
	    /// 影響範囲を取得。
	    /// </summary>
	    /// <returns></returns>
		float GetRange() const
		{
			return m_range;
		}

	private:
		Vector3 m_position=Vector3::Zero;
		Vector3 m_color = Vector3::Zero;
		float m_range = 0.0f;

		SPointLight* m_pointLight = nullptr;	// ポイントライト。
		int m_ptNum=0;

		SceneLight m_sceneLight;
	};
}


