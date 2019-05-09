#include "XTMesh.h"
#include <string>


XTMesh::XTMesh(std::vector<XTVertex>& vecVertex, std::vector<unsigned int>& vecIndices, std::vector<XTTexture>& vecTexture)
{
	this->m_vVertices = vecVertex;
	this->m_vIndices = vecIndices;
	this->m_vTextures = vecTexture;

	setupMesh();
}


XTMesh::~XTMesh()
{
}

void XTMesh::Draw(Shader shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < m_vTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = m_vTextures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_vTextures[i].id);
	}

	glActiveTexture(0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_vIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void XTMesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vVertices.size() * sizeof(XTVertex), &m_vVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_vIndices.size() * sizeof(unsigned int), &m_vIndices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(XTVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(XTVertex), (void*)offsetof(XTVertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(XTVertex), (void*)offsetof(XTVertex, texCoords));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
}
