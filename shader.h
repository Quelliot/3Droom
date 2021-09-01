#ifndef SHADER_H
#define SHADER_H

#include"room.h"

//Gouraud shading
glm::vec2 bboxmax, bboxmin;

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

void gouraudShading(glm::vec4* pts, float* intensity, glm::vec3 kd)
{
	bboxmin = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	bboxmax = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

	for (int i = 0; i < 3; i++) {
		//kinda redundant but cannot loop this
		//takes x and y from lowest to highest value of triangle
		

		bboxmin.x = std::min(bboxmin.x, pts[i].x);
		bboxmax.x = std::max(bboxmax.x, pts[i].x);

		bboxmin.y = std::min(bboxmin.y, pts[i].y);
		bboxmax.y = std::max(bboxmax.y, pts[i].y);

		bboxmax.x = std::min(bboxmax.x, (float)width);
		bboxmax.y = std::min(bboxmax.y, (float)height);

		bboxmin.x = std::max(bboxmin.x, 0.f);
		bboxmin.y = std::max(bboxmin.y, 0.f);

	}
	glm::vec3 P;
	glm::vec3 clr;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
		{
			glm::vec3 bc_screen = barycentricPoint(pts[0], pts[1], pts[2], P);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;

			float it = (bc_screen.x) * intensity[0] + (bc_screen.y) * intensity[1] + (bc_screen.z) * intensity[2];
			clr = kd + kd * it;
			clr /= 2;
			P.z = pts[0].z * bc_screen.x + pts[1].z * bc_screen.y + pts[2].z * bc_screen.z + 100;
			putpixel(P, clr);
		}
	}
}


void gouraudShading(triangle TriangularFace, glm::mat4 CameraMatrix)
{
	glm::vec4 vertex[3];
	for(int i = 0; i<3;i++)
	{
		 vertex[i] = glm::vec4(TriangularFace.TriangleVertices[i].position, 1.0f);

	}
	//std::cout << "vertex =" << vertex[0].x<<"\n";
	for (int i = 0; i < 3; i++)
	{
		vertex[i] = modelMatrix * CameraMatrix * vertex[i];
		vertex[i].x = ((vertex[i].x / vertex[i].w) + 1) * width / 2 - width / 2;
		vertex[i].y = ((vertex[i].y / vertex[i].w) + 1) * height / 2 - height / 2;
	}
	/*std::cout << "vertex[0].x=" << vertex[0].x << "\n";
	std::cout << "vertex[0].w=" << vertex[0].w << "\n";
	vertex[0].x = ((vertex[0].x / vertex[0].w) + 1) * width / 2 - width / 2;
	std::cout << "vertex[0]=" << vertex[0].x<<"\n";
	*/
	float intensity[3];

	intensity[0] = glm::dot(TriangularFace.TriangleVertices[0].normal, lightDir);
	intensity[1] = glm::dot(TriangularFace.TriangleVertices[1].normal, lightDir);
	intensity[2] = glm::dot(TriangularFace.TriangleVertices[2].normal, lightDir);

	//std::cout << "vertex =" << vertex[0].x;
	//std::cout << "normal = " << TriangularFace.TriangleVertices[0].normal.x << "\t" << TriangularFace.TriangleVertices[0].normal.y << "\t" << TriangularFace.TriangleVertices[0].normal.z << "\n";
	//std::cout << "intensity[0]" << intensity[0]<<"\n";

	for (int i = 0 ; i < 3 ; i++)
	{
		box.max.x = std::max(box.max.x, vertex[i].x);
		box.min.x = std::min(box.min.x, vertex[i].x);

		box.min.y = std::min(box.min.y, vertex[i].y);

		box.max.x = std::min(box.max.x, (float)width);
		box.max.y = std::min(box.max.y, (float)height);

		box.min.x = std::max(box.min.x, -(float)width);
		box.min.y = std::max(box.min.y, -(float)height);

	}

	glm::vec3 point;
	glm::vec3 color;

	

	for (point.x=box.min.x;point.x<=box.max.x;point.x++)
	{
		for (point.y = box.min.y; point.y <= box.max.y; point.y++)
		{
			
			glm::vec3 bPoint = barycentricPoint(vertex[0],vertex[1],vertex[2],point);


			if (bPoint.x < 0 || bPoint.y < 0 || bPoint.z < 0 || bPoint.x<-(float)width || bPoint.y<-(float)height)
				continue;
			float inten = bPoint.x * intensity[0] + bPoint.y * intensity[1] + bPoint.z * intensity[2];
			color = TriangularFace.mtl.kd + TriangularFace.mtl.kd * inten;
			point.z = TriangularFace.TriangleVertices[0].position.z * bPoint.x + TriangularFace.TriangleVertices[1].position.z * bPoint.y + TriangularFace.TriangleVertices[2].position.z * bPoint.z + 100.0f;
			putpixel_adjusted(point,color);
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

#endif // !SHADER_H
