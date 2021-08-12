#ifndef OBJLOADER_CLASS_H
#define OBJLOADER_CLASS_H

//STD libs
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<algorithm>

#include<GLFW/glfw3.h>


//Own libs
#include"Vertex.h"

#endif // !OBJLOADER_CLASS_H

std::vector<Vertex> loadOBJ(const char* File_Name);