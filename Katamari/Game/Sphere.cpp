#include "stdafx.h"
#include "Sphere.h"
#include "Object.h"
#include "Game.h"
#include "Stick.h"
namespace 
{
	const float		SPEED_DOWN = 0.8;									//速度減速率
	const float		STANDARDSIZE = 40.0f;								//基準の塊の直径
	const float		ALWAYS_SPEED = 10.0f;								//歩き時の乗算量
	const float		PLAYER_RUNSPEED = 10.0f;							//走り時の乗算量
	const float		ENTER_STICK = 0.001f;
	const float		INITIAL_RADIUS = 10.0f;								//初期半径
	const float		GRAVITY = 10.0f;
}
Sphere::~Sphere()
{

}

bool Sphere::Start()
{
	//それぞれのクラスの情報を持ってくる
	m_object = FindGO<Object>("object");
	m_game = FindGO<Game>("game");
	m_stick = FindGO<Stick>("stick");

	//スフィアモデルの情報の設定
	m_sphereRender.InitToonModel("Assets/modelData/sphere/sphere.tkm",
		0,
		0, 
		enModelUpAxisY, 
		true, 
		false, 
		true
	);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetScale(m_scale);
	m_sphereRender.Update();

	//初期半径の設定
	m_radius = INITIAL_RADIUS;

	//キャラコンの設定
	m_charaCon.Init(
		100.0f,
		100.0f,
		m_position
	);

	m_collisionObject.CreateSphere(
		m_position,
		m_rotation,
		m_radius
	);

	return true;
}

void Sphere::Update()
{

	Move();

	Rotation();
	
	//キャラコンを使用して
	//プレイヤーの座標とモデルの座標を更新する
	m_charaCon.SetPosition(m_position);
	m_position = m_charaCon.Execute(
		m_moveSpeed, 
		g_gameTime->GetFrameDeltaTime()
	);

	
	m_sphereRender.SetRotation(m_rotation);
	m_sphereRender.SetPosition(m_position);
	m_sphereRender.Update();

	//前の座標を記憶する
	m_beforePosition = m_position;

	//一番新しい塊のワールド行列が必要なので
	//ここで巻き込まれたオブジェクトのワールド行列を計算する
	for (int i = 0; i < m_game->GetObjectList().size(); i++) {
		if (m_game->GetObjectList()[i]->GetObjectState() ==
			Object::m_enObject_Involution) {
			m_game->GetObjectList()[i]->CalcMatrix();
		}
		
	}
}




void Sphere::Move()
{
	//速度を初期化
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;

	Vector3 Rstick = m_stick->GetRStickValue();
	Vector3 Lstick = m_stick->GetLStickValue();
	Vector3 stick = Rstick + Lstick;
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	//XZ平面での前方方向と右方向を取得
	cameraFoward.y = 0.0f;
	cameraFoward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//左ステックと歩く速度を乗算させる
	if (m_stick->GetStickState() == Stick::m_enStick_Both){
		m_moveSpeed += cameraFoward * stick.y * ALWAYS_SPEED * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed += cameraRight * stick.x * ALWAYS_SPEED * g_gameTime->GetFrameDeltaTime();
	}

	//もし地面に着いていないなら
	if (m_charaCon.IsOnGround() == false){
		//落下するようにする
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}
	else {
		//落下しないようにする
		m_moveSpeed.y = 0.0f;
	}

	m_position += m_moveSpeed;
}

void Sphere::Rotation()
{
	const float rotationSpeed = 3.0f;//=1.0f*(m_protMoveSpeedMultiply/ m_moveSpeedMultiply);
	//前の座標との差分を求める
	Vector3 move = m_position - m_beforePosition;
	move.y = 0.0f;

	//動いていないなら
	if (fabsf(move.x) < 0.001f
		&& fabsf(move.z) < 0.001f) {
		return;
	}
	
	//進行方向のベクトルの長さを求める
	float length = move.Length();

	//ノーマライズして移動方向の方向ベクトルを求める
	move.Normalize();

	//上記の外積を求める
	Vector3 vertical = Cross(Vector3::AxisY, move);
	//外積ベクトルを元に回転させるクォータニオンを求める
	Quaternion rot;
	rot.SetRotationDeg(vertical, length * rotationSpeed);

	//求めたクォータニオンを乗算する
	m_rotation.Multiply(m_rotation,rot);
}

void Sphere::Render(RenderContext& rc)
{
	m_sphereRender.Draw(rc);
}

