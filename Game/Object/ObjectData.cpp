#include "stdafx.h"
#include "ObjectData.h"
ObjectData* ObjectData::m_instance = nullptr;
void ObjectData::Init()
{
	FILE* fp = NULL;
	
	std::string fname = "Assets/Excel/ObjectData.csv";
	std::string buf;

	// 読み込むcsvファイルを開く
	std::ifstream file(fname);
	if (!file.is_open()) {
		return;
	}
	std::string line;
	//最初の1行は飛ばす。
	std::getline(file, line);
	while (std::getline(file,line)){
		StructObjectData objectData;
		// getline関数で1行ずつ読み込む
		std::istringstream stream(line);
		//オブジェクトの名前
		std::getline(stream, buf, ',');
		objectData.m_name = buf;

		//日本語のオブジェクト名を読み込む
		std::getline(stream, buf, ',');
		objectData.m_jName = buf.c_str();

		//オブジェクトの体積を読み込む
		std::getline(stream, buf, ',');
		objectData.m_volume = std::stof(buf);
		objects.emplace_back(objectData);
	}
	
	file.close();

}