#include "XTModel.h"



XTModel::XTModel(char* path)
{
	LoadModel(path);
}


XTModel::~XTModel()
{
}

void XTModel::Draw(Shader shader)
{
	for (unsigned int i = 0; i < m_vecMesh.size(); i++)
	{
		m_vecMesh.at(i).Draw(shader);
	}
}

void XTModel::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		std::cout << "Error::assimp:: " << importer.GetErrorString() << std::endl;
		return;
	}

	m_sDirectory = path.substr(0, path.find_last_of('/')+1);
	ProcessNode(pScene->mRootNode, pScene);
}

void XTModel::ProcessNode(aiNode* pNode, const aiScene* pScene)
{
	if (pNode == NULL)
		return;

	int nNodes = pNode->mNumMeshes;
	for (int i = 0; i < nNodes; i++)
	{
		aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
		m_vecMesh.push_back(ProcessMesh(pMesh, pScene));
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ProcessNode(pNode->mChildren[i], pScene);
	}
	
}

XTMesh XTModel::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
{
	std::vector<XTVertex> vecVertices;
	std::vector<unsigned int> vecIndices;
	std::vector<XTTexture> vecTextures;

	if (pMesh == NULL || pScene == NULL)
		return XTMesh(vecVertices, vecIndices, vecTextures);

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
	{
		XTVertex vertex;

		//处理顶点位置，法向，纹理坐标
		glm::vec3 position;
		position.x = pMesh->mVertices[i].x;
		position.y = pMesh->mVertices[i].y;
		position.z = pMesh->mVertices[i].z;
		vertex.position = position;

		glm::vec3 normal;
		normal.x = pMesh->mNormals[i].x;
		normal.y = pMesh->mNormals[i].y;
		normal.z = pMesh->mNormals[i].z;
		vertex.normal = normal;

		glm::vec2 texCoords = glm::vec2(0.0, 0.0);
		if (pMesh->mTextureCoords[0])
		{
			texCoords.x = pMesh->mTextureCoords[0][i].x;
			texCoords.y = pMesh->mTextureCoords[0][i].y;
		}
		vertex.texCoords = texCoords;

		vecVertices.push_back(vertex);
	}

	//处理索引
	for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
	{
		aiFace face = pMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			vecIndices.push_back(face.mIndices[j]);
		}
	}

	//处理材质
	unsigned int iMaterial = pMesh->mMaterialIndex;
	if (iMaterial >= 0)
	{
		aiMaterial* pMaterial = pScene->mMaterials[iMaterial];
		std::vector<XTTexture> diffuseMaps = LoadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
		vecTextures.insert(vecTextures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<XTTexture> specularMaps = LoadMaterialTextures(pMaterial, aiTextureType_SPECULAR, "texture_specular");
		vecTextures.insert(vecTextures.end(), specularMaps.begin(), specularMaps.end());
	}

	return XTMesh(vecVertices, vecIndices, vecTextures);
}

std::vector<XTTexture> XTModel::LoadMaterialTextures(aiMaterial* pMat, aiTextureType type, std::string typeName)
{

	std::vector<XTTexture> vecTextures;
	if (pMat == NULL)
	{
		return vecTextures;
	}
	for (unsigned int i = 0; i < pMat->GetTextureCount(type); i++)
	{
		aiString strPath;
		pMat->GetTexture(type, i, &strPath);
		bool bSkip = false;
		for (unsigned int j = 0; j < m_vecTexLoaded.size(); j++)
		{
			if (std::strcmp(m_vecTexLoaded[j].path.c_str(), strPath.C_Str()) == 0)
			{
				vecTextures.push_back(m_vecTexLoaded[j]);
				bSkip = true;
				break;
			}
		}
		if (!bSkip)
		{
			XTTexture texture;
			texture.id = TextureFromFile(strPath.C_Str(), m_sDirectory, 0);
			texture.type = typeName;
			texture.path = strPath.C_Str();
			vecTextures.push_back(texture);
			m_vecTexLoaded.push_back(texture);
		}
	}
	return vecTextures;
}

unsigned int XTModel::TextureFromFile(const std::string& path, const std::string& directory, bool gamma)
{
	std::string fileName = directory + path;

	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width;
	int height;
	int nComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nComponents, 0);
	if (data)
	{
		GLenum format;
		if (nComponents == 1)
		{
			format = GL_RED;
		}
		else if (nComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << fileName.c_str() << std::endl;
		stbi_image_free(data);
	}

	return textureId;
}
