#pragma once
namespace nsK2EngineLow 
{
	class RenderingEngine:public Noncopyable
	{
	public:
		RenderingEngine();
		~RenderingEngine();
		

		/// <summary>
		/// �C���X�^���X�̍쐬
		/// </summary>
		static void CreateInstance()
		{
			if (m_instance != nullptr)
			{
				std::abort();
			}
			m_instance = new RenderingEngine;
			m_instance->Init();
		}

		/// <summary>
		/// �C���X�^���X�̍폜
		/// </summary>
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/// <summary>
		/// �C���X�^���X�̎擾
		/// </summary>
		/// <returns></returns>
		static RenderingEngine* GetInstance()
		{
			if (m_instance == nullptr) {
				CreateInstance();
			}

			return m_instance;
		}

		/// <summary>
		/// �����_�����O�p�C�v���C����������
		/// </summary>
		void Init();

		/// <summary>
		/// �����_�����O�p�C�v���C�������s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �`��I�u�W�F�N�g��ǉ�
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}
	public:
	private:
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g��������
		/// </summary>
		void InitMainRenderTarget();

		/// <summary>
	    /// ���C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�̓��e��
	    /// �t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g������������
	    /// </summary>
		void InitCopyMainRenderTargetToFrameBufferSprite();

		/// <summary>
		/// �V���h�E�}�b�v�ւ̕`�揈����������
		/// </summary>
		void InitShadowMapRenderTarget();

		/// <summary>
		/// G-Buffer��������
		/// </summary>
		void InitGBuffer();

		/// <summary>
		/// �f�B�t�@�[�h���C�e�B���O�̏�����
		/// </summary>
		void InitDeferredLighting();

		/// <summary>
	    /// 2D�`��p�̃����_�\�^�[�Q�b�g��������
	    /// </summary>
		void Init2DRenderTarget();

		/// <summary>
		/// �V���h�E�}�b�v�ɕ`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// G-Buffer�ւ̕`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
		/// �f�B�t�@�[�h�����_�����O
		/// </summary>
		/// <param name="rc"></param>
		void DeferredLighting(RenderContext& rc);

		/// <summary>
		/// 2D�`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render2D(RenderContext& rc);

		/// <summary>
	    /// ���C�������_�����O�^�[�Q�b�g�̓��e���t���[���o�b�t�@�ɃR�s�[����
	    /// </summary>
	    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		/// <summary>
		/// �t�H���[�h�����_�����O
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ForwardRendering(RenderContext& rc);
	private:
		//GBuffer
		enum EnGBffer
		{
			enGBuffer_Albedo,				//�A���x�h
			enGBuffer_Normal,				//�@��
			enGBuffer_WorldPos,				//���[���h���W
			enGBuffer_MetalSmooth,			//�����x�Ɗ��炩��
			enGBuffer_ShadowParam,			//�V���h�E�p�����[�^	
			enGBuffer_Num,
		};

		static RenderingEngine* m_instance;								// �B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B
	
		RenderTarget m_mainRenderTarget;								//���C�������_�[�^�[�Q�b�g
		RenderTarget m_gBuffer[enGBuffer_Num];							//G-Buffer

		Sprite m_deferredLightInSpr;

		RenderTarget m_2DRenderTarget;                                  // 2D�`��p�̃����_�\�^�[�Q�b�g�B
		Sprite m_2DSprite;                                              // 2D�����p�̃X�v���C�g�B
		Sprite m_mainSprite;
		Sprite m_copyMainRtToFrameBufferSprite;						    // ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g

		SpriteInitData m_deferredSpriteInitData;

		std::vector<IRenderer*> m_renderObjects;
	};
}


