#include "stdafx.h"
#include "Title.h"
#include "Player/Player.h"
#include "Player/Sphere.h"
#include "Stick.h"
#include "GameCamera.h"
#include "Fade.h"
#include "Game.h"
namespace {
	const Vector3 START_POSITION = { 0.0f,0.0f,0.0f };
}
Title::~Title()
{
	//��̍폜
	DeleteGO(m_sphere);
	//�X�e�B�b�N�̍폜
	DeleteGO(FindGO<Stick>("stick"));
}

bool Title::Start()
{
	GameManager::GetInstance()->SetGameSceneState(GameManager::m_enGameState_Title);
	NewGO<Stick>(0, "stick");
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	InitLevel();
	
	return true;
}

void Title::InitLevel()
{
	LevelRender LevelRender;
	LevelRender.Init("Assets/modelData/level/title.tkl", [&](LevelObjecData& objdata) {
		if (objdata.ForwardMatchName(L"katamari")) {
			//��(�R�A)�̍쐬
			m_sphere = NewGO<Sphere>(0, "sphere");
			m_sphere->SetPosition(objdata.position);
			m_sphere->SetScale(objdata.scale);
			m_sphere->SetRotation(objdata.rotation);
			//��𒍎��_�Ƃ���
			Vector3 pos = objdata.position;
			g_camera3D->SetPosition(Vector3{ pos.x,pos.y,pos.z - 50.0f });
			g_camera3D->SetTarget(Vector3{ pos.x,pos.y,pos.z});
			g_camera3D->Update();
			return true;
		}
		else if (objdata.ForwardMatchName(L"unityChan")) {
			////�v���C���[�̍쐬
			//m_player = NewGO<Player>(0, "player");
			//m_player->SetPosition(objdata.position);
			//m_player->SetScale(objdata.scale);
			//m_player->SetRotation(objdata.rotation);
			return true;
		}
		//�X�^�[�g���f��
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
		//�^�C��
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
	if (m_isWaitFadeOut) {
		if (!m_fade->IsFade()) {
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
	//if (diff.Length() <= m_sphere->GetRadius())
	if(g_pad[0]->IsTrigger(enButtonA))
	{
		Involution();
		m_fade->StartFadeOut();
		m_isWaitFadeOut = true;
		m_fade->IsGameStart(true);
	}
}

void Title::Involution()
{

	//��i�R�A�j�̋t�s������߂�
	Matrix inverseMatrix;
	inverseMatrix.Inverse(m_sphere->GetSphereModel().GetModel().GetWorldMatrix());

	//�I�u�W�F�N�g�̃��[���h�s��ƃv���C���[�̋t�s�����Z���āA
	//�v���C���[����Ƃ����I�u�W�F�N�g�̃��[�J���s������߂�
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	Matrix mBias, mWorld;
	mBias.MakeRotationX(Math::PI * -0.5f);
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(m_position);
	mRot.MakeRotationFromQuaternion(m_rotation);
	mRot.Multiply(mBias, mRot);
	mScale.MakeScaling(m_scale);
	mWorld = mScale * mRot * mTrans;
	//��i�R�A�j���܂ރI�u�W�F�N�g�̃��[���h�s��Ɖ�̋t�s����v�Z����
	m_matInCore.Multiply(mWorld, inverseMatrix);

	
}

void Title::CalcMatrix()
{
	//�������݌�̃I�u�W�F�N�g�̃��[���h�s��̌v�Z
	m_objectWorldMatrix.Multiply(m_matInCore, m_sphere->GetSphereModel().GetModel().GetWorldMatrix());
	m_startRender.InvolutionTitleModelsUpdate(m_objectWorldMatrix);
}


void Title::Render(RenderContext& rc)
{
	m_startRender.Draw(rc);
	m_tileRender.Draw(rc);
}