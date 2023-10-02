#pragma once
namespace nsK2EngineLow
{
	class SpotLight : Noncopyable
	{
	public:
		~SpotLight();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// スポットライトを設定。
		/// </summary>
		/// <param name="num">番号</param>
		/// <param name="pos">座標</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		/// <param name="dir">方向</param>
		/// <param name="angle">射出角度</param>
		void SetSpotLight(const int num, const Vector3& pos, const Vector3& color, const float range, const Vector3& dir, const float angle)
		{
			SetNumber(num);
			SetPosition(pos);
			SetColor(color);
			SetRange(range);
			SetDirection(dir);
			SetAngle(angle);
			Update();
		}

		/// <summary>
		/// 番号を設定。
		/// </summary>
		/// <returns></returns>
		void SetNumber(const int num)
		{
			m_spNum = num;
		}


		/// <summary>
		/// 番号の取得
		/// </summary>
		/// <returns></returns>
		const int GetNumber()const
		{
			return m_spNum;
		}

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// カラーを設定。
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color = color;
		}

		/// <summary>
		/// カラーを取得。
		/// </summary>
		/// <returns></returns>
		const Vector3& GetColor()
		{
			return m_color;
		}

		/// <summary>
		/// 影響範囲を設定。
		/// </summary>
		/// <param name="range"></param>
		void SetRange(const float range)
		{
			m_range = range;
		}

		/// <summary>
		/// 影響範囲を取得。
		/// </summary>
		/// <returns></returns>
		const float GetRange()
		{
			return m_range;
		}

		/// <summary>
		/// 方向の設定
		/// </summary>
		/// <param name="dir">射出方向</param>
		void SetDirection(const Vector3& dir)
		{
			m_direction = dir;
			m_direction.Normalize();

		}

		/// <summary>
		/// 方向を取得。
		/// </summary>
		/// <returns></returns>
		Vector3& GetDirection()
		{
			return m_direction;
		}

		/// <summary>
		/// 射出角度
		/// </summary>
		/// <param name="angle">角度</param>
		void SetAngle(const float angle)
		{
			m_angle = Math::DegToRad(angle);
		}

		/// <summary>
		/// 射出角度を取得。
		/// </summary>
		/// <returns></returns>
		const float GetAngle()
		{
			return m_angle;
		}

	private:
		Vector3 m_position = Vector3::Zero;
		Vector3 m_color = Vector3::Zero;
		Vector3 m_direction = Vector3::Zero;

		float m_range = 0.0f;
		float m_angle = 0.0f;

		SSpotLight m_spotLight;
		SceneLight m_sceneLight;
		int m_spNum=0;
	};

}

