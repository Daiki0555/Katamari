#pragma once
#include "IMove.h"
class MoveRand :public IMove
{
public:
	~MoveRand()override;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	void Init(const Vector3& pos, const Quaternion& rot, const float speed, const float range) override;


	/// <summary>
	/// 移動処理
	/// </summary>
	const Vector3& Move() override;
public:
private:
	/// <summary>
	/// ランダムな座標の初期化
	/// </summary>
	void InitRandomPosition();
private:
	
	Vector3 m_firstPosition = Vector3::Zero;										//最初の座標
	Vector3 m_randomPosition = Vector3::Zero;										//ランダムな移動範囲
	Vector3 m_objectDiff = Vector3::Zero;
	float m_limitMoveRange = 0.0f;													//移動範囲の限界
	float m_moveTime = 0.0f;														//移動出来る時間
	bool m_isStop = false;															//止まっているかの判定
	bool m_isInitRandom = false;

};
