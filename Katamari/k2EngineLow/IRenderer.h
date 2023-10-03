#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// �����_���[�̊��N���X
	/// </summary>
	class IRenderer :public Noncopyable {
	public:
		/// <summary>
		/// G-Buffer�`��p�X����Ă΂�鏈���B
		/// </summary>
		virtual void OnRenderToGBuffer(RenderContext& rc)
		{

		}

		/// <summary>
		/// �t�H���[�h�����_�[�p�X����Ă΂�鏈���B
		/// </summary>
		virtual void OnForwardRender(RenderContext& rc)
		{

		}


		/// <summary>
		/// �g�D�[�������_�[�p�X����Ă΂�鏈��
		/// </summary>
		virtual void OnToonRender(RenderContext& rc)
		{
			
		}

		/// <summary>
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}


		/// <summary>
		/// �V���h�E�}�b�v�ւ̕`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="lvpMatrix">���C�g�r���[�v���W�F�N�V�����s��</param>
		virtual void OnRenderShadowMap(
			RenderContext& rc,
			int shadowMapNo,
			const Matrix& lvpMatrix
		)
		{

		}
	};
}