#pragma once
class StageCollider:public IGameObject
{
public:
	~StageCollider();
	bool Start();

	/// <summary>
	/// ç¿ïWÇÃê›íË
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// âÒì]ÇÃê›íË
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// ëÂÇ´Ç≥ÇÃê›íË
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	const void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

private:
	
private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsObject;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
};

