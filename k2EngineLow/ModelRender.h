#pragma once
namespace nsK2EngineLow{
	class ModelRender :IRenderer
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// �f�B�t�@�[�h�����_�����O�̏�����
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationClip">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		/// <param name="isShadow">�e�𐶐����邩</param>
		/// <param name="isShadowReceiver">�e�𗎂Ƃ���</param>
		void InitDeferredRendering(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis=enModelUpAxisZ,
			const bool isShadow=false,
			const bool isShadowReceiver=false,
			const bool isWipeModel=false
			);

		/// <summary>
		/// �g�D�[���V�F�[�_�[�p���f���̏���������
		/// </summary>
		/// <param name="initData"></param>
		void InitToonModel(const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			const bool isShadow = false,
			const bool isShadowReceiver = false,
			const bool isOutline = false,
			const bool isToon=false
		);

		/// <summary>
		/// ����ȃV�F�[�f�B���O���s�������ꍇ�̏����������B
		/// </summary>
		/// <param name="initData">���f���f�[�^�B</param>
		void InitForwardRendering(ModelInitData& initData);

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �������܂ꂽ�I�u�W�F�N�g�̍X�V����
		/// </summary>
		void InvolutionModelsUpdate(
			Matrix matrix,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);


		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// �A�j���[�V�����̍Đ����x
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ�</param>
		/// <param name="interpolateTime">�⊮����(�P�� : �b)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�������Đ������ǂ���
		/// </summary>
		/// <returns></returns>
		bool IsPlayingAnimation()const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// ���f���̎擾
		/// </summary>
		/// <returns></returns>
		Model& GetModel()
		{
			//�f�B�t�@�[�h�����_�����O�p�̃X�P���g���̂̍X�V����
			if (m_renderToGBufferModel.IsInited())
			{
				return m_renderToGBufferModel;
			}
			//�t�H���[�h�����_�����O�p�̃X�P���g���̍X�V����
			if (m_forwardRenderModel.IsInited()) {
				return m_forwardRenderModel;
			}

			//�g�D�[���V�F�[�_�[�p�̃X�P���g���̂̍X�V����
			if (m_toonModel.IsInited()) {
				return m_toonModel;
			}

		}

		/// <summary>
		/// ���W�A��]�A�g���S�Đݒ�B
		/// </summary>
		/// <remark>
		/// �C���X�^���V���O�`�悪�L���̏ꍇ�́A���̐ݒ�͖�������܂��B
		/// </remark>
		/// <param name="pos">���W�B</param>
		/// <param name="rotation">��]�B</param>
		/// <param name="scale">�g��B</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
		}

		/// <summary>
		/// ���W�̐ݒ�
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
		const Vector3 GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rotaition">��]</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// �g�嗦�̐ݒ�
		/// </summary>
		/// <param name="scale">�g�嗦</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �A�j���[�V�����Đ��̑��x��ݒ肷��
		/// </summary>
		/// <param name="animationSpeed">���l�̕������{�ɂ���@</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}


		/// <summary>
		/// �������܂�Ă��邩�̐ݒ�
		/// </summary>
		/// <param name="flag"></param>
		void SetIsInvolution(const bool flag)
		{
			m_isInvolution = flag;
		}

		/// <summary>
		/// ���[���h�s��̑��
		/// </summary>
		/// <param name="matrix"></param>
		void SetMatrix(const Matrix matrix)
		{
			m_setMatrix = matrix;
		}

		/// <summary>
		/// �{�[�����O����{�[���ԍ�������
		/// </summary>
		/// <param name="boneName">�{�[���̖��O</param>
		/// <returns></returns>
		int FindBoneID(const wchar_t* boneName) const
		{
			return m_skeleton.FindBoneID(boneName);
		}
		/// <summary>
		/// �{�[���ԍ�����{�[�����擾
		/// </summary>
		/// <param name="boneNo">�{�[���ԍ�</param>
		/// <returns></returns>
		Bone* GetBone(int boneNo)const
		{
			return m_skeleton.GetBone(boneNo);
		}
		/// <summary>
		/// G-Buffer�`��p�X����Ă΂�鏈��
		/// </summary>
		void OnRenderToGBuffer(RenderContext& rc) override
		{
			if (m_renderToGBufferModel.IsInited()) {
				m_renderToGBufferModel.Draw(rc);
			}
		}

		/// <summary>
		/// �t�H���[�h�����_�[�p�X����Ă΂�鏈��
		/// </summary>
		void OnForwardRender(RenderContext& rc)override
		{
			if (m_forwardRenderModel.IsInited()) {
				m_forwardRenderModel.Draw(rc);
			}
		}

		/// <summary>
		/// �g�D�[�������_�[�p�X����Ă΂�鏈��
		/// </summary>
		void OnToonRender(RenderContext& rc)override
		{
			if (m_toonModel.IsInited()){
				m_toonModel.Draw(rc);
			}
		}

		/// <summary>
		/// ZPrepass����Ă΂�鏈��
		/// </summary>
		/// <param name="rc"></param>
		void OnZPrepass(RenderContext& rc)override
		{
			m_zprepassModel.Draw(rc);
		}

		/// <summary>
		/// ���C�v��������Ă΂��
		/// </summary>
		/// <param name="rc"></param>
		void OnWipeModelRender(RenderContext& rc, Camera& camera) override
		{
			if (m_wipeModel.IsInited()) {
				m_wipeModel.Draw(rc, camera, 1);
			}
		}

		/// <summary>
		/// �V���h�E�}�b�v�ւ̕`��p�X����Ă΂�鏈���B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="shadowMapNo">�V���h�E�}�b�v�ԍ�</param>
		/// <param name="lvpMatrix">���C�g�r���[�v���W�F�N�V�����s��</param>
		void OnRenderShadowMap(
			RenderContext& rc,
			int shadowMapNo,
			const Matrix& lvpMatrix) override;
		
			
		

	private:
		/// <summary>
		/// �X�P���g���̏�����
		/// </summary>
		/// <param name="filePath"></param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// �A�j���[�V�����̏�����
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// �f�B�t�@�[�h�����_�����O�p�̃��f���̏�����
		/// </summary>
		/// <param name="tkmFilePath">�t�@�C���p�X</param>
		/// <param name="modelUpAxis">���f���̏����</param>
		///<param name = "isShadow">true�Ȃ�e��^����< / param>
		/// <param name=isShadowReceiver">true�Ȃ�e�𗎂Ƃ�</param>
		void InitDeferredModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const bool isShadow,
			const bool isShadowReceiver,
			const bool isWipeModel
		);

		/// <summary>
		/// �t�H���[�h�����_�����O�p�̃��f���̏�����
		/// </summary>
		/// <param name="tkmFilePath">�t�@�C���p�X</param>
		/// <param name="modelUpAxis">���f���̏����</param>
		///<param name = "isShadow">true�Ȃ�e��^����< / param>
		/// <param name=isShadowReceiver">true�Ȃ�e�𗎂Ƃ�</param>
		void InitModelOnToon(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			const bool isShadow,
			const bool isShadowReceiver,
			const bool isOutline,
			const bool isToon
		);


		/// <summary>
		/// �V���h�E�p�̃��f���̏�����
		/// </summary>
		/// <param name="tkmFilePath">�t�@�C���p�X</param>
		/// <param name="modelUpAxis">���f���̏����</param>
		void InitShadowModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);


		/// <summary>
		/// ZPrepass�p�̃��f���̏�����
		/// </summary>
		/// <param name="tkmFilePath"></param>
		/// <param name="modelUpAxis"></param>
		void InitZPrepassModel(
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);


		/// <summary>
		/// ���[���h�s���������
		/// </summary>
		void SetWorldMatrixInModes(Matrix matrix);

		/// <summary>
		/// �e�탂�f���̃��[���h�s����X�V����B
		/// </summary>
		void UpdateWorldMatrixInModes();

		/// <summary>
		/// �e��X�P���g�����X�V����
		/// </summary>
		void UpdateModelSkeletons();


	private:

		Model			m_renderToGBufferModel;
		Model			m_forwardRenderModel;
		Model			m_toonModel;
		Model			m_zprepassModel;
		Model			m_shadowModels[NUM_SHADOW_MAP];						//�e�����郂�f��
		Model			m_wipeModel;

		Skeleton		m_skeleton;							

		EnModelUpAxis	m_enFbxUpAxis = enModelUpAxisZ;		//���f���̏����

		AnimationClip*  m_animationClips = nullptr;			
		Animation		m_animation;

		Vector3			m_position = Vector3::Zero;
		Vector3			m_scale = Vector3::One;

		Quaternion		m_rotation = Quaternion::Identity;
	
		int				m_numAnimationClips = 0;			//�A�j���[�V�����N���b�v�̐�
		
		float			m_animationSpeed = 1.0f;			//�A�j���[�V�����X�s�[�h
		
		bool			m_isUpdateAnimation = true;			//�A�j���[�V�������X�V���邩�̔���

		bool			m_isInvolution = false;				//�������܂�Ă��邩�̔���

		Matrix			m_setMatrix;						//���[���h�s�s��������邽�߂����̕ϐ�
	};
}


