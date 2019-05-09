#ifndef XTMESH_H
#define XTMESH_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>


struct XTVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct XTTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class XTMesh
{
public:
	XTMesh(std::vector<XTVertex>& vecVertex, std::vector<unsigned int>& vecIndices, std::vector<XTTexture>& vecTexture);
	~XTMesh();
	void Draw(Shader shader);

public:
	std::vector<XTVertex> m_vVertices;
	std::vector<unsigned int> m_vIndices;
	std::vector<XTTexture> m_vTextures;

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	void setupMesh();
};

#endif

