#pragma once
#include "GameManager.h"
namespace
{
	const int START_NUMBER = 16;
	const float	STRT_NUMBER_UPDATE = 0.1f;
}
class Fade:public IGameObject
{
public:
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�F�[�h�C�������B
	/// </summary>
	void StartFadeIn()
	{
		m_state = m_enState_FadeIn;
	}

	/// <summary>
	/// �t�F�[�h�A�E�g�����B
	/// </summary>
	void StartFadeOut()
	{
		m_state = m_enState_FadeOut;
	}

	/// <summary>
	/// �t�F�[�h�����ǂ����B
	/// </summary>
	/// <returns></returns>
	const bool IsFade() const
	{
		return m_state != m_enState_Idle;
	}

	
	/// <summary>
	///�Q�[���J�n�����ǂ����̐ݒ�
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetIsGameStart(const bool state) {
		m_isGameStart = state;
	}

private:

	/// <summary>
	/// �摜�̍X�V����
	/// </summary>
	void SpriteUpdate();


	/// <summary>
	/// �t�H���g�����X�V
	/// </summary>
	void FontUpdate();


private:
	enum EnState {
		m_enState_FadeIn,		//�t�F�[�h�C����
		m_enState_FadeOut,		//�t�F�[�h�A�E�g��
		m_enState_Idle,			//�ҋ@��
	};
	SpriteRender	m_loadingSpriteRender;						//���[�f�B���O�摜
	SpriteRender	m_backRender;								//�w�i�摜
	SpriteRender	m_startRender[START_NUMBER];				//�Q�[���J�n���̃X�v���C�g
	EnState			m_state = m_enState_Idle;
	float			m_alpha = 2.0f;								//�����x
	int				m_startSpriteNumber=0;						//�X�^�[�g���̃X�v���C�g�i���o�[
	bool			m_isGameStart=false;						//�Q�[���J�n�����ǂ���
	float			m_countUpdateTime = STRT_NUMBER_UPDATE;		//�J�E���g���X�V���鎞��
};

