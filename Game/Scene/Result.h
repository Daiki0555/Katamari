#pragma once
class Fade;
class Result :public IGameObject
{
public:
	//���U���g�p�̍\����
	struct ResultStruct
	{
		float	m_sphereScale = 0.0f;		//��̑傫��
		float	m_clearTime = 0.0f;			//�ڕW�B������
		int		m_objectCount = 0;			//�I�u�W�F�N�g�̐�
	};

	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���U���g�p�̍\���̂̎擾
	/// </summary>
	/// <returns></returns>
	const ResultStruct& GetResultStruct() const {
		return m_resultStruct;
	}
private:

private:
	ResultStruct			m_resultStruct;									//���U���g�p�̍\����
	Fade*					m_fade = nullptr;
	SpriteRender			m_backRender;
};

