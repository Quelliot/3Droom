#pragma once
#ifndef ROOM_H
#define ROOM_H

#include<iostream>
#include<vector>
#include<algorithm>

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/matrix.hpp>
#include<glm/ext/matrix_transform.hpp>


#include"loadOBJ.h"
#include"Camera.h"

#define width 800
#define height 800

bool* grid;
float* zbuffer;
glm::vec3* color;

glm::vec3 BackgroundCol = glm::vec3(0.07f, 0.13f, 0.17f);
glm::vec3 CameraPosition = glm::vec3(0, 5.0f, 50.0f);

Camera camera(width,height,CameraPosition);

glm::vec3 lightPos = glm::vec3(0,0,0);
glm::vec3 lightDir = glm::normalize(glm::vec3(-1000,200,0));
glm::mat4 modelMatrix = glm::mat4(1.0f);


void clear()
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); 
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1);
}

void putpixel(glm::vec2 pixel, glm::vec3 color)
{
    glPointSize(1); // must be added before glDrawArrays is called
    glColor3f(color.x, color.y, color.z);
    glVertexPointer(2, GL_FLOAT, 0, &pixel); // point to the vertices to be used
    glDrawArrays(GL_POINTS, 0, 1); // draw the vertixes
}

void putpixel_adjusted(glm::vec2 pixel, glm::vec3 color)
{
    pixel.x = pixel.x + width/ 2;
    pixel.y = pixel.y + height / 2;
    putpixel(pixel, color);
}


//glm::mat4 cameraMatrix = camera.Matrix(45.0f,0.1f,100.0f);
/*
void InitializeGraph()
{
 
	grid = new bool[width * height];
	zbuffer = new float[width * height];
	color = new glm::vec3[width * height];
	for (GLint i = 0; i < width * height; i++ )
	{
		grid[i] = false;
		zbuffer[i] = std::numeric_limits<float>::min();
		color[i] = BackgroundCol;
	}
}

void cleargraph()
{
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (GLint i=0; i < width * height; i++)
	{
		grid[i] = false;
		zbuffer[i] = std::numeric_limits<float>::min();
        color[i] = BackgroundCol;
	}
}

void putpixel(glm::vec3 point, glm::vec3 col)
{
	if (point.x < width && point.x >= 0 && point.y < height && point.y >= 0)
	{
		if (zbuffer[(int)(point.x + point.y * width)]<=point.z)
		{
			grid[(int)(point.x + point.y * width)] = true;
			color[(int)(point.x + point.y * width)] = col;
			zbuffer[(int)(point.x + point.y * width)] = point.z;
		}
	}

}


void temp_putpixel(glm::vec2 point, glm::vec3 col)
{
    if (point.x < width && point.x >= 0 && point.y < height && point.y >= 0)
    {
        
            grid[(int)(point.x + point.y * width)] = true;
            color[(int)(point.x + point.y * width)] = col;
        
    }
}

void temp_putpixel_adjusted(glm::vec2 point, glm::vec3 col)
{
    point.x = point.x + width / 2;
    point.y = point.y + height / 2;
    temp_putpixel(point, col);
}

void drawGraph()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(1);
	glBegin(GL_POINTS);
	for (GLint i = 0; i < width; ++i) {
		for (GLint j = 0; j < height; ++j) {
			if (grid[(int)(i + j * width)]) 
			{
				glm::vec3& c = color[(int)(i + j * width)];
				glColor3f(c.x, c.y, c.z);
				glVertex2i(i, j);
			}
		}
	}
	glEnd();
	glFlush();
}

void putpixel_adjusted(glm::vec3 point, glm::vec3 col)
{
    point.x = point.x + width / 2;
    point.y = point.y + height / 2;
    putpixel(point,col);
}
 */
void line(float x1, float y1, float x2, float y2, glm::vec3 color)
    {
        /*x1 = (int)round(x1);
        x2 = (int)round(x2);
        y1 = (int)round(y1);
        y2 = (int)round(y2);
        */
        float dx, dy;

        dx = abs(x2 - x1);
        dy = abs(y2 - y1);
        //Sets increment/decrement : stepsize
        int lx, ly;

        if (x2 > x1) { lx = 1; }
        else { lx = -1; }

        if (y2 > y1) { ly = 1; }
        else { ly = -1; }
        //initialize
        int x = x1, y = y1;
        //slope<1
        if (dx > dy) {
            int p = 2 * dy - dx;
            for (int k = 0; k <= dx; k++) {
                //putpixel_adjusted(glm::vec3(x, y,INT_MAX), color);
                //temp_putpixel_adjusted(glm::vec2(x,y),color);
                putpixel_adjusted(glm::vec2(x, y), color);
                if (p < 0) {
                    x += lx;
                    p += 2 * dy;
                }
                else {
                    x += lx;
                    y += ly;
                    p += 2 * dy - 2 * dx;
                }
            }
        }
        //if slope>=1
        else {
            int p = 2 * dx - dy;
            for (int k = 0; k <= dy; k++) {
                //putpixel_adjusted(glm::vec3(x, y,INT_MAX), color);
                //temp_putpixel_adjusted(glm::vec2(x, y), color);
                putpixel_adjusted(glm::vec2(x, y), color);
                if (p < 0) {
                    y += ly;
                    p += 2 * dx;
                }
                else {
                    x += lx;
                    y += ly;
                    p += 2 * dx - 2 * dy;
                }
            }
        }
        //putpixel_adjusted(glm::vec3(x, y,INT_MAX), color);
        //temp_putpixel_adjusted(glm::vec2(x, y), color);
        putpixel_adjusted(glm::vec2(x, y), color);
    }

void line(glm::vec4 point1, glm::vec4 point2, glm::vec3 color)
{
    int x1 = (int)round(((point1.x/point1.w)+1) * width / 2 - width / 2);
    int y1 = (int)round(((point1.y/point1.w)+1) * height / 2 - height / 2);
    int x2 = (int)round(((point2.x/point2.w)+1) * width / 2 - width / 2);
    int y2 = (int)round(((point2.y/point2.w)+1) * height / 2 - height / 2);

    /*std::cout << point1.x << "\n";
    std::cout << point1.w << "\n";
    std::cout << x1<<"\n";
    */
    float dx = abs(x2 - x1);
    float dy = abs(y2 - y1);
    //Sets increment/decrement : stepsize
    int lx, ly;

    if (x2 > x1) { lx = 1; }
    else { lx = -1; }

    if (y2 > y1) { ly = 1; }
    else { ly = -1; }
    //initialize
    int x = x1, y = y1;
    //slope<1
    if (dx > dy) {
        int p = 2 * dy - dx;
        for (int k = 0; k <= dx; k++) {
            //putpixel_adjusted(glm::vec3(x, y, INT_MAX), color);
            putpixel_adjusted(glm::vec2(x, y), color);
            if (p < 0) {
                x += lx;
                p += 2 * dy;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    //if slope>=1
    else {
        int p = 2 * dx - dy;
        for (int k = 0; k <= dy; k++) {
            //putpixel_adjusted(glm::vec3(x, y, INT_MAX), color);
            putpixel_adjusted(glm::vec2(x, y), color);
            if (p < 0) {
                y += ly;
                p += 2 * dx;
            }
            else {
                x += lx;
                y += ly;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    //putpixel_adjusted(glm::vec3(x, y, INT_MAX), color);
    putpixel_adjusted(glm::vec2(x, y), color);

}

void drawTriangle(triangle TriangularFace, glm::mat4 CameraMatrix)
{
    glm::vec3 normal = glm:: normalize(glm::cross(TriangularFace.TriangleVertices[1].position-TriangularFace.TriangleVertices[0].position, TriangularFace.TriangleVertices[2].position - TriangularFace.TriangleVertices[0].position));
    if (glm::dot(normal, glm::vec3(TriangularFace.TriangleVertices[0].position - camera.Position)) < 0)
    {
        
    
        glm::vec4 vertex1 = glm::vec4(TriangularFace.TriangleVertices[0].position, 1.0f);
        glm::vec4 vertex2 = glm::vec4(TriangularFace.TriangleVertices[1].position, 1.0f);
        glm::vec4 vertex3 = glm::vec4(TriangularFace.TriangleVertices[2].position, 1.0f);

        vertex1 = modelMatrix * CameraMatrix * vertex1;
        vertex2 = modelMatrix * CameraMatrix * vertex2;
        vertex3 = modelMatrix * CameraMatrix * vertex3;

  

        line(vertex1, vertex2, TriangularFace.mtl.kd);
        line(vertex3, vertex2, TriangularFace.mtl.kd);
        line(vertex1, vertex3, TriangularFace.mtl.kd);
    }
}

void fillBottomFlatTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3,
    const glm::vec3& color) {
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float currentx1 = v1.x;
    float currentx2 = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
        //draw_bresenham_adjusted((int)currentx1, scanlineY, (int)currentx2, scanlineY,  color);
        line(currentx1, scanlineY, currentx2, scanlineY, color);
        currentx1 += invslope1;
        currentx2 += invslope2;
    }
}

void fillTopFlatTriangle(const glm::vec2& v1, const glm::vec2& v2, const glm::vec2& v3,
    const glm::vec3& color) {
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float currentx1 = v3.x;
    float currentx2 = v3.x;

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
        //draw_bresenham_adjusted((int)currentx1, scanlineY, (int)currentx2,scanlineY, color);

        line(currentx1, scanlineY, currentx2, scanlineY, color);
        currentx1 -= invslope1;
        currentx2 -= invslope2;
    }
}
glm::vec2 interpolate(const glm::vec2& src, const glm::vec2& dst, float alpha) {
    return src + (dst - src) * alpha;
}
bool sortcol(const glm::vec2& v1, const glm::vec2& v2) { return v1.y < v2.y; }



void rasterizeByTriangle(triangle TriangularFace, glm::mat4 CameraMatrix)
{
        
            glm::vec3 normal = glm::normalize(glm::cross(TriangularFace.TriangleVertices[1].position - TriangularFace.TriangleVertices[0].position, TriangularFace.TriangleVertices[2].position - TriangularFace.TriangleVertices[0].position));
            if (glm::dot(normal, glm::vec3(TriangularFace.TriangleVertices[0].position - camera.Position)) < 0)
            {
                glm::vec4 vertex1 = glm::vec4(TriangularFace.TriangleVertices[0].position, 1.0f);
                glm::vec4 vertex2 = glm::vec4(TriangularFace.TriangleVertices[1].position, 1.0f);
                glm::vec4 vertex3 = glm::vec4(TriangularFace.TriangleVertices[2].position, 1.0f);

                vertex1 = modelMatrix * CameraMatrix * vertex1;
                vertex2 = modelMatrix * CameraMatrix * vertex2;
                vertex3 = modelMatrix * CameraMatrix * vertex3;


                std::vector<glm::vec2>point = std::vector<glm::vec2>(3);

                point[0] = glm::vec2(((vertex1.x / vertex1.w) + 1) * width / 2 - width / 2, ((vertex1.y / vertex1.w) + 1) * height / 2 - height / 2);
                point[1] = glm::vec2(((vertex2.x / vertex2.w) + 1) * width / 2 - width / 2, ((vertex2.y / vertex2.w) + 1) * height / 2 - height / 2);
                point[2] = glm::vec2(((vertex3.x / vertex3.w) + 1) * width / 2 - width / 2, ((vertex3.y / vertex3.w) + 1) * height / 2 - height / 2);
            
                glm::vec3 color = TriangularFace.mtl.kd;
                
                glm::vec3 lightDir = glm::normalize(glm::vec3(vertex1)-lightPos);
                float intensity = glm::max(glm::dot(lightDir,normal),0.0f);
                color = color* intensity;

                std::sort(point.begin(), point.end(), sortcol);
                if (point[0].y == point[1].y)
                {
                    if (point[1].x < point[0].x)
                        std::swap(point[0],point[1]);
                    fillTopFlatTriangle(point[0],point[1],point[2],color);


                }
                else if (point[1].y == point[2].y)
                {
                    if (point[2].x < point[1].x)
                        std::swap(point[1],point[2]);
                    fillBottomFlatTriangle(point[0],point[1],point[2],color);
                }

                else
                {
                    float split = (point[1].y - point[0].y) / (point[2].y - point[0].y);
                    auto vi = interpolate(point[0],point[2],split);

                    if (point[1].x < vi.x)
                    {
                        fillBottomFlatTriangle(point[0],point[1],vi,color);
                        fillTopFlatTriangle(point[1],vi,point[2],color);
                    }
                    else
                    {
                        fillBottomFlatTriangle(point[0],vi,point[1],color);
                        fillTopFlatTriangle(vi,point[1],point[2],color);
                    }
                }


            }


            
       
    
}
#endif // !ROOM_H

