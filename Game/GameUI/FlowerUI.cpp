#include "stdafx.h"
#include "FlowerUI.h"
#include "Player/Sphere.h"
#include "Game.h"
namespace {
	//�C���X�g�̋��ʂ̒l
	const Vector3 UI_POSITION = { -800.0f,450.0f,0.0f };
	const Vector2 UI_PIVOT = { 0.5f,0.5f };
	const float   UI_MAX_SCALE = 1.1f;							//UI�̊g�嗦�̍ő�
	const float   UI_MIN_SCALE = 1.0f;							//UI�̏k�����̍ŏ�
	const float	  COMMON_SCALE_MAX = 1.7f;						//�O������X�P�[����ύX����Ƃ��̍ő�l

	
	//�C���X�g�̌ʂ̒l
	const float	  UI1_WIDTH = 1920 / 7;
	const float	  UI1_HEIGHT = 1080 / 7;
	
	const float	  UI2_WIDTH = 1920 / 3;
	const float	  UI2_HEIGHT = 1080 / 3;

	const float	  UI3_WIDTH = 1920 / 2.5;
	const float	  UI3_HEIGHT = 1080 / 2.5;
	
	//�X�P�[���̃e�L�X�g
	const Vector3 TEXT_POSITION = { -890.0f,470.0f,0.0f };
	const Vector3 TEXT_COLOR = { 1.0f,1.0f,1.0f };
	const float   TEXT_SCALE = 1.0f;

	//�����O�C���X�g
	const Vector3 RING_POSITION = { -790.0f,450.0f,0.0f };
	const Vector3 RING_SCALE ={0.6f,0.6f,1.0f};
	const float   RING_ROT = 15.0f;

	//�ڕW�e�L�X�g�̐ݒ�
	const Vector4 OBJECTIVE_SHADOW = { 1.0f,0.0f,0.0f,1.0f };
	const Vector3 OBJECTIVE_POSITION = { -770.0f,320.0f,0.0f };
	const Vector3 OBJECTIVE_COLOR = { 1.0f,1.0f,1.0f };
	const float   OBJECTIVE_SCALE = 1.0f;
}
FlowerUI::~FlowerUI()
{

}

bool FlowerUI::Start()
{
	//��̏��������Ă���
	m_sphere = FindGO<Sphere>("sphere");
	m_game = FindGO<Game>("game");

	//�ŏ��̑傫��
	m_initialRadius= m_sphere->GetRadius();

	m_commonScale = UI_MIN_SCALE;

	//�摜�̏�����
	m_flowerUI[0].m_flowerRender.Init("Assets/sprite/flower/Kflower.DDS", UI1_WIDTH, UI1_HEIGHT);
	
	m_flowerUI[1].m_flowerRender.Init("Assets/sprite/flower/Kflower2.DDS", UI2_WIDTH, UI2_HEIGHT);
	
	m_flowerUI[2].m_flowerRender.Init("Assets/sprite/flower/Kflower3.DDS", UI2_WIDTH, UI2_HEIGHT);
	m_flowerUI[2].m_scale = { UI_MAX_SCALE ,UI_MAX_SCALE ,UI_MAX_SCALE };
	m_flowerUI[2].m_isUpState = false;

	m_flowerUI[3].m_flowerRender.Init("Assets/sprite/flower/Kflower4.DDS", UI3_WIDTH, UI3_HEIGHT);
	
	m_flowerUI[4].m_flowerRender.Init("Assets/sprite/flower/Kflower5.DDS", UI3_WIDTH, UI3_HEIGHT);
	m_flowerUI[4].m_scale = { UI_MAX_SCALE ,UI_MAX_SCALE ,UI_MAX_SCALE };
	m_flowerUI[4].m_isUpState = false;

	for (int i = 0; i < FLOWER_NUMBER; i++) {
		m_flowerUI[i].m_flowerRender.SetPosition(UI_POSITION);
		m_flowerUI[i].m_flowerRender.SetScale(m_flowerUI[i].m_scale);
		m_flowerUI[i].m_flowerRender.SetPivot(UI_PIVOT);
		m_flowerUI[i].m_flowerRender.SetRotation(m_rotation);
		m_flowerUI[i].m_flowerRender.Update();
	}

	//�����O�C���X�g�̏�����
	m_ringRender.Init("Assets/sprite/ring.DDS", 1920, 1080);
	m_ringRender.SetPosition(RING_POSITION);
	m_ringRender.SetScale(RING_SCALE);
	Quaternion rot;
	rot.SetRotationDegZ(-RING_ROT);
	m_ringRender.SetRotation(rot);
	m_ringRender.Update();

	//�e�L�X�g�̏�����
	m_fontRender.SetPosition(TEXT_POSITION);
	m_fontRender.SetColor(TEXT_COLOR);
	m_fontRender.SetScale(TEXT_SCALE);
	m_fontRender.SetShadowParam(true, 1.0f, Vector4{ 0.0f,0.0f,0.0f,0.80f });

	//�ڕW�p�̐ݒ������
	wchar_t objectiveText[255];
	//�ڕW�̑傫�����Q�[���}�l�[�W���[���玝���Ă���
	m_tagetScale = GameManager::GetInstance()->GetGameDataStruct().GetTargetSize();
	swprintf_s(objectiveText, L"%dcm", m_tagetScale);
	m_objectiveRender.SetText(objectiveText);
	m_objectiveRender.SetPosition(OBJECTIVE_POSITION);
	m_objectiveRender.SetColor(OBJECTIVE_COLOR);
	m_objectiveRender.SetScale(OBJECTIVE_SCALE);
	m_objectiveRender.SetShadowParam(true, 1.0f, OBJECTIVE_SHADOW);
	return true;
}

void FlowerUI::Update()
{
	Rotation();
	Scaling();
	Font();
	CommonMagnification();
	for (int i = 0; i < FLOWER_NUMBER; i++) {
		m_flowerUI[i].m_flowerRender.SetScale(m_flowerUI[i].m_scale* m_commonScale);
		m_flowerUI[i].m_flowerRender.Update();
	}
}

void FlowerUI::Scaling()
{
	//�摜�𓯎��ɍX�V����
	for (int i = 0; i < FLOWER_NUMBER; i++) {
		Vector3 scale = m_flowerUI[i].m_scale;
		if (m_flowerUI[i].m_isUpState) {
			scale *= 1.001f;
			scale.x = min(scale.x, UI_MAX_SCALE);
			scale.y = min(scale.y, UI_MAX_SCALE);
		}
		else {
			scale *= 0.999f;
			scale.y = max(scale.x, UI_MIN_SCALE);
			scale.x = max(scale.y, UI_MIN_SCALE);
		}
		//�g�嗦���ő�Ȃ�
		if (scale.x == UI_MAX_SCALE) {
			//�k������悤�ɂ���
			m_flowerUI[i].m_isUpState = false;
		}
		//�k���������E�Ȃ�
		else if (scale.x == UI_MIN_SCALE) {
			//�g�傷��悤�ɂ���
			m_flowerUI[i].m_isUpState = true;
		}
		//���̌v�Z�Ɏg���̂œn��
		m_flowerUI[i].m_scale = scale;
	}
	
}

void FlowerUI::Rotation()
{
	m_rotation.AddRotationDegZ(-0.5f);
	m_flowerUI[0].m_flowerRender.SetRotation(m_rotation);
}

void FlowerUI::Font()
{
	float value = m_sphere->GetRadius();

	//cm��mm�ɕ���
	int cm = static_cast<int>(value);
	int mm = static_cast<int>((value - cm) * 10);
	wchar_t scaleText[255];
	swprintf_s(scaleText, L"%dcm %dmm",cm,mm);
	m_fontRender.SetText(scaleText);

}

void FlowerUI::CommonMagnification()
{
	float scale = m_sphere->GetRadius();
	//���a���ő�l�𒴂��Ȃ��悤�ɂ���
	scale = min(scale, m_tagetScale);
	//���a�ɍ��킹�Ċg�嗦��ς���
	m_commonScale = UI_MIN_SCALE +(COMMON_SCALE_MAX- UI_MIN_SCALE)*((m_sphere->GetRadius()-m_initialRadius)/(m_tagetScale - m_initialRadius));
	m_commonScale = min(m_commonScale, COMMON_SCALE_MAX);
	if (scale == m_tagetScale) {
		m_game->SetClearState(true);
	}
}

void FlowerUI::Render(RenderContext& rc)
{
	for (int i = FLOWER_NUMBER-1; i > -1; i--){
		m_flowerUI[i].m_flowerRender.Draw(rc);
	}
	m_fontRender.Draw(rc);
	m_ringRender.Draw(rc);
	m_objectiveRender.Draw(rc);
}
