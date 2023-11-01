#pragma once
class Sphere;
class Game;
class IMove;
class Object :public IGameObject
{
public:
	enum EnObjectNameState{

	};

	//�I�u�W�F�N�g�̏��
	enum EnObjectState {
		m_enObject_NotInvolution,								//�I�u�W�F�N�g���������܂�Ă��Ȃ����
		m_enObject_Involution,									//�I�u�W�F�N�g���������܂�Ă�����
	};

	

	~Object();
	bool Start();

	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	const void SetPosition(const Vector3 pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot"></param>
	/// <returns></returns>
	const void SetRotation(const Quaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	const void SetScale(const Vector3 scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �I�u�W�F�N�g�̏�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	const EnObjectState GetObjectState() const
	{
		return m_objectState;
	}

	/// <summary>
	/// ���[���h�s��̌v�Z
	/// </summary>
	void CalcMatrix();
private:

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �I�u�W�F�N�g�̓ǂݍ��ݏ���
	/// </summary>
	void InitObject();

	/// <summary>
	/// �R���W�����̏�����
	/// </summary>
	void InitCollision();

	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Hit();

	/// <summary>
	/// �������ݏ���
	/// </summary>
	void Involution();

	

private:
	ModelRender m_objectRender;
	CollisionObject m_collisionObject;
	PhysicsStaticObject m_physicsObj;
	Sphere* m_sphere;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Matrix m_matInCore;										//��i�R�A�j���܂ލs��
	Matrix m_objectWorldMatrix;								//�������݌�̃I�u�W�F�N�g�̃��[���h�s��
	IMovePtr
	EnObjectState m_objectState = m_enObject_NotInvolution;
};

