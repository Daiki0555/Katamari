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
		/// 2D�`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRender2D(RenderContext& rc)
		{

		}
	};
}