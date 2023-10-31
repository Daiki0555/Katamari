#pragma once
class BackGround:public IGameObject
{
public:
	~BackGround();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// À•W‚Ìİ’è
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// À•W‚ğæ“¾B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// ‰ñ“]‚Ìİ’è
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ‰ñ“]‚Ìæ“¾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// ‘å‚«‚³‚Ìİ’è
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	const void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}
private:
private:
	PhysicsStaticObject m_physicsObj;
	ModelRender m_backRender;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
};

