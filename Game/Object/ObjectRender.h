#pragma once
#include "ObjectData.h"
class ObjectRender : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �I�u�W�F�N�g�̍ő吔��ݒ肷��
	/// </summary>
	/// <param name="maxObject"></param>
	void SetMaxObject(int maxObject)
	{
		m_maxObject = maxObject;
	}

	/// <summary>
	/// �C���X�^���X������
	/// </summary>
	/// <param name="instanceNo"></param>
	void RemoveInstance(int instanceNo)
	{
		m_modelRender.RemoveInstance(instanceNo);
	}

	/// <summary>
	/// �I�u�W�F�N�g�f�[�^��ݒ肷��
	/// </summary>
	/// <param name="objdata"></param>
	const void SetObjectData(const StructObjectData objdata)
	{
		m_objData = objdata;
	}

	/// <summary>
	/// �C���X�^���V���O�f�[�^�̍X�V
	/// </summary>
	/// <param name="instanceNo"></param>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	void UpdateInstancingData(
		int instanceNo,
		Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale
	) {
		m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
	}

	/// <summary>
	/// �������݌�̍X�V����
	/// </summary>
	/// <param name="matrix"></param>
	/// <param name="enModelUpAxis"></param>
	void InvolutionModelsUpdate(
		int instanceNo,
		Matrix matrix,
		EnModelUpAxis enModelUpAxis
	) {
		m_modelRender.InvolutionModelsUpdate(instanceNo,matrix, enModelUpAxis);
	}

private:
private:
	StructObjectData m_objData;
	ModelRender	m_modelRender;		// ���f�������_���[�B
	int			m_maxObject = 0;	// ���f���̐�
};

