#pragma once
class Player;
class ModelUI:public IGameObject
{
public:
	~ModelUI();
	bool Start();
	void Update();
public:

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
	AnimationClip m_animationClips[m_enAnimationClip_Num];

	Player* m_player=nullptr;
	ModelRender m_modelUI;
	ModelRender m_earthRender;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
};

