#include "stdafx.h"
#include "Result.h"
#include "Player/Sphere.h"
#include "Object/Object.h"
#include "Object/ObjectRender.h"
#include "Scene/Fade.h"
#include "Title.h"
namespace
{
	//塊の大きさ
	const Vector3 RESULT_KATAMRI_FONT_POS = { 400.0f,150.0f,0.0f };
	const Vector3 RESULT_KATAMRI_TEXT_POS = { 400.0f,200.0f,0.0f };
	const float	  RESULT_KATAMRI_FONT_SCALE = 2.0f;
	
	//目標達成時間
	const Vector3 RESULT_CLEAR_FONT_POS = { 400.0f,0.0f,0.0f };
	const Vector3 RESULT_CLEAR_TEXT_POS = { 400.0f,50.0f,0.0f };
	const float	  RESULT_CLEAR_FONT_SCALE = 1.0f;
	
	//巻き込んだオブジェクトの数
	const Vector3 RESULT_OBJECTNUM_FONT_POS = { 400.0f,-110.0f,0.0f };
	const Vector3 RESULT_OBJECTNUM_TEXT_POS = { 400.0f,-60.0f,0.0f };
	const float	  RESULT_OBJECTNUM_FONT_SCALE = 1.0f;
	
	//テキストの共通の値
	const Vector3 TEXT_COLOR = { 0.0f,0.0f,1.0f };				//テキストレンダー用のカラー
	const float   TEXT_SCALE = 1.0f;
	const Vector3 CAMERA_POS = { 0.0f,0.0f,-60.0f };			//カメラのポジション
	const float   ADD_SPHERE_POS = 400.0f;						//スフィアを座標を加算させる量
	const float	  SPHERE_UPDATE_TIME = 4.0f;

	//スターモデル
	const Vector3 STAR_SCALE = { 1.0f,1.0f,1.0f };

	//星の名前用の値
	const Vector3 STAR_FONT_POS = { -200.0f,-200.0f,0.0f };
	const float	  STAR_FONT_SCALE = 2.0f;

	const Vector3 RESULT_CLEAR_FONT_POS2 = { -200.0f,-300.0f,0.0f };

	//地球モデル
	const Vector3 EARTH_POS = {0.0f,-60.0f,0.0f};
	const Vector3 EARTH_SCALE = {2.0f,1.0f,2.0f};

	const float	  SPEED_DOWN = 2.0f;							//塊を上昇させる下げる
	
	const float	  DIRECTION_START_TIME = 0.5f;
}

Result::~Result()
{
	for (auto object:m_objctList) {
		DeleteGO(object);
	}
	//インスタンシング描画用のオブジェクトレンダーの削除
	for (auto object : m_objectRenderList) {
		DeleteGO(object);
	}
	//リストのクリア
	m_objctList.clear();
	m_objectRenderList.clear();
	//スカイキューブの削除
	DeleteGO(FindGO<SkyCube>("skycube"));
	//塊を削除
	DeleteGO(m_sphere);
}

bool Result::Start()
{
	//フェードイン
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	m_sphere = FindGO<Sphere>("sphere");
	m_sphere->GetSphereModel().SetPosition(Vector3::Zero);
	m_sphere->IsDraw(true);
	m_sphere->GetSphereModel().Update();
	
	//塊の大きさ
	//リザルト用の設定
	float value = GameManager::GetInstance()->GetResultStruct().GetSphereScale();
	//cmとmmに分解
	int cm = static_cast<int>(value);
	int mm = static_cast<int>((value - cm) * 10);
	wchar_t scaleText[255];
	swprintf_s(scaleText, L"%dcm %dmm", cm, mm);
	m_resultRender[m_enResult_Scale].SetText(scaleText);
	m_resultRender[m_enResult_Scale].SetPosition(RESULT_KATAMRI_FONT_POS);
	m_resultRender[m_enResult_Scale].SetColor(Vector3::One);
	m_resultRender[m_enResult_Scale].SetScale(RESULT_KATAMRI_FONT_SCALE);

	//テキストの設定
	swprintf_s(scaleText, L":大きさ");
	m_textRender[m_enResult_Scale].SetText(scaleText);
	m_textRender[m_enResult_Scale].SetPosition(RESULT_KATAMRI_TEXT_POS);
	m_textRender[m_enResult_Scale].SetColor(TEXT_COLOR);
	m_textRender[m_enResult_Scale].SetScale(TEXT_SCALE);

	//目標達成時間
	float clearTime = GameManager::GetInstance()->GetResultStruct().GetClearTime();
	int minutes = static_cast<int>(floor(clearTime / 60));
	int remainingSeconds = static_cast<int>(fmod(clearTime, 60.0f));
	wchar_t timeText[255];
	swprintf_s(timeText, L"%d分%d秒", minutes, remainingSeconds);
	m_resultRender[m_enResult_ClearTime].SetText(timeText);
	m_resultRender[m_enResult_ClearTime].SetPosition(RESULT_CLEAR_FONT_POS);
	m_resultRender[m_enResult_ClearTime].SetColor(Vector3::One);
	m_resultRender[m_enResult_ClearTime].SetScale(RESULT_CLEAR_FONT_SCALE);

	//テキストの設定
	swprintf_s(timeText, L":目標達成");
	m_textRender[m_enResult_ClearTime].SetText(timeText);
	m_textRender[m_enResult_ClearTime].SetPosition(RESULT_CLEAR_TEXT_POS);
	m_textRender[m_enResult_ClearTime].SetColor(TEXT_COLOR);
	m_textRender[m_enResult_ClearTime].SetScale(TEXT_SCALE);


	//巻き込んだオブジェクトの数
	int objectNum = GameManager::GetInstance()->GetResultStruct().GetObjectCount();
	wchar_t objectText[255];
	swprintf_s(objectText, L"%dコ", objectNum);
	m_resultRender[m_enResult_ObjectNum].SetText(objectText);
	m_resultRender[m_enResult_ObjectNum].SetPosition(RESULT_OBJECTNUM_FONT_POS);
	m_resultRender[m_enResult_ObjectNum].SetColor(Vector3::One);
	m_resultRender[m_enResult_ObjectNum].SetScale(RESULT_OBJECTNUM_FONT_SCALE);

	//テキストの設定
	swprintf_s(objectText, L":モノ");
	m_textRender[m_enResult_ObjectNum].SetText(objectText);
	m_textRender[m_enResult_ObjectNum].SetPosition(RESULT_OBJECTNUM_TEXT_POS);
	m_textRender[m_enResult_ObjectNum].SetColor(TEXT_COLOR);
	m_textRender[m_enResult_ObjectNum].SetScale(TEXT_SCALE);

	//星の名前を設定
	wchar_t starText[255];
	swprintf_s(starText, L"カタマリ星");
	m_starNameFont.SetText(starText);
	m_starNameFont.SetPosition(STAR_FONT_POS);
	m_starNameFont.SetScale(STAR_FONT_SCALE);
	m_starNameFont.SetColor(Vector3::One);


	//フラッシュ用のスプライトを初期化
	m_flashRender.Init("Assets/sprite/Result/white.DDS", 1920, 1080);
	m_flashRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,0.0f });
	m_flashRender.Update();

	//スターモデルの初期化
	m_starRender.InitDeferredRendering(
		"Assets/modelData/clear/star.tkm",
		0,
		0,
		enModelUpAxisY
	);
	m_starRender.SetScale(STAR_SCALE);
	m_starRender.Update();

	//地球モデル
	m_earthRender.InitDeferredRendering("Assets/modelData/UI/earth.tkm");
	m_earthRender.SetPosition(EARTH_POS);
	m_earthRender.SetScale(EARTH_SCALE);
	m_earthRender.Update();


	//スカイキューブの作成
	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetLuminance(1.0f);
	skyCube->SetType((EnSkyCubeType)enSkyCubeType_Night);
	skyCube->SetScale(600.0f);
	skyCube->Update();

	g_camera3D->SetTarget(m_sphere->GetSphereModel().GetPosition());
	g_camera3D->SetPosition(CAMERA_POS);
	return true;
}

void Result::Update()
{
	//演出が終わった時にAボタンが押されたら
	if (m_isDirectionEnd&&
		g_pad[0]->IsTrigger(enButtonA)) {
		m_fade->StartFadeOut();
		m_isWaitFadeOut = true;
	}

	if (m_isWaitFadeOut) {
		if (!m_fade->IsFade()) {
			//ゲームマネージャーの値を削除する
			GameManager::GetInstance()->GetResultStruct().ResultReset();
			GameManager::GetInstance()->DeleteInstance();
			//自身を消してタイトルを生成する
			NewGO<Title>(0, "title");
			DeleteGO(this);

		}
	}

	//フェードが終わったら
	if (!m_fade->IsFade()) {
		//演出を始める
		if (g_pad[0]->IsTrigger(enButtonA)&&
			!m_isDramSE) {
			GameManager::GetInstance()->GetBGM()->Stop();
			//巻き込まれSEの再生　	
			m_dramSE = NewGO<SoundSource>(0);
			m_dramSE->Init(11);
			m_dramSE->Play(false);
			m_dramSE->SetVolume(0.3f);
			m_isDramSE = true;
			m_isFontDraw = false;
			//大きさのフォントは別で使うので座標と大きさを再設定
			m_resultRender[m_enResult_Scale].SetPosition(RESULT_CLEAR_FONT_POS2);
		}
		if (!m_isStartBGM) {
			GameManager::GetInstance()->SetBGM(12);
			m_isStartBGM = true;
		}
	}

	if (!m_isFontDraw) {
		SphereUpdate();
	}

	//塊と同じ座標にする
	m_starRender.SetPosition(m_spherePos);
	m_starRender.Update();

	for (auto object : m_objctList) {
		if (object->GetIsDraw()) {
			object->CalcMatrix();
		}
	}

	//カメラの座標を少し後ろにし注視点を塊にする
	g_camera3D->SetTarget(m_sphere->GetSphereModel().GetPosition());
	g_camera3D->SetPosition(CAMERA_POS);
}

void Result::SphereUpdate()
{
	
	if (m_sphereTime < SPHERE_UPDATE_TIME) {
		m_sphereTime += g_gameTime->GetFrameDeltaTime();
		m_spherePos.y += ADD_SPHERE_POS * g_gameTime->GetFrameDeltaTime();
	}
	else {
		m_dramSE->Stop();
		//オブジェクトの削除
		for (auto object : m_objctList) {
			if (object->GetObjectState() == Object::m_enObject_Involution) {
				object->IsDraw(false);
			}
		}
		m_sphere->IsDraw(false);
		m_isFlash = true;
		flashUpdate();
	}
	m_sphere->GetSphereModel().SetPosition(m_spherePos);
	m_sphere->GetSphereModel().Update();
}

void Result::flashUpdate()
{		
	m_flashRender.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,m_alpha });
	m_flashRender.Update();
	//少しの間白いイラスト出し続けるようにする
	m_DirectionStartTime += g_gameTime->GetFrameDeltaTime();
	if (m_DirectionStartTime < DIRECTION_START_TIME) {
		return;
	}
	//α値を減少させる
	m_alpha -= g_gameTime->GetFrameDeltaTime() / SPEED_DOWN;
	m_alpha = max(m_alpha, 0.0f);
	if (m_alpha == 0.0f) {
		//演出が終わっていないなら
		if (!m_isDirectionEnd) {
			//巻き込まれSEの再生　
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(9);
			se->Play(false);
			se->SetVolume(0.2f);
		}
		m_isDirectionEnd = true;
	}
}


void Result::Render(RenderContext& rc)
{
	if (m_isFlash) {
		m_starNameFont.Draw(rc);
		m_resultRender[m_enResult_Scale].Draw(rc);
		m_flashRender.Draw(rc);
		m_starRender.Draw(rc);
		return;
	}
	if (m_isFontDraw) {
		for (int i = 0; i < m_enResult_Num; i++) {
			m_resultRender[i].Draw(rc);
			m_textRender[i].Draw(rc);
		}
	}
	m_earthRender.Draw(rc);
}