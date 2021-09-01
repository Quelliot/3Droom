#ifndef MATERIAL_H
#define MATERIAL_H

#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

#include<glm/glm.hpp>



struct MaterialData
{
	int materialNumber = 0;

	float Ns = 0;//shininess
	glm::vec3 ka = glm::vec3(0, 0, 0); //ambient color 
	glm::vec3 kd = glm::vec3(0, 0, 0); //diffuse color
	glm::vec3 ks = glm::vec3(0, 0, 0); //specular color
};


class MaterialParse
{
public:
	std::vector<MaterialData> Materials;//vector array to store the material data
	void LoadMaterialFile(std::string Filename);//loading a material
	//int nmaterial();//no.of material
	MaterialData LoadMaterial(int materialNumber);
};

#endif // !MATERIAL_H
