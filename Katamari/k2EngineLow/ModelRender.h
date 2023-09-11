#pragma once
namespace nsK2EngineLow
{
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClip">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションの数</param>
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


