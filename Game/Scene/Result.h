#pragma once
#include "GameManager.h"
class Fade;
class Sphere;
class Object;
class ObjectRender;
class Result :public IGameObject
{
public:
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �I�u�W�F�g���X�g�̐ݒ�
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetObjectList(const std::vector<Object*> list)
	{
		m_objctList = list;
	}

	/// <summary>
	/// �I�u�W�F�g�����_�[���X�g�̐ݒ�
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetObjectRenderList(const std::vector<ObjectRender*> list)
	{
		m_objectRenderList = list;
	}

private:
	/// <summary>
	/// ���U���g�̎��
	/// </summary>
	enum EnResult
	{
		m_enResult_Scale,						//�傫��
		m_enResult_ClearTime,					//�ڕW�B������
		m_enResult_ObjectNum,					//�I�u�W�F�N�g�̐�
		m_enResult_Num							//���U���g�̐�
	};

	/// <summary>
	/// �t���b�V���̍X�V����
	/// </summary>
	void flashUpdate();

	/// <summary>
	/// �X�t�B�A�̉�
	/// </summary>
	void SphereUpdate();
private:
	Fade*								m_fade = nullptr;
	Sphere*								m_sphere = nullptr;
	FontRender							m_textRender[m_enResult_Num];
	FontRender							m_resultRender[m_enResult_Num];
	SpriteRender						m_flashRender;
	ModelRender							m_starRender;														//�X�^�[���f��
	ModelRender							m_earthRender;														//�n�����f��
	Vector3								m_spherePos = Vector3::Zero;
	float								m_sphereTime = 0.0f;
	float								m_DirectionStartTime = 0.0f;										//���o���n�߂鎞��
	float								m_alpha = 1.0f;
	int									m_fontNumber = 1;
	bool								m_isFontDraw = true;												//�t�H���g��`�悷�邩�ǂ���
	bool								m_isFlash = false;													//���点�邩�ǂ���
	bool								m_isDirectionEnd = false;											//���o���I��������ǂ���
	bool								m_isWaitFadeOut = false;											//�t�F�[�h�����ǂ���
	bool								m_isStartBGM = false;												//BGM���n�܂������ǂ���
	std::vector<Object*>				m_objctList;
	std::vector<ObjectRender*>			m_objectRenderList;
};

