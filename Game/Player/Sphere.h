#pragma once
class Object;
class Game;
class Stick;
class Sphere: public IGameObject
{
public:
	/// <summary>
	/// ボールの当たり判定の大きさの状態
	/// </summary>
	enum enSphereScaleState {
		m_enSphere_Level1,
		m_enSphere_Level2,
		m_enSphere_Level3,
		m_enSphere_Level4,
		m_enSphere_Level5,
		m_enSphere_LevelMax
	};

	/// <summary>
	/// ボールの状態
	/// </summary>
	enum enSphereState {
		m_enSphere_ObjectGet,		//オブジェクト取得時
		m_enSphere_Dash,			//ダッシュ時
		m_enSphere_Damege,			//ダメージ
		m_enSphere_Invincible,		//無敵
		m_enSphere_InvincibleEnd,	//無敵終了
		m_enPlayer_GameEnd,			//ゲーム終了
	};

	~Sphere();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// 更新前の座標
	/// </summary>
	/// <returns></returns>
	const Vector3& GetBeforePosition() const
	{
		return m_beforePosition;
	}

	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	const void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()const
	{
		return m_scale;
	}

	/// <summary>
	/// 移動速度の取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetMoveSpeed()
	{
		return m_moveSpeed;
	}


	/// <summary>
	/// モデル情報の取得
	/// </summary>
	/// <returns></returns>
	ModelRender& GetSphereModel()
	{
		return m_sphereRender;
	}

	/// <summary>
	/// 塊の半径
	/// </summary>
	/// <returns></returns>
	const float GetRadius() const
	{
		return m_radius;
	}
	

	/// <summary>
	/// ダッシュカウントの取得
	/// </summary>
	/// <returns></returns>
	const int GetDashCount() const
	{
		return m_dashCount;
	}

	/// <summary>
	/// 体積を加算する
	/// </summary>
	/// <param name="volume"></param>
	const void AddVolume(const float volume)
	{
		m_volume += volume;
		m_radius = pow(3.0f * m_volume / (4.0f * Math::PI), 1.0f / 3.0f);
		m_charaCon.SetRadius(m_radius);
	}
public:

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 反対同士のステック時の移動
	/// </summary>
	void InverseStickMove();


	/// <summary>
	/// どちらの向きに移動しているかの判定
	/// </summary>
	void IsMoving();
	
	/// <summary>
	/// ダッシュ
	/// </summary>
	void Dash();

	/// <summary>
	/// ブレーキ
	/// </summary>
	void Brake();

	/// <summary>
	/// ダッシュカウント
	/// </summary>
	void DashCount();

	/// <summary>
	/// 重力
	/// </summary>
	void Gravity();
	

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

	/// <summary>
	/// ダッシュ回転
	/// </summary>
	void DashRotation();

private:
	Object* m_object = nullptr;
	Game* m_game = nullptr;
	Stick* m_stick = nullptr;

	ModelRender m_sphereRender;
	FontRender m_fontRender;
	
	Vector3 m_position ={0.0f,50.0f,0.0f};

	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_beforePosition = Vector3::Zero;					//移動前の座標

	Vector3 m_scale = Vector3::One;
	Vector3 m_inputStick = Vector3::Zero;
	
	Vector3 m_vertical = Vector3::Zero;
	
	CharacterSphereController m_charaCon;


	Quaternion m_rotation= Quaternion::Identity;
	
	
	float m_moveSpeedMultiply = 5.0f;							//移動速度

	float m_radius = 0.0f;										//球体の半径

	float m_angle = 0.0f;

	float m_volume = 0.0f;										//オブジェクトの体積

	float m_dashTimer=0.0f;										//ダッシュ可能な判定の時間

	float m_moveDownTime = 0.0f;

	float m_beforeSpeedLength = 0.0f;
	
	int m_dashCount = 0;										//ダッシュカウント

	bool m_isInverseStick = true;								//ステックを逆に倒した時に反転する

	bool m_isDash = false;										//ダッシュしているかどうか

	bool m_isBrake = false;										//ブレーキしていないなら

	bool m_isMovingBackward = false;							// 後退中かどうかを示すフラグ (falseなら前進)
	bool m_wasMovingBackward = false;							// 直前まで後退中だったかどうかを示すフラグ (falseなら前進)
};

