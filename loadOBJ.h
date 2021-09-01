#ifndef LOADOBJ_H
#define LOADOBJ_H

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include"material.h"

struct vertex
{
	glm::vec3 position;//vertex position
	glm::vec3 normal;//vertex normal
};

struct triangle
{
	vertex TriangleVertices[3]; //triangle vertice
	MaterialData mtl;
};

class model
{
public:

	triangle face;


	std::vector<triangle> TriangleData;

	void loadObj(const char* ModelFilename, const char* MaterialFilename);

};

#endif // !LOADOBJ_H

