#pragma once
/// <summary>
/// ステックの状態
/// </summary>
enum EnStickState {
	m_enStick_Right,					//右ステックだけ
	m_enStick_Left,						//左ステックだけ
	m_enStick_Both,						//両方のステックが倒れているなら
	m_enStick_No,						//何もしていないなら
	m_enStick_LeftYInverseRightY,		//Y方向で+左ステックと反対に-右ステックになっているなら
	m_enStick_RightYInverseLeftY,		//Y方向で+右ステックと反対に-左ステックになっているなら
};

class Stick:public IGameObject
{
public:

	~Stick();
	void Update();

	/// <summary>
	/// ステックの入力情報
	/// </summary>
	/// <returns></returns>
	const EnStickState GetStickState()const
	{
		return m_stickState;
	}

	/// <summary>
	/// 右ステックの入力量
	/// </summary>
	/// <returns></returns>
	const Vector3 GetRStickValue()const
	{
		return m_Rstick;
	}

	/// <summary>
	/// 左ステックの入力量
	/// </summary>
	/// <returns></returns>
	const Vector3 GetLStickValue()const
	{
		return m_Lstick;
	}



public:

private:
	/// <summary>
	/// ステックの状態の遷移
	/// </summary>
	void StickState();
private:
	EnStickState m_stickState;


	Vector3 m_Lstick = Vector3::Zero;						//左ステックの入力量
	Vector3 m_Rstick = Vector3::Zero;						//右ステックの入力量
};

