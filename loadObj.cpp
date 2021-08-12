#include"loadObj.h"

std::vector<Vertex> loadOBJ(const char* File_Name)
{
	// for vertices
	//store the read data to respective vectors
	std::vector<glm::vec4> Vertex_Position;
	std::vector<glm::vec3> Vertex_Normal;
	std::vector<glm::vec2> Vertex_TexCoord;

	// for faces
	//store the read indices
	std::vector<GLint> Vertex_Position_Indices;
	std::vector<GLint> Vertex_Normal_Indices;
	std::vector<GLint> Vertex_Tex_Indices;

	//Vertex array
	//creates an Vertex array object of the above read data
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in(File_Name);
	std::string line = "";
	std::string prefix = "";

	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;

	GLint temp_int = 0;

	//error if file is not opened
	if (!in.is_open())
	{
		throw "Error: could not Open File";
	}

	//Read one line at a time
	while (std::getline(in, line))//to get a single line at a time
	{
		//take in the prefix of the line i.e. v,vt,vn
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}

		else if (prefix == "o")
		{

		}

		else if (prefix == "s")
		{

		}
		else if (prefix == "use_mtl")
		{

		}
		else if (prefix == "v")
		{

			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			Vertex_Position.push_back(glm::vec4(temp_vec3,1.0f));
		}

		else if (prefix == "vt")
		{

			ss >> temp_vec2.x >> temp_vec2.y;
			Vertex_TexCoord.push_back(temp_vec2);
		}

		else if (prefix == "vn")
		{

			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			Vertex_Normal.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{
			int counter = 0;
			while (ss >> temp_int)
			{
				//puching indices in the arrays
				if (counter == 0)
				{
					Vertex_Position_Indices.push_back(temp_int);
				}
				else if (counter == 1)
				{
					Vertex_Tex_Indices.push_back(temp_int);
				}
				else if (counter == 2)
				{
					Vertex_Normal_Indices.push_back(temp_int);
				}
				//handling the unwanted characters
				if (ss.peek() == '/')
				{
					++counter;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					++counter;
					ss.ignore(1, ' ');
				}

				//resetting counter
				if (counter > 2)
					counter = 0;
			}
		}
		else
		{

		}


	}

	//main vertex array or mesh
	vertices.resize(Vertex_Position_Indices.size(), Vertex());

	//loading indices
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Position = Vertex_Position[Vertex_Position_Indices[i] - 1];
		vertices[i].TexCoord = Vertex_TexCoord[Vertex_Tex_Indices[i] - 1];
		vertices[i].Normal = Vertex_Normal[Vertex_Normal_Indices[i] - 1];
		vertices[i].Color = glm::vec3(1.0f, 1.0f, 1.0f);
	}


	std::cout << "no. of vertices: " << vertices.size() << "\n";


	//return a vertex vector // vector is similar to array, the benefit being that you dont need to specify the size thus is dynamic
	std::cout << "Object loaded successfully" << "\n";
	/*for(int i=0;i<vertices.size();i++)
	{
		std::cout << vertices[i].Position.x << vertices[i].Position.y << vertices[i].Position.z << "\n";
	}*/


	return vertices;
}