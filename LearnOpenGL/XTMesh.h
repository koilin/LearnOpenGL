#ifndef XTMESH_H
#define XTMESH_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
};

class XTMesh
{
public:
	XTMesh(std::vector<Vertex>& vecVertex, std::vector<unsigned int>& vecIndices, std::vector<Texture>& vecTexture);
	~XTMesh();
	void Draw(Shader shader);

public:
	std::vector<Vertex> m_vVertices;
	std::vector<unsigned int> m_vIndices;
	std::vector<Texture> m_vTextures;

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	void setupMesh();
};

#endif

