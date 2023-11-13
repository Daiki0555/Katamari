#pragma once
namespace nsK2EngineLow {
	namespace
	{
		const int POINTLIGHT_NUM = 1;
		const int SPOTLIGHT_NUM = 1;
	}

	//�f�B���N�V�������C�g�̍\����
	struct DirectionLight
	{
		Vector3 ligDirection = Vector3::Zero;		//���C�g�̕���
		float pad0 = 0.0f;
		Vector3 ligColor = Vector3::Zero;			//���C�g�̃J���[
		float pad1 = 0.0f;
		Vector3 eyePos = Vector3::Zero;				//���_�̕���
		float pad2 = 0.0f;
		Vector3 ambient = Vector3::Zero;			//����
		float pad3 = 0.0f;

	};

	/// <summary>
	/// �|�C���g���C�g�\����
	/// </summary>
	struct SPointLight
	{
		Vector3 ptPosition = Vector3::Zero;			//���W
		float pad0=0.0f;
 		Vector3 ptColor = Vector3::Zero;			//�J���[
		float ptRange = 0.0f;						//�e���͈�
	};

	//�X�|�b�g���C�g�̍\����
	struct SSpotLight
	{
		Vector3 spPosition = Vector3::Zero;			//���W
		float pad0 = 0.0f;
		Vector3 spColor = Vector3::Zero;			//�J���[
		float spRange = 0.0f;						//�e���͈�
		Vector3 spDirection = Vector3::Zero;		//����	
		float spAngle = 0.0f;						//�ˏo�p�x
	};

	//�������C�g�̍\����
	struct HemiSphereLight
	{
		Vector3 groundColor = Vector3::Zero;		//�n�ʂ̐F
		float pad0 = 0.0f;
		Vector3 skyColor = Vector3::Zero;			//�V���F
		float pad1 = 0.0f;
		Vector3 groundNormal = Vector3::Zero;		//�n�ʂ̖@��
		//float pad2 = 0.0f;
	};

	//���C�g�̍\����
	struct Light
	{
		DirectionLight		directionLight;					//�f�B���N�V�������C�g
		SPointLight			pointLight[POINTLIGHT_NUM];		//�|�C���g���C�g
		SSpotLight			spotLight[SPOTLIGHT_NUM];		//�X�|�b�g���C�g
		HemiSphereLight		hemiSphereLight;				//�������C�g
		int ptNum= POINTLIGHT_NUM;							//�|�C���g���C�g�̐�
		int spNum= SPOTLIGHT_NUM;							//�X�|�b�g���C�g�̐�
		int pad[3];
	};

	//�V�[�����C�g�N���X
	class SceneLight:public Noncopyable
	{
	public:
		SceneLight();
		~SceneLight();

		/// <summary>
		/// ������
		/// </summary>
		void Init();

		/// <summary>
		/// �V�[�����C�g���擾
		/// </summary>
		/// <returns></returns>
		Light& GetSceneLight()
		{
			return m_Sinstance->m_light;
		}

		/// <summary>
		/// �V�[�����C�g�̃C���X�^���X�̍쐬
		/// </summary>
		static void CreateSceneLightInstance()
		{
			if (m_Sinstance != nullptr)
			{
				std::abort();
			}
			m_Sinstance = new SceneLight;
			m_Sinstance->Init();
		}

		/// <summary>
		/// �V�[�����C�g�̃C���X�^���X���擾
		/// </summary>
		/// <returns></returns>
		static SceneLight* GetSceneLightClass()
		{
			if (m_Sinstance == nullptr) {
				CreateSceneLightInstance();
			}
			return m_Sinstance;
		}


		/// <summary>
		/// �f�B���N�V�������C�g�̃p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="dir">���C�g�̕���</param>
		/// <param name="color">�J���[</param>
		void SetDirectionLight(const Vector3 dir, const Vector3 color)
		{
			m_light.directionLight.ligDirection = dir;
			m_light.directionLight.ligDirection.Normalize();
			m_light.directionLight.ligColor = color;
		}

		/// <summary>
		/// ���̃p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="amb">�����̋���</param>
		void SetAmbientLight(const float amb)
		{
			m_light.directionLight.ambient.x = amb;
			m_light.directionLight.ambient.y = amb;
			m_light.directionLight.ambient.z = amb;
		}

		/// <summary>
		/// �|�C���g���C�g�̃p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="ptNum">�ԍ�</param>
		/// <param name="pos">���W</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		void SetPointLight(const int ptNum, const Vector3& pos, const Vector3& color, const float range)
		{
			m_light.pointLight[ptNum].ptPosition = pos;
			m_light.pointLight[ptNum].ptColor = color;
			m_light.pointLight[ptNum].ptRange = range;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="spNum">�ԍ�</param>
		/// <param name="pos">���W</param>
		/// <param name="color">�J���[</param>
		/// <param name="range">�e���͈�</param>
		/// <param name="dir">����</param>
		/// <param name="angle">�ˏo����</param>
		void SetSpotLight(const int spNum, const Vector3& pos, const Vector3& color, const float range, const Vector3& dir, const float angle)
		{
			m_light.spotLight[spNum].spPosition = pos;
			m_light.spotLight[spNum].spColor = color;
			m_light.spotLight[spNum].spRange = range;
			m_light.spotLight[spNum].spDirection = dir;
			m_light.spotLight[spNum].spAngle = angle;
		}

		/// <summary>
		/// �������C�g
		/// </summary>
		/// <param name="grdColor">�n�ʂ̐F</param>
		/// <param name="skyColor">�V���F</param>
		/// <param name="grdNormal">�@��</param>
		void SetHemiSphereLight(const Vector3& grdColor, const Vector3& skyColor,const Vector3& grdNormal)
		{
			m_light.hemiSphereLight.groundColor = grdColor;
			m_light.hemiSphereLight.skyColor = skyColor;
			m_light.hemiSphereLight.groundNormal = grdNormal;
		}


	private:
		Light m_light;					//���C�g�̍\����
		static SceneLight* m_Sinstance;
		//SPointLight m_pointLight;		//�|�C���g���C�g�̍\����
		//SSpotLight m_spotLight;			//�X�|�b�g���C�g�̍\����
	};

	


}
