#pragma once
class Player;
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
	/// 移動速度の取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()const
	{
		return m_scale;
	}


public:

private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	

	/// <summary>
	/// 回転
	/// </summary>
	void Rotation();

private:
	ModelRender m_sphereRender;
	Player* m_player;
	Vector3 m_position ={0.0f,50.0f,0.0f};
	Vector3 m_beforePosition = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	const float m_protMoveSpeedMultiply = 5.0f;					//最初の速度
	float m_moveSpeedMultiply = 5.0f;							//移動速度
	float m_radius=0.0f;


	CharacterController m_charaCon;

	Quaternion m_rotation= Quaternion::Identity;
	CollisionObject m_collisionObject;
	float m_angle = 0.0f;
};

