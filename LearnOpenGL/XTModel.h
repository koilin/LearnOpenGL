#ifndef XTMODEL_H
#define XTMODEL_H
#include <learnopengl/shader_m.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>

#include <vector>
#include "XTMesh.h"

class XTModel
{
public:
	XTModel(char* path);
	~XTModel();

	void Draw(Shader shader);

private:

	std::vector<XTMesh> m_vecMesh;
	std::string m_sDirectory;
	std::vector<XTTexture> m_vecTexLoaded;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* pNode, const aiScene* pScene);
	XTMesh ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
	std::vector<XTTexture> LoadMaterialTextures(aiMaterial* pMat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const std::string& path, const std::string& directory, bool gamma);
};

#endif
