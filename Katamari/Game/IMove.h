#pragma once
enum EnMoveState {
	m_enMove_LR,				//左右に移動する
	m_enMove_FB,				//前後に移動する
	m_enMove_UD,				//上下に移動する
	m_enMove_Rot,				//回転
	m_enMove_Rand,				//ランダム移動
	m_enMove_Chase,				//追いかける
	m_enMove_Away,				//逃げる
	m_enMove_No,				//移動しない
};
/// <summary>
/// オブジェクト移動の基底クラス
/// </summary>
class IMove : public IGameObject
{
public:
	virtual ~IMove(){};

	virtual void Init(const Vector3& pos,const Quaternion& rot){};

	/// <summary>
	/// 移動処理
	/// </summary>
	virtual const Vector3& Move() { return Vector3::Zero; };

	
	/// <summary>
	/// 移動ステートを取得
	/// </summary>
	/// <returns></returns>
	const EnMoveState GetMoveState()const
	{
		return m_moveState;
	}

	/// <summary>
	/// 移動ステートの設定
	/// </summary>
	/// <param name="state"></param>
	void SetMoveState(EnMoveState state)
	{
		m_moveState = state;
	}

protected:
	Vector3 m_position = Vector3::Zero;
	float m_moveSpeed = 0.0f;

	Quaternion m_rotation = Quaternion::Identity;
private:
	EnMoveState m_moveState;					//移動のステート

};
using IMovePtr = std::unique_ptr<IMove>;
