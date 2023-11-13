#pragma once
class Sphere;

class Player :public IGameObject
{
public:
	enum EnPlayerState {
		m_enPlayer_Idle,			//待機中
		m_enPlayer_walk,			//歩く
		m_enPlayer_Run,				//走る
		m_enPlayer_Back,			//後退
		m_enPlayer_Damage,			//ダメージ
		m_enPlayer_Invincible,		//無敵
		m_enPlayer_InvincibleEnd,	//無敵終了
		m_enPlayer_GameEnd,			//ゲーム終了
	};



	~Player();
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
	/// プレイヤーの回転の設定
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// プレイヤーの回転の取得
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
	/// プレイヤーの状態を設定
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetPlayerState(EnPlayerState state)
	{
		m_playerState = state;
	}

	/// <summary>
	/// プレイヤーの状態を取得
	/// </summary>
	/// <returns></returns>
	const EnPlayerState GetPlayerState()const
	{
		return m_playerState;
	}

public:

private:
	/// <summary>
	/// プレイヤーの移動
	/// </summary>
	void Move();

	/// <summary>
	/// プレイヤーの回転
	/// </summary>
	void Rotation();

	
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void Animation();

	/// <summary>
	/// 共通のステート遷移処理
	/// </summary>
	void ProcessCommonStateTransition();

	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition();

	/// <summary>
	/// 移動ステートの遷移処理
	/// </summary>
	void ProcessWalkStateTransition();

	/// <summary>
	/// ダッシュステートの遷移処理
	/// </summary>
	void ProcessRunStateTransition();

	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();

private:
	Sphere* m_sphere=nullptr;


	ModelRender m_playerRender;


	//アニメーション
	enum EnAnimationClip {
		m_enAnimationClip_Idle,				//待機アニメーション
		m_enAnimationClip_Walk,				//歩きアニメーション
		m_enAnimationClip_Run,				//走りアニメーション
		m_enAnimationClip_Num,				//アニメーションの数
	};
	EnPlayerState m_playerState = m_enPlayer_Idle;				//待機状態
	AnimationClip m_animationClips[m_enAnimationClip_Num];

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;


	Vector3 m_scale = Vector3::One;

	Quaternion m_rotation = Quaternion::Identity;
	
	CharacterController* m_characon;							//キャラコン

	float m_radius = 0.0f;										//球体の半径

	
};

