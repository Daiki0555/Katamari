#pragma once
namespace 
{
	const int FLOWER_NUMBER = 5;
}
class Sphere;
class FlowerUI : public IGameObject
{

public:
	~FlowerUI();
	bool Start();
	void Update();
	/// <summary>
	/// ���ʂ̊g��
	/// </summary>
	void CommonMagnification();

	/// <summary>
	/// �ڕW�̑傫�����ǂ����̎擾
	/// </summary>
	/// <returns></returns>
	const bool GetIsObjectiveScale() const
	{
		return m_isObjectiveScale;
	}

private:
	void Render(RenderContext& rc);
	void Rotation();
	
	/// <summary>
	/// �T�C�Y�̊g��
	/// </summary>
	void Scaling();

	/// <summary>
	/// �t�H���g�̏���
	/// </summary>
	void Font();



	struct StructFlowerUI
	{
		SpriteRender m_flowerRender;
		Vector3 m_scale = Vector3::One;			
		bool m_isUpState = true;				//�g�傷��Ȃ�true�k������Ȃ�false
	};
private:
	Sphere* m_sphere;
	StructFlowerUI m_flowerUI[FLOWER_NUMBER];
	SpriteRender m_ringRender;
	FontRender m_fontRender;
	FontRender m_objectiveRender;								//�ڕW��`�悷��

	float m_commonScale = 1.0f;									//���ʂ̊g�嗦
	
	float m_initialRadius = 0.0f;								//�������a

	Quaternion m_rotation = Quaternion::Identity;

	bool m_isObjectiveScale = false;							//�傫�����ڕW���ǂ���
};

