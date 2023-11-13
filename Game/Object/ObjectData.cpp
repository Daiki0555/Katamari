#include "stdafx.h"
#include "ObjectData.h"
ObjectData* ObjectData::m_instance = nullptr;
void ObjectData::Init()
{
	FILE* fp = NULL;
	
	std::string fname = "Assets/Excel/ObjectData.csv";
	std::string buf;

	// �ǂݍ���csv�t�@�C�����J��
	std::ifstream file(fname);
	if (!file.is_open()) {
		return;
	}
	std::string line;
	//�ŏ���1�s�͔�΂��B
	std::getline(file, line);
	while (std::getline(file,line)){
		StructObjectData objectData;
		// getline�֐���1�s���ǂݍ���
		std::istringstream stream(line);
		//�I�u�W�F�N�g�̖��O
		std::getline(stream, buf, ',');
		objectData.m_name = buf;

		//���{��̃I�u�W�F�N�g����ǂݍ���
		std::getline(stream, buf, ',');
		objectData.m_jName = buf.c_str();

		//�I�u�W�F�N�g�̑̐ς�ǂݍ���
		std::getline(stream, buf, ',');
		objectData.m_volume = std::stof(buf);
		objects.emplace_back(objectData);
	}
	
	file.close();

}