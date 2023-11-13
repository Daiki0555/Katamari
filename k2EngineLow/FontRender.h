#pragma once
namespace nsK2EngineLow {

	/// <summary>
	/// �t�H���g�����_�[�N���X
	/// </summary>
	class FontRender : public IRenderer
	{
	public:
		static const int MAX_TEXT_SIZE = 256;	//�����̍āX��
		/// <summary>
		/// �\�����镶���̐ݒ�
		/// </summary>
		/// <param name="text"></param>
		void SetText(const wchar_t* text)
		{
			swprintf_s(m_text, text);
		}

		/// <summary>
		/// �\�����镶�����擾
		/// </summary>
		/// <returns></returns>
		const wchar_t* GetText()
		{
			return m_text;
		}
		/// <summary>
		/// ���W�̐ݒ�Bz��0.0f
		/// </summary>
		/// <param name="pos"></param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ���W�̎擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetPosition()
		{
			return m_position;
		}
		/// <summary>
		/// �傫����ݒ�
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �傫���̎擾
		/// </summary>
		/// <returns></returns>
		const float GetScale()
		{
			return m_scale;
		}
		/// <summary>
		/// �F�̐ݒ�
		/// </summary>
		/// <param name="color"></param>
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}

		/// <summary>
		/// �F�̎擾
		/// </summary>
		/// <returns></returns>
		const Vector4& GetColor()
		{
			return m_color;
		}

		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rot"></param>
		void SetRotation(const float rot)
		{
			m_rotaition = rot;
		}
		/// <summary>
		/// ��]���擾
		/// </summary>
		/// <returns></returns>
		const float GetRotation()
		{
			return m_rotaition;
		}
		/// <summary>
		/// �s�{�b�g��ݒ�
		/// </summary>
		/// <param name="pivot"></param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// �s�{�b�g���擾
		/// </summary>
		/// <returns></returns>
		const Vector2& GetPivot()
		{
			return m_pivot;
		}

		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// �e�̃p�����[�^��ݒ�
		/// </summary>
		/// <param name="isDrawShadow">�e��`�悷�邩�ǂ���</param>
		/// <param name="shadowOffset">�s�N�Z���̃I�t�Z�b�g��</param>
		/// <param name="shadowColor">�e�̐F</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}


	private:
		/// <summary>
		/// 2D�`��p�X����Ă΂�鏈��
		/// </summary>
		/// <param name="rc"></param>
		void OnRender2D(RenderContext& rc) override
		{
			m_font.Begin(rc);
			m_font.Draw(m_text, Vector2(m_position.x, m_position.y), m_color, m_rotaition, m_scale, m_pivot);
			m_font.End(rc);
		}

	private:
		Font		m_font;								//�t�H���g
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;	//�s�{�b�g
		Vector3		m_position = Vector3::Zero;			//���W
		Vector4		m_color = g_vec4White;				//�F
		wchar_t		m_text[MAX_TEXT_SIZE];				//����		
		float		m_scale = 1.0f;						//�傫��
		float		m_rotaition = 0.0f;					//��]
	};
}