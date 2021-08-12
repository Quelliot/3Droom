#include"LetThereBeLight.h"


Light::Light(std::vector<Vertex> vertices, glm::vec3 lightPosition, glm::vec3 lightColor, glm::mat4 model)
{
	 LightPos = lightPosition;
	 LightColor = lightColor ;
	 Model = model ;
	 Vertices = vertices;
}

glm::vec4 Light::PointLight()
{
	glm::vec3 crntPos;

	crntPos = glm::vec3();
}