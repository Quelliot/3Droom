#ifndef VERTEX_CLASS_H
#define VERTEX_CLASS_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

struct Vertex
{
	glm::vec4 Position;
	glm::vec3 Color;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};

class faces
{
public:
	glm::vec4 vertex1;
	glm::vec4 vertex2;
	glm::vec4 vertex3;

	
	void initialize(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
	{
	vertex1 = v1;
	vertex2 = v2;
	vertex3 = v3;
	}

	glm::vec3 centroid()
	{
		glm::vec3 temp;
		temp.x = ( vertex1.x + vertex2.x + vertex3.x ) / 3;
		temp.y = ( vertex1.y + vertex2.y + vertex3.y) / 3;
		temp.x = ( vertex1.z + vertex2.z + vertex3.z) / 3;
		return temp;

	}
};



#endif // !VERTEX_CLASS_H

