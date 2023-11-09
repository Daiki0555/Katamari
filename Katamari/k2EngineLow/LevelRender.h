#pragma once
#include "level3D/TklFile.h"
namespace nsK2EngineLow {

	struct LevelObjecData {
		Vector3			position;		//座標
		Quaternion		rotation;		//回転
		Vector3			scale;			//大きさ
		const wchar_t*  name;			//名前
		int				number = 0;		//番号

		/// <summary>
		/// オブジェクト名を検索する
		/// </summary>
		/// <param name="objName">オブジェクト名</param>
		/// <returns>完全一致ならtrue</returns>
		bool EqualObjectName(const wchar_t* objName)
		{
			return wcscmp(objName, name) == 0;
		}

		bool ForwardMatchName(const wchar_t* objName)
		{
			auto len = wcslen(objName);
			auto namelen = wcslen(name);

			if (len > namelen)
			{
				//名前が長いので不一致
			}
			return wcsncmp(objName, name, len) == 0;
		}


	};
	/// <summary>
	/// レンダラークラス
	/// </summary>
	class LevelRender : public Noncopyable
	{
	public:
		LevelRender();
		~LevelRender();
		/// <summary>
		/// 初期化処理。
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="hookFunc"></param>
		void Init(const char* filePath, std::function<bool(LevelObjecData& objData)> hookFunc);

	private:
		/// <summary>
		/// Tklファイルの行列を変換する
		/// </summary>
		void MatrixTklToLevel();
	private:
		using BonePtr = std::unique_ptr<Bone>;		//ボーンのポインタ
		std::vector<BonePtr>	m_boneList;			//ボーンのリスト
		TklFile					m_tklFile;			//tklファイル


	};

}

