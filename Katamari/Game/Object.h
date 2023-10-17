#pragma once
class Sphere;
class Object :public IGameObject
{
public:
	enum enObjectNameState {

	};

	enum enObjectState {

	};
	~Object();
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
private:
	/// <summary>
	/// オブジェクトの読み込み処理
	/// </summary>
	void InitObject();

	/// <summary>
	/// コリジョンの初期化
	/// </summary>
	void InitCollision();

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Hit();

	/// <summary>
	/// 巻き込み処理
	/// </summary>
	void Involution();

private:
	ModelRender m_objectRender;
	CollisionObject m_collisionObject;
	Sphere* m_sphere;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
};

