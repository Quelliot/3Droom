#include"loadOBJ.h"

void model::loadObj(const char* ModelFilename, const char* MaterialFilename)
{
	MaterialParse materialObj;
	materialObj.LoadMaterialFile(MaterialFilename);

	int materialNumber = 0;
	char trash;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	std::ifstream in(ModelFilename);
	std::string line, str;
	if (!in.is_open())
	{
		throw "Error: could not Open File";
	}

	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream ss(line.c_str());

		if (!line.compare(0, 6, "usemtl"))
		{
			ss >> str >> str;
			str = (str.substr(str.size() - 3));
			std::stringstream tempss(str);
			tempss >> materialNumber;
		}
		else if (!line.compare(0, 2, "v "))  //starts with v<space>
		{
			ss >> trash; // first character is v
			glm::vec3 v;
			// followed by xyz co-ords
			ss >> v.x >> v.y >> v.z;
			vertices.push_back(v);
		}

		else if (!line.compare(0, 3, "vn "))
		{
			ss >> trash >> trash;
			glm::vec3 n;
			ss >> n.x >> n.y >> n.z;
			normals.push_back(n);
		}

		else if (!line.compare(0, 2, "f "))
		{
			triangle face;
			std::vector<glm::vec3> tempVertexArray;
			glm::vec3 tempVertex;
			ss >> trash;
			while (ss >> tempVertex.x >> trash >> tempVertex.y >> trash >> tempVertex.z)
			{
				tempVertex.x--;
				tempVertex.y--;
				tempVertex.z--;
				tempVertexArray.push_back(tempVertex);
			}

			for (int i = 0; i < 3; i++)
			{
				face.TriangleVertices[i].position = vertices.at(tempVertexArray.at(i).x);
				face.TriangleVertices[i].normal = normals.at(tempVertexArray.at(i).z);
				//face.position[i] = vertices.at(tempVertexArray.at(i).x);
			}
			face.mtl = materialObj.LoadMaterial(materialNumber);
			TriangleData.push_back(face);
		}
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		std::cout << vertices.at(i).x << "\t" << vertices.at(i).y << "\t" << vertices.at(i).z << "\n";
	}

	std::cout << std::endl;

	for (int i = 0; i < normals.size(); i++)
	{
		std::cout << normals.at(i).x << "\t" << normals.at(i).y << "\t" << normals.at(i).z << "\n";
	}

	/*std::cout << std::endl;
	std::cout << TriangleData.size();
	std::cout << std::endl;
	std::cout << TriangleData.at(0).TriangleVertices[0].position.x;
	//std::cout << TriangleData.at(0).position[0].x;
	std::cout << std::endl;
	std::cout << TriangleData.at(2).mtl.materialNumber;
	std::cout << std::endl;
	std::cout << TriangleData.at(2).mtl.kd.x;
	*/
}