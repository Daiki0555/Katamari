#pragma once
/// <summary>
/// �X�e�b�N�̏��
/// </summary>
enum EnStickState {
	m_enStick_Right,						//�E�X�e�b�N����
	m_enStick_Left,							//���X�e�b�N����
	m_enStick_Both,							//�����̃X�e�b�N���|��Ă���Ȃ�
	m_enStick_No,							//�������Ă��Ȃ��Ȃ�
	m_enStick_LYInverseRY,					//Y�����ō�+�E-�Ȃ�
	m_enStick_RYInverseLY,					//Y�����ŉE+��-�Ȃ�
	m_enStick_FullLeftYInverseRightY,		//�ő��Y������+���X�e�b�N�Ɣ��΂�-�E�X�e�b�N�ɂȂ��Ă���Ȃ�
	m_enStick_FullRightYInverseLeftY,		//�ő��Y������+�E�X�e�b�N�Ɣ��΂�-���X�e�b�N�ɂȂ��Ă���Ȃ�
};

class Stick:public IGameObject
{
public:

	~Stick();
	void Update();

	/// <summary>
	/// �X�e�b�N�̓��͏��
	/// </summary>
	/// <returns></returns>
	const EnStickState GetStickState()const
	{
		return m_stickState;
	}

	/// <summary>
	/// �E�X�e�b�N�̓��͗�
	/// </summary>
	/// <returns></returns>
	const Vector3 GetRStickValue()const
	{
		return m_Rstick;
	}

	/// <summary>
	/// ���X�e�b�N�̓��͗�
	/// </summary>
	/// <returns></returns>
	const Vector3 GetLStickValue()const
	{
		return m_Lstick;
	}



public:

private:
	/// <summary>
	/// �X�e�b�N�̏�Ԃ̑J��
	/// </summary>
	void StickState();
private:
	EnStickState m_stickState;


	Vector3 m_Lstick = Vector3::Zero;						//���X�e�b�N�̓��͗�
	Vector3 m_Rstick = Vector3::Zero;						//�E�X�e�b�N�̓��͗�
};

