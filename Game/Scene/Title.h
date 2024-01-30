#pragma once
namespace 
{
	const int SPRITE_NUMBER = 2;
}

#include "GameManager.h"
class Player;
class Sphere;
class Fade;
class Title:public IGameObject
{
public:
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// ���[���h�s��̌v�Z
	/// </summary>
	void CalcMatrix();

	/// <summary>
	/// �X�^�[�g���f���̎擾
	/// </summary>
	const ModelRender& GetStartModel() const
	{
		return m_startRender;
	}

	/// <summary>
	/// �q�b�g�������ǂ����̔���
	/// </summary>
	/// <returns></returns>
	const bool GetIsHit()const
	{
		return m_isWaitFadeOut;
	}

private:
	/// <summary>
	/// ���x���̏�����
	/// </summary>
	void InitLevel();
	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Hit();

	/// <summary>
	/// �������ݏ���
	/// </summary>
	void Involution();

private:
	PhysicsStaticObject m_physicsObj;
	ModelRender			m_startRender;
	ModelRender			m_tileRender;
	SpriteRender		m_stickSprite[SPRITE_NUMBER];									    //�X�e�b�N�C���X�g���
	SpriteRender		m_titleSprite;														//�^�C�g���X�v���C�g
	SpriteRender		m_fontSprite;														//�����p�̃X�v���C�g	
	Player*				m_player = nullptr;
	Sphere*				m_sphere = nullptr;
	Fade*				m_fade = nullptr;																
	Vector3				m_position = Vector3::Zero;
	Vector3				m_scale = Vector3::Zero;
	Quaternion			m_rotation = Quaternion::Identity;
	Matrix				m_matInCore;														//��i�R�A�j���܂ލs��
	Matrix				m_objectWorldMatrix;												//�������݌�̃I�u�W�F�N�g�̃��[���h�s��
	bool				m_isWaitFadeOut = false;											//�t�F�[�h�����ǂ���											
};

