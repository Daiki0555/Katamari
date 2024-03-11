#include "stdafx.h"
#include "LineSegment.h"
#include "Physics/CollisionAttr.h"
#include "Player/Sphere.h"
//衝突したときに呼ばれる関数オブジェクト(地面用)
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;									//衝突フラグ。
	Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
	Vector3 startPos = Vector3::Zero;				//レイの始点。
	Vector3 hitNormal = Vector3::Zero;				//衝突点の法線。
	btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
	float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
	float playerRadius = 0.0f;							//プレイヤーの半径
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
			) {
			//自分に衝突した。or キャラクタ属性のコリジョンと衝突した。
			return 0.0f;
		}
		//衝突点の法線を引っ張ってくる。
		Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
		//上方向と法線のなす角度を求める。
		float angle = hitNormalTmp.y;
		angle = fabsf(acosf(angle));
		if (angle < Math::PI * 0.3f		//地面の傾斜が54度より小さいので地面とみなす。
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //もしくはコリジョン属性が地面と指定されている。
			) {
			//衝突している。
			isHit = true;
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//衝突点の距離を求める。。
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//この衝突点の方が近いので、最近傍の衝突点を更新する。
				hitPos = hitPosTmp;
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}
		return 0.0f;
	}
};
//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	Vector3 hitPos;							//衝突点。
	Vector3 startPos;						//レイの始点。
	float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
	Vector3 hitNormal;						//衝突点の法線。
	btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
											//衝突したときに呼ばれるコールバック関数。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getInternalType() == btCollisionObject::CO_GHOST_OBJECT
			) {
			//自分に衝突した。or 地面に衝突した。
			return 0.0f;
		}
		//衝突点の法線を引っ張ってくる。
		Vector3 hitNormalTmp;
		hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;

		//上方向と衝突点の法線のなす角度を求める。
		float angle = fabsf(acosf(hitNormalTmp.y));
		if (angle >= Math::PI * 0.3f		//地面の傾斜が54度以上なので壁とみなす。
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//もしくはコリジョン属性がキャラクタなので壁とみなす。
			) {
			isHit = true;
			Vector3 hitPosTmp;
			hitPosTmp = *(Vector3*)&convexResult.m_hitNormalLocal;

			//交点との距離を調べる。
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//この衝突点の方が近いので、最近傍の衝突点を更新する。
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}
		return 0.0f;
	}
};


void LineSegment::Init(const Vector3& pos)
{
	m_collider.Create(m_radius);

	//剛体を初期化
	RigidBodyInitData rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Init(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//剛体の位置を更新
	trans.setOrigin(btVector3(pos.x, pos.z, pos.z));
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_User);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
}

void LineSegment::Execute(const Vector3& pos, const Vector3 linesegment)
{
	//次の移動先となる座標を計算する。
	Vector3 nextPosition = pos;
	//速度からこのフレームでの移動量を求める。オイラー積分。
	Vector3 addPos = linesegment;
	nextPosition += addPos;
	Vector3 nextPosition2 = nextPosition;

	btRigidBody* btBody = m_rigidBody.GetBody();
	//剛体を動かす。
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//剛体の位置を更新。
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	//下方向を調べる。
	{
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心。
		start.setOrigin(btVector3(pos.x, pos.y, pos.z));
		end.setOrigin(btVector3(nextPosition.x, nextPosition.y, nextPosition.z));
		SweepResultGround callback;
		callback.me = m_rigidBody.GetBody();
		callback.startPos = *(Vector3*)&start.getOrigin();
		callback.playerRadius = m_sphere->GetRadius();
		//衝突検出。
		if (fabsf(nextPosition.y - callback.startPos.y) > FLT_EPSILON) {
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.isHit) {
				nextPosition = callback.hitPos;
			}
		}
		Vector3 vector = nextPosition2 - nextPosition;
		Vector3 pos = m_sphere->GetPosition();
		pos.y -= vector.y;
		m_sphere->SetPosition(pos - Vector3::AxisY * m_sphere->GetRadius());
	}
}