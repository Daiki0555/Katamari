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
	/// アニメーション
	/// </summary>
	void Animation();


private:
	//アニメーション
	enum EnAnimationClip {
		m_enAnimationClip_Idle,				//待機アニメーション
		m_enAnimationClip_Walk,				//歩きアニメーション
		m_enAnimationClip_Run,				//走りアニメーション
		m_enAnimationClip_Num,				//アニメーションの数
	};
	AnimationClip m_animationClips[m_enAnimationClip_Num];

	Player* m_player=nullptr;
	ModelRender m_modelUI;
	ModelRender m_earthRender;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;
};

