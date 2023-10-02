#include "stdafx.h"
#include "Debug.h"
Debug::Debug()
{
	//アニメーションを読み込む
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_modelRender.InitDeferredRendering("Assets/modelData/debug/testModel.tkm", 0, 0, enModelUpAxisZ, true, false);
	m_modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotaition);

	m_fontRender.SetText(L"Aボタンでタイトルに戻る");
	m_fontRender.SetPosition(Vector3{ -100.0f, 100.0f, 0.0f });
	m_fontRender.SetColor(Vector4::White);
	m_fontRender.SetScale(1.0f);

	m_backRender.InitDeferredRendering("Assets/modelData/bg/bg.tkm", 0, 0, enModelUpAxisZ, false, true);
	m_backRender.SetPosition(Vector3::Zero);
	m_backRender.SetScale(Vector3::One);
	m_backRender.Update();

	//m_pointLight.SetPointLight(0, Vector3::Zero, Vector3{ 1.0f,0.0f,0.0f }, 500.0f);


	//m_spotLight.SetSpotLight(0, Vector3{ 0.0f,50.0f,0.0f }, Vector3{ 0.0f,1.0f,0.0f }, 500, Vector3{ -1.0f,-1.0f,1.0f }, 25);


	g_camera3D->SetPosition({ 0.0f, 200.0f, 300.0f });
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });
}

Debug::~Debug()
{

}

void Debug::Update()
{
	

	if (g_pad[0]->IsPress(enButtonDown))
	{
		pos.y -= 10.0f;
	}

	if (g_pad[0]->IsPress(enButtonUp))
	{
		pos.y += 10.0f;
	}


	if (g_pad[0]->IsPress(enButtonA))
	{
		m_rotaition.AddRotationDegY(2.0f);
	}

	if (g_pad[0]->IsPress(enButtonB))
	{
		m_position.x -= 1.0f;
	}
	m_modelRender.PlayAnimation(m_enAnimationClip_Idle, 0.5f);
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotaition);

	m_modelRender.Update();
	g_camera3D->SetPosition(pos);
}

void Debug::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_backRender.Draw(rc);
	m_fontRender.Draw(rc);

}