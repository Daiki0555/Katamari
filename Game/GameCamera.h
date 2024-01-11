#pragma once
class Sphere;
class Stick;
class Player;
class Game;
class GameClear;
class GameCamera:public IGameObject
{
public:
	~GameCamera();
	bool Start();
	void Update();
	
	/// <summary>
	/// ゲーム終了時のカメラの前方向を設定
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetEndCameraFoward(Vector3 pos)
	{
		m_endCameraFront = pos;
	}

private:
	/// <summary>
	/// カメラの回転
	/// </summary>
	void Rotation();

	/// <summary>
	/// カメラの向きを反転させる
	/// </summary>
	void InvertCamera();

	/// <summary>
	/// 終わり時のカメラの処理
	/// </summary>
	void EndCamera();
private:
	Sphere*				m_sphere=nullptr;
	//Player* m_sphere = nullptr;
	Stick*				m_stick = nullptr;
	Game*				m_game = nullptr;
	GameClear*			m_gameClear = nullptr;


	Quaternion			m_rotation = Quaternion::Identity;
	Vector3				m_target;								//注視点
	Vector3				m_toCameraPos;							//注視点から視点に向かうベクトル
	Vector3				m_initialCameraPos;
	Vector3				m_axis = Vector3::Zero;
	Vector3				m_endCameraFront = Vector3::Zero;


	float				m_turnTimer = 0.0f;						//カメラの回転時間
	float				m_degree = 0.0f;
	float				m_endTime = 2.0f;
	bool				m_isTurningCamera = false;				//カメラを回転させているどうか
};

