#include"material.h"


void MaterialParse::LoadMaterialFile(std::string Filename)
{
    std::ifstream in(Filename);
    std::string line = "";
    std::string strash;

    //error if file is not opened
    if (!in.is_open())
    {
        throw "Error: could not Open File";
    }

    MaterialData temp;
    char trash;
    while (!in.eof())
    {

        std::getline(in, line);
        std::istringstream ss(line.c_str());



        if (!line.compare(0, 6, "newmtl"))  //New Material
        {
            if (temp.materialNumber)
                Materials.push_back(temp);   //This pushes MaterialDate into vector
            ss >> strash >> strash;
            strash = (strash.substr(strash.size() - 3));
            std::stringstream tempss(strash);
            tempss >> temp.materialNumber;
        }
        if (!line.compare(0, 2, "Ns"))  //Shininess
        {
            ss >> trash >> trash >> temp.Ns;      // line is in the form ofNs 1.00000
        }
        if (!line.compare(0, 2, "Ka"))  //Ambient colour
        {
            ss >> trash >> trash >> temp.ka.x >> temp.ka.y >> temp.ka.z;      // line is in the form of Ka 0.1 0.2 0.3
        }
        if (!line.compare(0, 2, "Kd"))  //Diffuse colour
        {
            ss >> trash >> trash >> temp.kd.x >> temp.kd.y >> temp.kd.z;      // line is in the form of kd 0.1 0.2 0.3
        }
        if (!line.compare(0, 2, "Ks"))  //Specular colour
        {
            ss >> trash >> trash >> temp.ks.x >> temp.ks.y >> temp.ks.z;      // line is in the form of ks 0.1 0.2 0.3
        }


    }
    Materials.push_back(temp);
    std::cout << "Numbers of materials: " << Materials.size() << "\n";
    for (int i = 0; i < Materials.size(); i++)
    {
        std::cout << std::endl;
        std::cout << "MaterialNumber: " << Materials.at(i).materialNumber << std::endl;
        std::cout << "Ambient: " << Materials.at(i).ka.x << "\t" << Materials.at(i).ka.y << "\t" << Materials.at(i).ka.z << std::endl;
        std::cout << "Diffuse: " << Materials.at(i).kd.x << "\t" << Materials.at(i).kd.y << "\t" << Materials.at(i).kd.z << std::endl;
        std::cout << "Specular: " << Materials.at(i).ks.x << "\t" << Materials.at(i).ks.y << "\t" << Materials.at(i).ks.z << std::endl;
        std::cout << "Alpha: " << Materials.at(i).Ns << std::endl;
        std::cout << std::endl;
    }

}

MaterialData MaterialParse::LoadMaterial(int materialNumber)
{
    for (int i = 0; i < Materials.size(); i++)
    {
        if (Materials.at(i).materialNumber == materialNumber)
        {
            return Materials.at(i);
        }
    }
}