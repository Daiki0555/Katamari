#pragma once
class Player;
class ModelUI:public IGameObject
{
public:
	~ModelUI();
	bool Start();
	void Update();
	
	/// <summary>
	/// �`�悷�邩�ǂ���
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void IsDraw(const bool state)
	{
		m_isDraw = state;
	}

private:


	void Render(RenderContext& rc);

	/// <summary>
	/// �A�j���[�V����
	/// </summary>
	void Animation();


private:
	//�A�j���[�V����
	enum EnAnimationClip {
		m_enAnimationClip_Idle,				//�ҋ@�A�j���[�V����
		m_enAnimationClip_Walk,				//�����A�j���[�V����
		m_enAnimationClip_Run,				//����A�j���[�V����
		m_enAnimationClip_Num,				//�A�j���[�V�����̐�
	};
	AnimationClip	m_animationClips[m_enAnimationClip_Num];

	Player*			m_player=nullptr;
	ModelRender		m_modelUI;										//���f��UI�p�̃��f��
	ModelRender		m_earthRender;									//�n�����f��
	Vector3			m_position = Vector3::Zero;
	Vector3			m_scale = Vector3::One;
	Quaternion		m_rotation = Quaternion::Identity;

	bool			m_isDraw = true;
};

