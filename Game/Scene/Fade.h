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
	/// フェードイン処理。
	/// </summary>
	void StartFadeIn()
	{
		m_state = m_enState_FadeIn;
	}

	/// <summary>
	/// フェードアウト処理。
	/// </summary>
	void StartFadeOut()
	{
		m_state = m_enState_FadeOut;
	}

	/// <summary>
	/// フェード中かどうか。
	/// </summary>
	/// <returns></returns>
	const bool IsFade() const
	{
		return m_state != m_enState_Idle;
	}

	
	/// <summary>
	///ゲーム開始時かどうかの設定
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void SetIsGameStart(const bool state) {
		m_isGameStart = state;
	}

private:

	/// <summary>
	/// 画像の更新処理
	/// </summary>
	void SpriteUpdate();


	/// <summary>
	/// フォント処理更新
	/// </summary>
	void FontUpdate();


private:
	enum EnState {
		m_enState_FadeIn,		//フェードイン中
		m_enState_FadeOut,		//フェードアウト中
		m_enState_Idle,			//待機中
	};
	SpriteRender	m_loadingSpriteRender;						//ローディング画像
	SpriteRender	m_backRender;								//背景画像
	SpriteRender	m_startRender[START_NUMBER];				//ゲーム開始時のスプライト
	EnState			m_state = m_enState_Idle;
	float			m_alpha = 2.0f;								//透明度
	int				m_startSpriteNumber=0;						//スタート時のスプライトナンバー
	bool			m_isGameStart=false;						//ゲーム開始時かどうか
	float			m_countUpdateTime = STRT_NUMBER_UPDATE;		//カウントを更新する時間
};

