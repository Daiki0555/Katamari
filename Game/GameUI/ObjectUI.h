#pragma once
#include "Object/ObjectData.h"
namespace 
{
	const int OBJECT_NUM_SORT=15;
}
class ObjectUI : public IGameObject
{
public:
	~ObjectUI();
	bool Start();
	void Update();

	/// <summary>
	/// ���C�v�ɕ\�����郂�f�������߂�
	/// </summary>
	/// <param name="objectData"></param>
	void InitWipeModelUI(const StructObjectData& objectData);


public:

private:
	void Render(RenderContext& rc);

	/// <summary>
	/// ���C�v�p�J�����̉�]
	/// </summary>
	void WipeCameraRotation();

	/// <summary>
	/// ���C�v��`��o���鎞��
	/// </summary>
	void WipeTimer();


	struct ObjectUIDatas
	{
		ModelRender m_objectModel;								//���f��
		FontRender m_jpName;									//���f���̓��{��̃t�H���g
		std::string m_modelName;								//���f���̖��O
		bool m_isDraw = false;									//�`�悷�邩�ǂ���
	};
private:
	Vector3 m_wipePos = Vector3::Zero;							//���C�v�̍��W
	Vector3 m_outlinePos = Vector3::Zero;						//�֊s���̍��W
	Vector3 m_cameraPos = Vector3::Zero;						//�J�����̍��W
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	//�G�N�Z���Ń��f���̎�ނ��m�F�\
	ObjectUIDatas m_objectUIDatas[OBJECT_NUM_SORT];

	float m_drawTimer = 0.0f;
};

