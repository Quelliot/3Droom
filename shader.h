#ifndef SHADER_H
#define SHADER_H

#include"room.h"

//Gouraud shading
glm::vec2 boxmax, boxmin;

struct boundary
{
	glm::vec2 max = { -std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
	glm::vec2 min = { std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };
};

boundary box;
//::vec3 lightDir = glm::vec3(1,1,0);

glm::vec3 barycentricPoint(glm::vec4 vertex1, glm::vec4 vertex2, glm::vec4 vertex3, glm::vec3 point)
{
	

	glm::vec3 COG = glm::cross(glm::vec3(vertex3.x - vertex1.x, vertex2.x - vertex1.x, vertex1.x - point.x), glm::vec3(vertex3.y - vertex1.y, vertex2.y - vertex1.y, vertex1.y - point.y));

	if (std::abs(COG.z) > 0)
	{
		return glm::vec3(1-(COG.x+COG.y)/COG.z,COG.y/COG.z,COG.x/COG.z);
	}

	return glm::vec3(-1, 1, 1);
}

void gouraudShading(glm::vec4* points, float* intensity, glm::vec3 kd)
{
	//glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(points[1] - points[0]), glm::vec3(points[2] - points[0])));
	//if ((glm::dot(normal, glm::vec3(points[0]) - camera.Position)) < 0)
	//{
		boxmin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
		boxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

		for (int i = 0; i < 3; i++) {
			//kinda redundant but cannot loop this
			//takes x and y from lowest to highest value of triangle


			boxmin.x = std::min(boxmin.x, points[i].x);
			boxmax.x = std::max(boxmax.x, points[i].x);

			boxmin.y = std::min(boxmin.y, points[i].y);
			boxmax.y = std::max(boxmax.y, points[i].y);

			boxmax.x = std::min(boxmax.x, (float)width);
			boxmax.y = std::min(boxmax.y, (float)height);

			boxmin.x = std::max(boxmin.x, 0.f);
			boxmin.y = std::max(boxmin.y, 0.f);

		}
		glm::vec3 P;
		glm::vec3 clr;
		for (P.x = boxmin.x; P.x <= boxmax.x; P.x++)
		{
			for (P.y = boxmin.y; P.y <= boxmax.y; P.y++)
			{
				glm::vec3 bc_screen = barycentricPoint(points[0], points[1], points[2], P);
				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

				float it = (bc_screen.x) * intensity[0] + (bc_screen.y) * intensity[1] + (bc_screen.z) * intensity[2];
				clr = kd + kd * it;
				clr /= 2;
				P.z = points[0].z * bc_screen.x + points[1].z * bc_screen.y + points[2].z * bc_screen.z + 100;
				putpixel(P, clr);
			}
		}
	//}
}

void PhongShading (glm::vec4* pts, triangle TriangularFace)
{

	boxmin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	boxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

	for (int i = 0; i < 3; i++) {
		boxmin.x = std::min(boxmin.x, pts[i].x);
		boxmax.x = std::max(boxmax.x, pts[i].x);

		boxmin.y = std::min(boxmin.y, pts[i].y);
		boxmax.y = std::max(boxmax.y, pts[i].y);

		boxmax.x = std::min(boxmax.x, (float)width);
		boxmax.y = std::min(boxmax.y, (float)height);

		boxmin.x = std::max(boxmin.x, 0.f);
		boxmin.y = std::max(boxmin.y, 0.f);

	}
	glm::vec3 P;
	glm::vec3 clr;
	for (P.x = boxmin.x; P.x <= boxmax.x; P.x++)
	{
		for (P.y = boxmin.y; P.y <= boxmax.y; P.y++)
		{
			glm::vec3 bc_screen = barycentricPoint(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 100;
			float ambient = 1;

			glm::vec3 n =glm::normalize( TriangularFace.TriangleVertices[0].normal * (bc_screen.x) + TriangularFace.TriangleVertices[1].normal * (bc_screen.y) + TriangularFace.TriangleVertices[2].normal * (bc_screen.z));
			

			//float diffuse = glm::dot (n , glm::normalize(glm::normalize(glm::vec3(-1000,0,0))));
			float diffuse = glm::dot(n, glm::normalize(glm::normalize(glm::vec3(pts[0])-lightPos)));

			diffuse = glm::max(diffuse,0.0f);
			//glm::vec3 ref = n * (2.0f * diffuse) - glm::normalize(glm::vec3(-1000,0,0));
			glm::vec3 ref = n * (2.0f * diffuse) - glm::normalize(glm::vec3(pts[0]) - lightPos);
			
			int alpha = 0.2;
			float specular = glm::max(std::pow(glm::dot(ref,camera.Position),alpha),0.0f);
			
			clr = (TriangularFace.mtl.kd * ambient) + ((TriangularFace.mtl.kd * diffuse)) + (TriangularFace.mtl.ks * specular);
			clr /= 3;
			clr.x = glm::max(clr.x,0.0f);
			clr.y = glm::max(clr.y, 0.0f);
			clr.z = glm::max(clr.z, 0.0f);
			putpixel(P, clr);
		}
	}
}


float minimum(float a, float b, float c)
{
	float result;
	if (a <= b && a <= c)
		result = a;
	else if (b <= a && b <= c)
		result = b;
	else
		result = c;
	return result;
}

float maximum(float a, float b, float c)
{
	float result;
	if (a >= b && a >= c)
		result = a;
	else if (b >= a && b >= c)
		result = b;
	else
		result = c;
	return result;
}

float barycentricFunction(glm::vec3 a, glm::vec3 b, float x, float y)
{
	float result;
	result = (a.y - b.y) * x + (a.x - b.x) * y + a.x * b.y - b.x * a.y;
	return result;
}

float computeAlpha(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3, glm::vec3 point)
{
	return barycentricFunction(vertex2,vertex3,point.x,point.y) / barycentricFunction(vertex2,vertex3,vertex1.x,vertex1.y);
}

float computeBeta(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3, glm::vec3 point)
{
	return barycentricFunction(vertex1, vertex3, point.x, point.y) / barycentricFunction(vertex1, vertex3, vertex2.x, vertex2.y);
}

float computeGamma(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3, glm::vec3 point)
{
	return barycentricFunction(vertex1, vertex2, point.x, point.y) / barycentricFunction(vertex1, vertex2, vertex3.x, vertex3.y);
}

/*
void rasterize(triangle TriangularFace, glm::mat4 CameraMatrix)
{
	glm::vec4 vertex1 = glm::vec4(TriangularFace.TriangleVertices[0].position, 1.0f);
	glm::vec4 vertex2 = glm::vec4(TriangularFace.TriangleVertices[1].position, 1.0f);
	glm::vec4 vertex3 = glm::vec4(TriangularFace.TriangleVertices[2].position, 1.0f);

	vertex1 = modelMatrix * CameraMatrix * vertex1;
	vertex2 = modelMatrix * CameraMatrix * vertex2;
	vertex3 = modelMatrix * CameraMatrix * vertex3;

	vertex1.x = ((vertex1.x / vertex1.w) + 1) * width / 2 - width / 2;
	vertex2.x = ((vertex2.x / vertex2.w) + 1) * width / 2 - width / 2;
	vertex2.x = ((vertex2.x / vertex2.w) + 1) * width / 2 - width / 2;

	vertex1.y = ((vertex1.y / vertex1.w) + 1) * height / 2 - height / 2;
	vertex2.y = ((vertex1.y / vertex1.w) + 1) * height / 2 - height / 2;
	vertex3.y = ((vertex1.y / vertex1.w) + 1) * height / 2 - height / 2;

	
	float xmin = minimum(vertex1.x,vertex2.x,vertex3.x);

	float ymin = minimum(vertex1.y, vertex2.y, vertex3.y);

	float xmax = maximum(vertex1.x, vertex2.x, vertex3.x);

	float ymax = maximum(vertex1.y, vertex2.y, vertex3.y);

	
	glm::vec4 point;

	for (point.x=xmin;point.x<=xmax;point.x++)
	{
		for (point.y = ymin; point.y <= ymax; point.y++)
		{
			float alpha, beta, gamma;
			alpha = computeAlpha(glm::vec3(vertex1), glm::vec3(vertex2), glm::vec3(vertex3),glm::vec3(point));
			beta = computeBeta(glm::vec3(vertex1), glm::vec3(vertex2), glm::vec3(vertex3), glm::vec3(point));
			gamma = computeGamma(glm::vec3(vertex1), glm::vec3(vertex2), glm::vec3(vertex3), glm::vec3(point));

			if (alpha < 0 && alpha > 1 || beta < 0 && beta > 1 && gamma < 0 && gamma > 1)
			{
				continue;
			}
			else
			{
				putpixel_adjusted(point,glm::vec3(0,0,1));
			}

		}
	}
}
*/
#endif // !SHADER_H
