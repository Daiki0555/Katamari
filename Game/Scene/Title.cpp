#include "stdafx.h"
#include "Title.h"
#include "Player/Player.h"
#include "Player/Sphere.h"
#include "Stick.h"
#include "GameCamera.h"
#include "Fade.h"
#include "Game.h"
namespace 
{
	
	const Vector3 RIGTH_STICK_SPRITE_POS = {-20.0f,-300.0f,0.0f};
	const Vector3 LEFT_STICK_SPRITE_POS = { -140.0f,-300.0f,0.0f };
	const Vector3 TITLE_SPRITE_POS = { 0.0f,350.0f,0.0f };
	const Vector3 SPRITE_SCALE = { 0.3f,0.3f,0.3f };
	const Vector3 FONT_SPRITE_POS = { 200.0f,-300.0f,0.0f };
	const Vector3 FONT_SCALE = { 0.2f,0.2f,0.2f };
	const float	  SPRITE_ALPHA = 0.8f;						//画像のα値
}
Title::~Title()
{
	//塊の削除
	DeleteGO(m_sphere);
	//スティックの削除
	DeleteGO(FindGO<Stick>("stick"));
}

bool Title::Start()
{
	//フェードイン
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	GameManager::GetInstance()->SetGameSceneState(GameManager::m_enGameState_Title);
	NewGO<Stick>(0, "stick");
	InitLevel();

	m_stickSprite[0].SetPosition(RIGTH_STICK_SPRITE_POS);
	m_stickSprite[1].SetPosition(LEFT_STICK_SPRITE_POS);
	//スプライトの初期化
	for (int i = 0; i < SPRITE_NUMBER; i++) {
		m_stickSprite[i].Init("Assets/sprite/Title/stick.DDS", 1920, 1080);
		m_stickSprite[i].SetMulColor(Vector4{ 1.0f,1.0f,1.0f,SPRITE_ALPHA });
		m_stickSprite[i].SetScale(SPRITE_SCALE);
		m_stickSprite[i].Update();
	}

	//タイトルスプライトの初期化
	m_titleSprite.Init("Assets/sprite/Title/katamari.DDS", 1920, 1080);
	m_titleSprite.SetPosition(TITLE_SPRITE_POS);
	m_titleSprite.Update();

	//フォントスプライトの初期化
	m_fontSprite.Init("Assets/sprite/Title/titleStart.DDS", 1920, 1080);
	m_fontSprite.SetPosition(FONT_SPRITE_POS);
	m_fontSprite.SetScale(FONT_SCALE);
	m_fontSprite.Update();

	//スカイキューブの作成
	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetLuminance(1.0f);
	skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	skyCube->SetScale(600.0f);
	skyCube->Update();

	
	return true;
}

void Title::InitLevel()
{
	LevelRender LevelRender;
	LevelRender.Init("Assets/modelData/level/title.tkl", [&](LevelObjecData& objdata) {
		if (objdata.ForwardMatchName(L"katamari")) {
			//塊(コア)の作成
			m_sphere = NewGO<Sphere>(0, "sphere");
			m_sphere->SetPosition(objdata.position);
			m_sphere->SetScale(objdata.scale);
			m_sphere->SetRotation(objdata.rotation);
			//塊を注視点とする
			Vector3 pos = objdata.position;
			g_camera3D->SetPosition(Vector3{ pos.x,pos.y,pos.z - 50.0f });
			g_camera3D->SetTarget(Vector3{ pos.x,pos.y,pos.z});
			g_camera3D->Update();
			return true;
		}
		else if (objdata.ForwardMatchName(L"unityChan")) {
			////プレイヤーの作成
			//m_player = NewGO<Player>(0, "player");
			//m_player->SetPosition(objdata.position);
			//m_player->SetScale(objdata.scale);
			//m_player->SetRotation(objdata.rotation);
			return true;
		}
		//スタートモデル
		else if (objdata.ForwardMatchName(L"START")) {
			m_startRender.InitToonModel(
				"Assets/modelData/Title/START.tkm",
				0,
				0,
				enModelUpAxisZ,
				true
			);
			m_startRender.SetPosition(objdata.position);
			m_startRender.SetScale(objdata.scale);
			m_startRender.SetRotation(objdata.rotation);
			m_startRender.Update();
			m_position = objdata.position;
			m_scale = objdata.scale;
			m_rotation = objdata.rotation;
		}
		//タイル
		else if (objdata.ForwardMatchName(L"tail")) {
			m_tileRender.InitDeferredRendering(
				"Assets/modelData/Title/Tile.tkm",
				0,
				0,
				enModelUpAxisZ,
				false,
				true
			);
			m_tileRender.SetPosition(objdata.position);
			m_tileRender.SetScale(objdata.scale);
			m_tileRender.SetRotation(objdata.rotation);
			m_tileRender.Update();
			m_physicsObj.CreateFromModel(
				m_tileRender.GetModel(),
				m_tileRender.GetModel().GetWorldMatrix(),
				1.0f
			);
			
		}
		});
}

void Title::Update()
{
	if (!m_fade->IsFade()) {
		if (!m_isStartBGM) {
			GameManager::GetInstance()->SetBGM(13);
			m_isStartBGM = true;
		}
	}

	if (m_isWaitFadeOut) {
		if (!m_fade->IsFade()) {
			GameManager::GetInstance()->GetBGM()->Stop();
			NewGO<Game>(0, "game");
			DeleteGO(this);
			
		}
	}
	else {
		Hit();
	}
}

void Title::Hit()
{
	Vector3 pos = m_sphere->GetPosition();
	Vector3 diff = pos - m_position;
	float hoge = diff.Length();
	//塊とプレイヤーモデルの距離が塊の半径以下なら
	if (diff.Length() <= m_sphere->GetRadius())
	{
		//巻き込み処理
		Involution();
		//フェード処理
		m_fade->StartFadeOut();
		m_isWaitFadeOut = true;
		m_fade->IsGameStart(true);
	}
}

void Title::Involution()
{

	//塊（コア）の逆行列を求める
	Matrix inverseMatrix;
	inverseMatrix.Inverse(m_sphere->GetSphereModel().GetModel().GetWorldMatrix());

	//オブジェクトのワールド行列とプレイヤーの逆行列を乗算して、
	//プレイヤーを基準としたオブジェクトのローカル行列を求める
	//3dsMaxと軸を合わせるためのバイアス。
	Matrix mBias, mWorld;
	mBias.MakeRotationX(Math::PI * -0.5f);
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	mRot.Multiply(mBias, mRot);
	mScale.MakeScaling(m_scale);
	mWorld = mScale * mRot * mTrans;
	//塊（コア）を含むオブジェクトのワールド行列と塊の逆行列を計算する
	m_matInCore.Multiply(mWorld, inverseMatrix);

	
}

void Title::CalcMatrix()
{
	//巻き込み後のオブジェクトのワールド行列の計算
	m_objectWorldMatrix.Multiply(m_matInCore, m_sphere->GetSphereModel().GetModel().GetWorldMatrix());
	m_startRender.InvolutionTitleModelsUpdate(m_objectWorldMatrix);
}


void Title::Render(RenderContext& rc)
{
	m_startRender.Draw(rc);
	m_tileRender.Draw(rc);
	for (int i = 0; i < SPRITE_NUMBER; i++) {
		m_stickSprite[i].Draw(rc);
	}
	m_titleSprite.Draw(rc);
	m_fontSprite.Draw(rc);
}