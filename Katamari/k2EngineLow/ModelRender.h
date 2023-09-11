#pragma once
namespace nsK2EngineLow
{
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X</param>
		/// <param name="animationClip">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����̐�</param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadow"></param>
		/// <param name="isShadowReceiver"></param>
		void Init(
			const char* filePath,
			AnimationClip* animationClip = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis=enModelUpAxisZ,
			const bool isShadow=false,
			const bool isShadowReceiver=false
			);
	};
}


