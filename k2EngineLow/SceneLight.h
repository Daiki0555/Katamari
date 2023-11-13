#pragma once
namespace nsK2EngineLow {
	namespace
	{
		const int POINTLIGHT_NUM = 1;
		const int SPOTLIGHT_NUM = 1;
	}

	//ディレクションライトの構造体
	struct DirectionLight
	{
		Vector3 ligDirection = Vector3::Zero;		//ライトの方向
		float pad0 = 0.0f;
		Vector3 ligColor = Vector3::Zero;			//ライトのカラー
		float pad1 = 0.0f;
		Vector3 eyePos = Vector3::Zero;				//視点の方向
		float pad2 = 0.0f;
		Vector3 ambient = Vector3::Zero;			//環境光
		float pad3 = 0.0f;

	};

	/// <summary>
	/// ポイントライト構造体
	/// </summary>
	struct SPointLight
	{
		Vector3 ptPosition = Vector3::Zero;			//座標
		float pad0=0.0f;
 		Vector3 ptColor = Vector3::Zero;			//カラー
		float ptRange = 0.0f;						//影響範囲
	};

	//スポットライトの構造体
	struct SSpotLight
	{
		Vector3 spPosition = Vector3::Zero;			//座標
		float pad0 = 0.0f;
		Vector3 spColor = Vector3::Zero;			//カラー
		float spRange = 0.0f;						//影響範囲
		Vector3 spDirection = Vector3::Zero;		//方向	
		float spAngle = 0.0f;						//射出角度
	};

	//半球ライトの構造体
	struct HemiSphereLight
	{
		Vector3 groundColor = Vector3::Zero;		//地面の色
		float pad0 = 0.0f;
		Vector3 skyColor = Vector3::Zero;			//天球色
		float pad1 = 0.0f;
		Vector3 groundNormal = Vector3::Zero;		//地面の法線
		//float pad2 = 0.0f;
	};

	//ライトの構造体
	struct Light
	{
		DirectionLight		directionLight;					//ディレクションライト
		SPointLight			pointLight[POINTLIGHT_NUM];		//ポイントライト
		SSpotLight			spotLight[SPOTLIGHT_NUM];		//スポットライト
		HemiSphereLight		hemiSphereLight;				//半球ライト
		int ptNum= POINTLIGHT_NUM;							//ポイントライトの数
		int spNum= SPOTLIGHT_NUM;							//スポットライトの数
		int pad[3];
	};

	//シーンライトクラス
	class SceneLight:public Noncopyable
	{
	public:
		SceneLight();
		~SceneLight();

		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// シーンライトを取得
		/// </summary>
		/// <returns></returns>
		Light& GetSceneLight()
		{
			return m_Sinstance->m_light;
		}

		/// <summary>
		/// シーンライトのインスタンスの作成
		/// </summary>
		static void CreateSceneLightInstance()
		{
			if (m_Sinstance != nullptr)
			{
				std::abort();
			}
			m_Sinstance = new SceneLight;
			m_Sinstance->Init();
		}

		/// <summary>
		/// シーンライトのインスタンスを取得
		/// </summary>
		/// <returns></returns>
		static SceneLight* GetSceneLightClass()
		{
			if (m_Sinstance == nullptr) {
				CreateSceneLightInstance();
			}
			return m_Sinstance;
		}


		/// <summary>
		/// ディレクションライトのパラメータの設定
		/// </summary>
		/// <param name="dir">ライトの方向</param>
		/// <param name="color">カラー</param>
		void SetDirectionLight(const Vector3 dir, const Vector3 color)
		{
			m_light.directionLight.ligDirection = dir;
			m_light.directionLight.ligDirection.Normalize();
			m_light.directionLight.ligColor = color;
		}

		/// <summary>
		/// 環境のパラメータの設定
		/// </summary>
		/// <param name="amb">環境光の強さ</param>
		void SetAmbientLight(const float amb)
		{
			m_light.directionLight.ambient.x = amb;
			m_light.directionLight.ambient.y = amb;
			m_light.directionLight.ambient.z = amb;
		}

		/// <summary>
		/// ポイントライトのパラメータの設定
		/// </summary>
		/// <param name="ptNum">番号</param>
		/// <param name="pos">座標</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		void SetPointLight(const int ptNum, const Vector3& pos, const Vector3& color, const float range)
		{
			m_light.pointLight[ptNum].ptPosition = pos;
			m_light.pointLight[ptNum].ptColor = color;
			m_light.pointLight[ptNum].ptRange = range;
		}

		/// <summary>
		/// スポットライトのパラメータの設定
		/// </summary>
		/// <param name="spNum">番号</param>
		/// <param name="pos">座標</param>
		/// <param name="color">カラー</param>
		/// <param name="range">影響範囲</param>
		/// <param name="dir">方向</param>
		/// <param name="angle">射出方向</param>
		void SetSpotLight(const int spNum, const Vector3& pos, const Vector3& color, const float range, const Vector3& dir, const float angle)
		{
			m_light.spotLight[spNum].spPosition = pos;
			m_light.spotLight[spNum].spColor = color;
			m_light.spotLight[spNum].spRange = range;
			m_light.spotLight[spNum].spDirection = dir;
			m_light.spotLight[spNum].spAngle = angle;
		}

		/// <summary>
		/// 半球ライト
		/// </summary>
		/// <param name="grdColor">地面の色</param>
		/// <param name="skyColor">天球色</param>
		/// <param name="grdNormal">法線</param>
		void SetHemiSphereLight(const Vector3& grdColor, const Vector3& skyColor,const Vector3& grdNormal)
		{
			m_light.hemiSphereLight.groundColor = grdColor;
			m_light.hemiSphereLight.skyColor = skyColor;
			m_light.hemiSphereLight.groundNormal = grdNormal;
		}


	private:
		Light m_light;					//ライトの構造体
		static SceneLight* m_Sinstance;
		//SPointLight m_pointLight;		//ポイントライトの構造体
		//SSpotLight m_spotLight;			//スポットライトの構造体
	};

	


}
