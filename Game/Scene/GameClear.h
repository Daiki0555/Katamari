#pragma once
class Player;
class Sphere;
class ModelUI;
class GameCamera;
class GameClear:public IGameObject
{
public:
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �N���A���o���J�n���邩�ǂ���
	/// </summary>
	/// <param name="state"></param>
	/// <returns></returns>
	const void IsClearStart(const bool state)
	{
		m_isStart = state;
	}

	/// <summary>
	/// �I��肩�ǂ���
	/// </summary>
	/// <returns></returns>
	const bool GetIsEndState()const 
	{
		return m_isEnd;
	}

private:

	/// <summary>
	/// �傫���̕ύX
	/// </summary>
	void ScaleChange();
private:
	Player* m_player = nullptr;
	Sphere* m_sphere = nullptr;
	ModelUI* m_modelUI = nullptr;
	GameCamera* m_gameCamera = nullptr;


	SpriteRender m_rainbowRender;
	SpriteRender m_kingRender;
	SpriteRender m_smokeRender;
	Vector3 m_scale = { 0.02f,0.02f,1.0f };
	float m_timeScale = 1.0f;
	float m_stopTime = 0.5f;
	bool m_isStart = false;
	bool m_isScaleDown = false;
	bool m_isEnd = false;
};

