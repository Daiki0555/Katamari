#pragma once

namespace nsK2EngineLow {

	/// <summary>
	///	�|�C���g���C�g
	/// </summary>
	class PointLight : Noncopyable {
	public:
		//�f�X�g���N�^
		~PointLight();
		void Update();

		/// <summary>
		/// �|�C���g���C�g�̐ݒ�
		/// </summary>
		/// <param name="num">�ԍ�</param>
		/// <param name="pos">���W</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		void SetPointLight(const int num, const Vector3& pos, const Vector3 color, const float range)
		{
			SetNumber(num);
			SetPosition(pos);
			SetColor(color);
			SetRange(range);
			Update();
		}


		/// <summary>
	    /// ���W��ݒ�B
	    /// </summary>
	    /// <param name="position"></param>
		void SetPosition(const Vector3& position)
		{
			m_position =position;
		}

		/// <summary>
	    /// ���W���擾�B
	    /// </summary>
	    /// <returns></returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// �ԍ��̐ݒ�
		/// </summary>
		/// <param name="num"></param>
		void SetNumber(const int num)
		{
			m_ptNum = num;
		}

		/// <summary>
		/// �ԍ��̎擾
		/// </summary>
		/// <returns></returns>
		const int GetNumber()const
		{
			return m_ptNum;
		}
		/// <summary>
	    /// �J���[��ݒ�B
	    /// </summary>
	    /// <param name="color"></param>
		void SetColor(const Vector3& color)
		{
			m_color =color;
		}

		/// <summary>
		/// �J���[���擾�B
		/// </summary>
		/// <returns></returns>
		const Vector3& GetColor() const
		{
			return m_color;
		}

		/// <summary>
		/// �͈͂�ݒ�B
		/// </summary>
		/// <param name="range"></param>
		void SetRange(float range)
		{
			m_range = range;
		}

		/// <summary>
	    /// �e���͈͂��擾�B
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

		SPointLight* m_pointLight = nullptr;	// �|�C���g���C�g�B
		int m_ptNum=0;

		SceneLight m_sceneLight;
	};
}


