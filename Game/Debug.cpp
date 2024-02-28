#include "stdafx.h"
#include "Debug.h"
Debug::Debug()
{
	//アニメーションを読み込む
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	Quaternion rot;
	rot.SetRotationDegY(180);	rot.SetRotationDegY(180);
	//m_modelRender.InitDeferredRendering("Assets/modelData/debug/testModel.tkm",0,0,enModelUpAxisZ,true,false);
	m_modelRender.InitToonModel("Assets/modelData/player/unityChan.tkm",
		m_animationClips,
		m_enAnimationClip_Num, enModelUpAxisY, true, false);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(rot);
	m_modelRender.Update();
	
	m_g_modelRender.InitDeferredRendering("Assets/modelData/player/unityChan.tkm",
		m_animationClips,
		m_enAnimationClip_Num, enModelUpAxisY, true, false); 
	m_g_modelRender.SetPosition(Vector3{200.0f,0.0f,0.0f});
	m_g_modelRender.SetScale(m_scale);
	

	m_g_modelRender.SetRotation(rot);
	m_g_modelRender.Update();

	m_fontRender.SetText(L"Aボタンでタイトルに戻る");
	m_fontRender.SetPosition(Vector3{ -100.0f, 100.0f, 0.0f });
	m_fontRender.SetColor(Vector4::White);
	m_fontRender.SetScale(1.0f);

	//m_backRender.InitDeferredRendering("Assets/modelData/stage/Stage.tkm", 0, 0, enModelUpAxisZ, false, true);
	m_backRender.InitToonModel("Assets/modelData/stage/Stage.tkm", 0, 0, enModelUpAxisZ, false, true);
	m_backRender.SetPosition(Vector3::Zero);
	m_backRender.SetScale(Vector3{5.0f,5.0f,5.0f});
	m_backRender.Update();

	m_skycube = NewGO<SkyCube>(0, "skycube");
	m_skycube->SetLuminance(1.0f);
	m_skycube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	m_skycube->SetScale(500.0f);
	m_skycube->Update();

	g_camera3D->SetPosition({ 0.0f, 0.0f, 350.0f });
	g_camera3D->SetTarget({ 0.0f, 200.0f, 0.0f });
	g_camera3D->SetFar(10000.0f);
}

Debug::~Debug()
{

}

void Debug::Update()
{
	

	if (g_pad[0]->IsPress(enButtonDown))
	{
		m_position.x -= 10.0f;
	}

	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		m_position.x += 10.0f;
	}


	if (g_pad[0]->IsPress(enButtonA))
	{
		m_rotaition.AddRotationDegY(2.0f);
	}

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_position.x -= 1.0f;
	}
	//m_modelRender.PlayAnimation(m_enAnimationClip_Idle, 0.5f);
	m_modelRender.SetPosition(m_position);
	//m_modelRender.SetRotation(m_rotaition);

	m_modelRender.Update();
	m_g_modelRender.Update();

	g_camera3D->SetPosition(pos);
}

void Debug::Render(RenderContext& rc)
{
//	m_modelRender.Draw(rc);
//	m_g_modelRender.Draw(rc);
	m_backRender.Draw(rc);

}