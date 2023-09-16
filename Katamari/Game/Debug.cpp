#include "stdafx.h"
#include "Debug.h"
Debug::Debug()
{
	//アニメーションを読み込む
	m_animationClips[m_enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[m_enAnimationClip_Idle].SetLoopFlag(true);
	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, m_enAnimationClip_Num, enModelUpAxisY, false, false);
	m_modelRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotaition);

	m_fontRender.SetText(L"Aボタンでタイトルに戻る");
	m_fontRender.SetPosition(Vector3{ -100.0f, 100.0f, 0.0f });
	m_fontRender.SetColor(Vector4::White);
	m_fontRender.SetScale(1.0f);
	

	g_camera3D->SetPosition({ 0.0f, 200.0f, 300.0f });
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });
}

Debug::~Debug()
{

}

void Debug::Update()
{
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
}

void Debug::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_fontRender.Draw(rc);
}