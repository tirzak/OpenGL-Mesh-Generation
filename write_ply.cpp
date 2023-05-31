#include "write_ply.h"
#include <fstream>
#include <iostream>

using namespace std;
void writeToPlyHelper(string &filename, vector<float> &vertices, vector<float> &normals, vector<int> &faces)
{
    ofstream myStream(filename);

    if (!myStream.is_open())
    {
        cerr << "Failed to open " << filename  << endl;
        return;
    }

    int vertexCount = vertices.size() / 3;
    int faceCount = faces.size() / 3;

    myStream << "ply\n";
    myStream << "format ascii 1.0\n";
    myStream << "element vertex " << vertexCount << "\n";
    myStream << "property float x\n";
    myStream << "property float y\n";
    myStream << "property float z\n";
    myStream << "property float nx\n";
    myStream << "property float ny\n";
    myStream << "property float nz\n";
    myStream << "element face " << faceCount << "\n";
    myStream << "property list uchar int vertex_indices\n";
    myStream << "end_header\n";

    for (int i = 0; i < vertexCount; ++i)
    {
        int idx = i * 3;
        myStream << vertices[idx] << " " << vertices[idx + 1] << " " << vertices[idx + 2] << " ";
        myStream << normals[idx] << " " << normals[idx + 1] << " " << normals[idx + 2] << "\n";
    }

    
for (int i = 0; i < faceCount; ++i)
{
    int idx = i * 3;
    myStream << "3 " << faces[idx] << " " << faces[idx + 1] << " " << faces[idx + 2] << "\n";
}


    myStream.close();
}
void WriteToPly(vector<float> &vertexWithNormals, string &fileName)
{
    vector<float> vertices;
    vector<float> normals;
    vector<int> faces;

    for (int i = 0; i < (int)vertexWithNormals.size(); i += 6)
    {
        vertices.insert(vertices.end(), vertexWithNormals.begin() + i, vertexWithNormals.begin() + i + 3);
        normals.insert(normals.end(), vertexWithNormals.begin() + i + 3, vertexWithNormals.begin() + i + 6);
    }


for (int i = 0; i < vertices.size() / 3; i++) {
    faces.push_back(i * 3);
    faces.push_back(i * 3 + 1);
    faces.push_back(i * 3 + 2);
}

    writeToPlyHelper(fileName,vertices,normals,faces);
}

