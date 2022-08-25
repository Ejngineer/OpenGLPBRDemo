#ifndef __MODEL_H__
#define __MODEL_H__

#include <stb_image.h>

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Model
{
private:
	std::vector<ModelTexture> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	std::vector<ModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
	Model(const std::string& path)
	{
		loadModel(path);
	}
	void Draw(Shader& shader);
	std::vector<Mesh> meshes;
};

#endif