#pragma once
enum EnMoveState {
	m_enMove_LR,				//���E�Ɉړ�����
	m_enMove_FB,				//�O��Ɉړ�����
	m_enMove_UD,				//�㉺�Ɉړ�����
	m_enMove_Rot,				//��]
	m_enMove_Rand,				//�����_���ړ�
	m_enMove_Chase,				//�ǂ�������
	m_enMove_Away,				//������
	m_enMove_No,				//�ړ����Ȃ�
};
/// <summary>
/// �I�u�W�F�N�g�ړ��̊��N���X
/// </summary>
class IMove : public IGameObject
{
public:
	virtual ~IMove(){};

	virtual void Init(const Vector3& pos,const Quaternion& rot){};

	/// <summary>
	/// �ړ�����
	/// </summary>
	virtual const Vector3& Move() { return Vector3::Zero; };

	
	/// <summary>
	/// �ړ��X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	const EnMoveState GetMoveState()const
	{
		return m_moveState;
	}

	/// <summary>
	/// �ړ��X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="state"></param>
	void SetMoveState(EnMoveState state)
	{
		m_moveState = state;
	}

protected:
	Vector3 m_position = Vector3::Zero;
	float m_moveSpeed = 0.0f;

	Quaternion m_rotation = Quaternion::Identity;
private:
	EnMoveState m_moveState;					//�ړ��̃X�e�[�g

};
using IMovePtr = std::unique_ptr<IMove>;
