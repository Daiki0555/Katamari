#pragma once
namespace 
{
	const int FLOWER_NUMBER = 5;
}
class Sphere;
class Game;
#include "GameManager.h"
class FlowerUI : public IGameObject
{

public:
	~FlowerUI();
	bool Start();
	void Update();
	/// <summary>
	/// ‹¤’Ê‚ÌŠg‘å
	/// </summary>
	void CommonMagnification();

private:
	void Render(RenderContext& rc);
	void Rotation();
	
	/// <summary>
	/// ƒTƒCƒY‚ÌŠg‘å
	/// </summary>
	void Scaling();

	/// <summary>
	/// ƒtƒHƒ“ƒg‚Ìˆ—
	/// </summary>
	void Font();
	struct StructFlowerUI
	{
		SpriteRender m_flowerRender;
		Vector3 m_scale = Vector3::One;			
		bool m_isUpState = true;				//Šg‘å‚·‚é‚È‚çtruek¬‚·‚é‚È‚çfalse
	};
private:
	Sphere*			m_sphere;
	Game*			m_game;
	StructFlowerUI	m_flowerUI[FLOWER_NUMBER];
	SpriteRender	m_ringRender;
	FontRender		m_fontRender;
	FontRender		m_objectiveRender;								//–Ú•W‚ğ•`‰æ‚·‚é
	Quaternion		m_rotation = Quaternion::Identity;

	float			m_commonScale = 1.0f;							//‹¤’Ê‚ÌŠg‘å—¦
	
	float			m_initialRadius = 0.0f;							//‰Šú”¼Œa

	int				m_tagetScale = 0;								//–Ú•W‚Ì‘å‚«‚³
};

