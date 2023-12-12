#pragma once
#include "physics/SphereCollider.h"
#include "physics/RigidBody.h"
namespace nsK2EngineLow {
	class CharacterSphereController
	{
	public:
		CharacterSphereController() {

		}
		~CharacterSphereController()
		{
			RemoveRigidBoby();
		}
		/*!
			* @brief	�������B
			*@param[in]	radius		�X�t�B�A�R���C�_�[�̔��a�B
			*@param[in]	position	�����ʒu�B
			*/
		void Init(float radius,const Vector3& position);
		/*!
			* @brief	���s�B
			*@param[in, out]	moveSpeed		�ړ����x�B
			*@param[in]	deltaTime		�o�ߎ��ԁB�P�ʂ͕b�B�f�t�H���g�ł́A�P�t���[���̌o�ߎ��Ԃ��n����Ă��܂��B
			*@return �ړ���̃L�����N�^�[�̍��W�B
			*/
		const Vector3& Execute(Vector3& moveSpeed, float deltaTime);
		/*!
			* @brief	���W���擾�B
			*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/*!
			* @brief	���W��ݒ�B
			*/
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}

		/*!
			* @brief	�W�����v��������
			*/
		bool IsJump() const
		{
			return m_isJump;
		}
		/*!
		* @brief	�n�ʏ�ɂ��邩����B
		*/
		bool IsOnGround() const
		{
			return m_isOnGround;
		}
		/*!
		* @brief	�R���C�_�[���擾�B
		*/
		SphereCollider* GetCollider()
		{
			return &m_collider;
		}
		/*!
		* @brief	���̂��擾�B
		*/
		RigidBody* GetRigidBody()
		{
			return &m_rigidBody;
		}

		/// <summary>
		/// ���a�̐ݒ�
		/// </summary>
		/// <param name="radius"></param>
		void SetRadius(const float& radius)
		{
			m_radius = radius;
			m_collider.SetRadius(m_radius);
		}

		/*!
		* @brief	���̂𕨗��G���W������폜�B�B
		*/
		void RemoveRigidBoby();
	private:
		bool				m_isInited = false;				//!<�������ς݁H
		Vector3 			m_position;						//!<���W�B
		bool 				m_isJump = false;				//!<�W�����v���H
		bool				m_isOnGround = true;			//!<�n�ʂ̏�ɂ���H
		SphereCollider		m_collider;						//!<�R���C�_�[�B
		float				m_radius = 0.0f;				//!<�X�t�B�A�R���C�_�[�̔��a�B
		float				m_height = 0.0f;				//!<�X�t�B�A�R���C�_�[�̍����B
		RigidBody			m_rigidBody;					//���́B
	};
}


