#pragma once
namespace nsK2EngineLow
{
	class SpotLight : Noncopyable
	{
	public:
		~SpotLight();

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �X�|�b�g���C�g��ݒ�B
		/// </summary>
		/// <param name="num">�ԍ�</param>
		/// <param name="pos">���W</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		/// <param name="dir">����</param>
		/// <param name="angle">�ˏo�p�x</param>
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
		/// �ԍ���ݒ�B
		/// </summary>
		/// <returns></returns>
		void SetNumber(const int num)
		{
			m_spNum = num;
		}


		/// <summary>
		/// �ԍ��̎擾
		/// </summary>
		/// <returns></returns>
		const int GetNumber()const
		{
			return m_spNum;
		}

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/// <summary>
		/// ���W���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// �J���[��ݒ�B
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color = color;
		}

		/// <summary>
		/// �J���[���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetColor()
		{
			return m_color;
		}

		/// <summary>
		/// �e���͈͂�ݒ�B
		/// </summary>
		/// <param name="range"></param>
		void SetRange(const float range)
		{
			m_range = range;
		}

		/// <summary>
		/// �e���͈͂��擾�B
		/// </summary>
		/// <returns></returns>
		const float GetRange()
		{
			return m_range;
		}

		/// <summary>
		/// �����̐ݒ�
		/// </summary>
		/// <param name="dir">�ˏo����</param>
		void SetDirection(const Vector3& dir)
		{
			m_direction = dir;
			m_direction.Normalize();

		}

		/// <summary>
		/// �������擾�B
		/// </summary>
		/// <returns></returns>
		Vector3& GetDirection()
		{
			return m_direction;
		}

		/// <summary>
		/// �ˏo�p�x
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetAngle(const float angle)
		{
			m_angle = Math::DegToRad(angle);
		}

		/// <summary>
		/// �ˏo�p�x���擾�B
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

