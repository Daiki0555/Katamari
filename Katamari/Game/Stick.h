#pragma once
class Stick:public IGameObject
{
public:
	/// <summary>
	/// ステックの状態
	/// </summary>
	enum EnStickState {
		m_enStick_Right,			//右ステックだけ
		m_enStick_Left,				//左ステックだけ
		m_enStick_Both,				//両方のステックが倒れているとき
		m_enStick_No				//何もしていないなら
	};

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


	~Stick();
	void Update();



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

