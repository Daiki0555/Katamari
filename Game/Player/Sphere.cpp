#include "stdafx.h"
#include "Sphere.h"
#include "Object/Object.h"
#include "Game.h"
#include "Stick.h"
#include "Scene/Title.h"
namespace 
{
	//タイトル
	const float		MODEL_UPPOS_RADIUS = 3.0f;							//モデルの半径と座標
	const float		ALWAYS_TITLE_SPEED = 30.0f;							//歩き時の乗算量

	const float		SPEED_DOWN = 0.99f;									//速度減速率
	const float		STANDARDSIZE = 40.0f;								//基準の塊の直径
	const float		ALWAYS_SPEED = 2.5f;								//歩き時の乗算量
	const float		ALWAYS_RUNSPEED = 30.0f;							//走り時の乗算量
	const float     MAX_SPEED = 4.5f;									//最高速度
	const float		BACK_SPEED = 2.0f;
	const float		ENTER_STICK = 0.001f;
	const float		INITIAL_RADIUS = 10.0f;								//初期半径
	const float		GRAVITY = 10.0f;
	const float		DASH_AVAILABLE_TIME = 0.2f;							//ダッシュ可能時間
	const int		DASH_AVAILABLE_COUNT = 5;							//ダッシュ可能カウント
	const int		DASH_ROTATION_COUNT = 3;
	const float		DASH_ROTATION_SPEED = 10.0f;						//ダッシュ時の回転速度
	const float		MODEL_UP = 6.0f;
	const float		ROT_SPEED = 3.0f;
	const float		COLIION_YDOWNPOS = 5.0f;
	const float     BRAKE_SPEED_DOWN = 0.3f;							//ブレーキ時のスピード減速率
	const float		BRAKE_ANGLE = 0.5f;									//ブレーキ時のアングル
	const float		BRAKE_LENGTH_SPEED = 1.5f;							//ブレーキ出来るときのスピードのベクトルの長さ
	const float		BRAKE_LENGTH_ADDSPEED = 0.8f;						//ブレーキ出来る時のステックのカメラとステックのベクトルの長さ
	const float		SPEED_ZERO_LENGTH = 1.3f;							//移動が止まるベクトルの長さ
	const float		SPEED_DOWN_TIME = 0.005f;							//スピード減速率の時間	
	

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
	m_title = FindGO<Title>("title");

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

	if (GameManager::GetInstance()->GetGameSceneState() == GameManager::m_enGameState_Title) {
		//初期半径の設定
		m_radius = MODEL_UPPOS_RADIUS;
		//キャラコンの設定
		m_charaCon.Init(
			m_radius,
			m_position
		);
	}
	else {
		//初期半径の設定
		m_radius = INITIAL_RADIUS;
		//キャラコンの設定
		m_charaCon.Init(
			m_radius,
			m_position
		);
	}

	//球の体積を求める
	m_volume = Math::PI * pow(m_radius, 3.0f) * 4 / 3;


	return true;
}

void Sphere::Update()
{
	//タイトル時の処理
	if (GameManager::GetInstance()->GetGameSceneState() == GameManager::m_enGameState_Title) {
		TitleMove();

		//キャラコンを使用して
		//プレイヤーの座標とモデルの座標を更新する
		m_charaCon.SetPosition(m_position);
		m_position = m_charaCon.Execute(
			m_moveSpeed,
			g_gameTime->GetFrameDeltaTime()
		);

		
		m_sphereRender.SetRotation(m_rotation);
		m_sphereRender.SetPosition(Vector3{ m_position.x,m_position.y + MODEL_UPPOS_RADIUS,m_position.z });
		m_sphereRender.Update();

		//前の座標を記憶する
		m_beforePosition = m_position;

		if (m_title->GetIsHit()) {
			m_title->CalcMatrix();
		}

		
	}
	else if (GameManager::GetInstance()->GetGameSceneState()==GameManager::m_enGameState_DuringGamePlay) {
		DashCount();

		Move();

		Rotation();

		DashRotation();

		//キャラコンを使用して
		//プレイヤーの座標とモデルの座標を更新する
		m_charaCon.SetPosition(m_position);
		m_position = m_charaCon.Execute(
			m_moveSpeed,
			g_gameTime->GetFrameDeltaTime()
		);

		m_sphereRender.SetRotation(m_rotation);
		m_sphereRender.SetScale(Vector3{ m_radius,m_radius,m_radius });
		m_sphereRender.SetPosition(Vector3{ m_position.x,m_position.y + MODEL_UP,m_position.z });
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
}

void Sphere::TitleMove()
{
	//カメラの前方向
	Vector3 cameraFoward = g_camera3D->GetForward();
	if (m_isTitleMove) {
		m_moveSpeed += cameraFoward * ALWAYS_TITLE_SPEED * g_gameTime->GetFrameDeltaTime();
		m_moveSpeed.z *= SPEED_DOWN;
		m_moveSpeedMultiply = ALWAYS_TITLE_SPEED * (MODEL_UPPOS_RADIUS / m_radius);
		//回転をさせる
		Quaternion rot;
		rot.SetRotationDegX(ALWAYS_SPEED);
		m_rotation.Multiply(rot);
	}
	Vector3 Rstick = m_stick->GetRStickValue();
	Vector3 Lstick = m_stick->GetLStickValue();
	//前方向にある程度倒れているなら
	if (Rstick.y > 0.5f &&
		Lstick.y > 0.5f) {
		m_isTitleMove = true;
	}
	m_position = m_moveSpeed;
}

void Sphere::Move()
{

	if (m_isReflective) {
		ReflectMove();
	}
	else {
		if (m_dashCount >= DASH_AVAILABLE_COUNT) {
			Dash();
		}
		else {

			//移動速度
			m_moveSpeedMultiply = ALWAYS_SPEED * (INITIAL_RADIUS / m_radius);
			Vector3 Rstick = m_stick->GetRStickValue();
			Vector3 Lstick = m_stick->GetLStickValue();
			m_inputStick = Rstick + Lstick;
			//ベクトルの内積を計算
			float dotProduct = Rstick.Dot(Lstick);

			//内積方角度を計算
			float angleRadians = acosf(dotProduct);
			float angleDegrees = angleRadians * (180.0f / Math::PI);
			//ブレーキ
			Brake();

			//カメラの前方向と、右方向の取得
			Vector3 cameraFoward = g_camera3D->GetForward();
			Vector3 cameraRight = g_camera3D->GetRight();

			//XZ平面での前方方向と右方向を取得
			cameraFoward.y = 0.0f;
			cameraFoward.Normalize();
			cameraRight.y = 0.0f;
			cameraRight.Normalize();

			//左ステックと歩く速度を乗算させる
			if (m_stick->GetStickState() == m_enStick_Both &&
				fabs(angleDegrees) < 85.0f) {
				m_moveSpeed += cameraFoward * m_inputStick.y * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime();
				m_moveSpeed += cameraRight * m_inputStick.x * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime();
				m_moveDownTime = 0.0f;
				m_wasMovingBackward = false;
				Vector3 speed = m_moveSpeed;
				//速さの正規化
				speed.Normalize();
				//ベクトルの内積を計算
				float dotProduct = speed.Dot(cameraFoward);
				//内積方角度を計算
				float angleRadians = acosf(dotProduct);
				float angleDegrees = angleRadians * (180.0f / Math::PI);
				//カメラの前方向と速さベクトル内積の角度が一定いないなら
				if (fabs(angleDegrees) <= 85.0f) {
					//速度の大きさが最高速度を超えているか判定する
					if (m_moveSpeed.Length() > MAX_SPEED) {
						//最高速度に制限
						m_moveSpeed.Normalize();
						m_moveSpeed = m_moveSpeed * MAX_SPEED;
					}
				}
				else {
					//違うなら最高速度を低めに判定する
					if (m_moveSpeed.Length() > BACK_SPEED) {
						m_moveSpeed.Normalize();
						m_moveSpeed = m_moveSpeed * BACK_SPEED;
					}
				}
			}
			//もし少しでも動いていて	
			//フルにどちらかのステックが逆向き倒れているなら
			else if (m_moveSpeed.Length() > SPEED_ZERO_LENGTH &&
				m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY ||
				m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
				InverseStickMove();
			}
		}

	}
		
	//普通に速度を減速させる
	m_moveSpeed.x *= SPEED_DOWN;
	m_moveSpeed.z *= SPEED_DOWN;

	Gravity();

	m_position += m_moveSpeed;
}

void Sphere::InverseStickMove() 
{
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();

	if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
		IsMoving();
		//右ステックがプラスなので左ステックの量を引く
		Vector3 stickPower = m_stick->GetLStickValue() - m_stick->GetRStickValue();
		//後退なら
		if (!m_isMovingBackward) {
			//後退しながら減速する
			m_moveSpeed -= cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed -= cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}
		//前進なら
		else if (m_isMovingBackward) {
			//前進しながら減速する
			m_moveSpeed += cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed += cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}
	}
	else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
		IsMoving();
		//右ステックがプラスなので左ステックの量を引く
		Vector3 stickPower = m_stick->GetRStickValue() - m_stick->GetLStickValue();
		if (!m_isMovingBackward) {
			m_moveSpeed -= cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed -= cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}
		else if (m_isMovingBackward) {
			m_moveSpeed += cameraFoward * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveSpeed += cameraRight * m_moveSpeedMultiply * g_gameTime->GetFrameDeltaTime() * stickPower.y;
			m_moveDownTime += SPEED_DOWN_TIME * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= (1.0f - m_moveDownTime);
		}

	}
	//速度の大きさが最高速度を超えているか判定する
	//最高速は直前の速度
	if (m_moveSpeed.Length() > m_beforeSpeedLength) {
		//最高速度に制限
		m_moveSpeed.Normalize();
		m_moveSpeed = m_moveSpeed * m_beforeSpeedLength;
	}
	
}

void Sphere::IsMoving()
{
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//速さ
	Vector3 speed = m_moveSpeed;
	//速さの正規化
	speed.Normalize();
	//ベクトルの内積を計算
	float dotProduct = speed.Dot(cameraFoward);
	if (!m_wasMovingBackward) {
		if (dotProduct > 0.0f) {
			m_isMovingBackward = true;
		}
		else {
			m_isMovingBackward = false;
		}
		m_wasMovingBackward = true;
		//直前の速度を最高速度にする
		m_beforeSpeedLength = m_moveSpeed.Length();
	}
}

void Sphere::Dash()
{
	//カメラの前方向と、右方向の取得
	Vector3 cameraFoward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	if (m_isDash) {
		m_moveSpeed += cameraFoward * ALWAYS_RUNSPEED * g_gameTime->GetFrameDeltaTime();
	}
	
	else {
		//ダッシュSEを流す
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(7);
		se->Play(false);
		se->SetVolume(1.0f);
		//ダッシュ時にある程度走らせる
		for (int i = 0; i < 200; i++) {
			m_moveSpeed += cameraFoward * ALWAYS_RUNSPEED * g_gameTime->GetFrameDeltaTime();
			m_moveSpeed *= 0.9f;
		}
		m_isDash = true;
	}
}

void Sphere::Brake()
{
	Vector3 frontXZ = g_camera3D->GetForward() * m_inputStick.y;
	Vector3 rightXZ = g_camera3D->GetRight() * m_inputStick.x;
	Vector3 addSpeed = frontXZ + rightXZ;
	addSpeed.y = 0.0f;

	//ブレーキしていないかつ、両方のステックが倒されたときに移動速度がある程度あるなら
	if (!m_isBrake &&
		addSpeed.Length()>=BRAKE_LENGTH_ADDSPEED &&
		m_moveSpeed.Length()>=BRAKE_LENGTH_SPEED&&
		m_stick->GetStickState()!= m_enStick_FullLeftYInverseRightY&&
		m_stick->GetStickState()!=m_enStick_FullRightYInverseLeftY){
		//前の座標との差分を求める
		Vector3 move = m_moveSpeed;
		move.Normalize();	
		addSpeed.Normalize();
		//移動方向とステックの向きの内積
		float angle = addSpeed.Dot(move);
		float b = fabs(acosf(angle));
		if (fabs(acosf(angle)) >= Math::PI * BRAKE_ANGLE) {
			m_moveSpeed.x *= BRAKE_SPEED_DOWN;
			m_moveSpeed.z *= BRAKE_SPEED_DOWN;
			m_isBrake = true;
			//ブレーキ音を再生
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(6);
			se->Play(false);
			se->SetVolume(0.5f);
		}
	}
	else {
		m_isBrake = false;
	}
}

void Sphere::DashCount()
{
	//入力が間に合っていないなら
	if (m_dashTimer <= 0.0f) {
		m_dashCount = 0;
		m_isDash = false;
	}

	if (m_dashCount <= DASH_AVAILABLE_COUNT) {
		if (m_isInverseStick) {
			//ステックの向きが反転したなら
			if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}
		}
		else {
			//ステックの向きが反転したなら
			if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}
		}

		if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
			//この処理が初めてなら
			if (m_dashCount == 0) {
				//trueでも加算できるようにする
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}
			m_isInverseStick = true;
		}
		else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
			//この処理が初めてなら
			if (m_dashCount == 0) {
				//falseでも加算できるようにする
				m_dashCount++;
				m_dashTimer = DASH_AVAILABLE_TIME;
			}

			m_isInverseStick = false;
		}
	}
	
	m_dashTimer -= g_gameTime->GetFrameDeltaTime();
}

void Sphere::Gravity()
{

	//もし地面に着いていないなら
	if (m_charaCon.IsOnGround() == false) {
		//落下するようにする
		m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	}
	else {
		//落下しないようにする
		m_moveSpeed.y = 0.0f;
	}
}

void Sphere::Rotation()
{
	const float rotationSpeed = ALWAYS_SPEED *(m_moveSpeedMultiply/ALWAYS_SPEED);
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
	m_vertical = Cross(Vector3::AxisY, move);
	//外積ベクトルを元に回転させるクォータニオンを求める
	Quaternion rot;
	rot.SetRotationDeg(m_vertical, length * rotationSpeed);

	//求めたクォータニオンを乗算する
	m_rotation.Multiply(m_rotation,rot);
}

void Sphere::DashRotation()
{
	if (m_dashCount >= DASH_ROTATION_COUNT) {
		//カメラの前ベクトルと縦ベクトルの外積を求める
		Vector3 vertical = Cross(Vector3::AxisY, g_camera3D->GetForward());
		Quaternion rot;
		//動いていないなら
		if (fabsf(m_moveSpeed.x) < 0.001f
			&& fabsf(m_moveSpeed.z) < 0.001f) {
			//カメラの前ベクトルと縦ベクトルの外積を軸にする
			for (int i = 0; i < 10; i++) {
				if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
					rot.SetRotationDeg(vertical, DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
				else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
					rot.SetRotationDeg(vertical, -DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
			}
		}
		else {
			//動いているなら動いている方向と縦ベクトルの外積を軸にする
			for (int i = 0; i < 10; i++) {
				if (m_stick->GetStickState() == m_enStick_FullLeftYInverseRightY) {
					rot.SetRotationDeg(m_vertical, DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
				else if (m_stick->GetStickState() == m_enStick_FullRightYInverseLeftY) {
					rot.SetRotationDeg(m_vertical, -DASH_ROTATION_SPEED);
					m_rotation.Multiply(m_rotation, rot);
				}
			}
		}	
		m_sphereRender.SetRotation(m_rotation);	
		m_sphereRender.Update();	
	}
}

void Sphere::ReflectMove()
{
	//反射開始時に反転させる
	if (m_reflectTime == REFLECTIVE_MAX_TIME) {
		//ベクトルを反転させる
		m_moveSpeed = Vector3{ -m_moveSpeed.x,m_moveSpeed.y,-m_moveSpeed.z };
	}
	m_reflectTime -= g_gameTime->GetFrameDeltaTime();
	//ステックに移動の入力があった時に
	if (m_stick->GetStickState() == m_enStick_Both) {
		//反射時間が終わってたら
		if (m_reflectTime < 0.0f) {
			m_isReflective = false;
			m_reflectTime = REFLECTIVE_MAX_TIME;
			Brake();
		}
	}
	
}

void Sphere::Render(RenderContext& rc)
{
	if (!m_isDraw) {
		return;
	}
	m_sphereRender.Draw(rc);
}

