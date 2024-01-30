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
	/// ワールド行列の計算
	/// </summary>
	void CalcMatrix();

	/// <summary>
	/// スタートモデルの取得
	/// </summary>
	const ModelRender& GetStartModel() const
	{
		return m_startRender;
	}

	/// <summary>
	/// ヒットしたかどうかの判定
	/// </summary>
	/// <returns></returns>
	const bool GetIsHit()const
	{
		return m_isWaitFadeOut;
	}

private:
	/// <summary>
	/// レベルの初期化
	/// </summary>
	void InitLevel();
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Hit();

	/// <summary>
	/// 巻き込み処理
	/// </summary>
	void Involution();

private:
	PhysicsStaticObject m_physicsObj;
	ModelRender			m_startRender;
	ModelRender			m_tileRender;
	SpriteRender		m_stickSprite[SPRITE_NUMBER];									    //ステックイラスト二つ
	SpriteRender		m_titleSprite;														//タイトルスプライト
	SpriteRender		m_fontSprite;														//文字用のスプライト	
	Player*				m_player = nullptr;
	Sphere*				m_sphere = nullptr;
	Fade*				m_fade = nullptr;																
	Vector3				m_position = Vector3::Zero;
	Vector3				m_scale = Vector3::Zero;
	Quaternion			m_rotation = Quaternion::Identity;
	Matrix				m_matInCore;														//塊（コア）を含む行列
	Matrix				m_objectWorldMatrix;												//巻き込み後のオブジェクトのワールド行列
	bool				m_isWaitFadeOut = false;											//フェード中かどうか											
};

