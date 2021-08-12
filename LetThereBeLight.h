#ifndef LETTHEREBELIGHT_H
#define LETTHEREBELIGHT_H

#include"Vertex.h"
#include<glm/glm.hpp>
#include<glm/matrix.hpp>
#include<glm/ext/matrix_transform.hpp>

#include<iostream>
#include<vector>

class Light
{
public:
	glm::vec3 LightPos;
	glm::vec3 LightColor;
	glm::mat4 Model;
	glm::vec3 Normal;
	glm::vec3 LightColor;
	std::vector<Vertex>Vertices;

	Light(std::vector<Vertex> vertices, glm::vec3 lightPosition, glm::vec3 lightColor, glm::mat4 model);

	glm::vec4 PointLight();
};

#endif // !LETTHEREBELIGHT_H