#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"

class Model
{
public:
	explicit Model(const std::string& path, MATERIAL_SET materialSet, std::vector<unsigned> texUnits)
	{
		loadModel(path, materialSet, texUnits);
	}

	void draw(const ShaderProgram& shader) 
	{
		for(auto& mesh: m_meshes)
		{
			mesh.draw(shader);
		}
	}

	void loadMaterials(MATERIAL_SET materialSet, std::vector<unsigned int> texUnit);

private:
	void loadModel(const std::string& path, MATERIAL_SET materialSet, std::vector<unsigned> texUnits);
	
	void processNode(aiNode* node);

	Mesh processMesh(aiMesh* mesh);
	
	Material loadMaterial(aiMaterial* material, aiTextureType type, TEXTURE_TYPE textureType);

private:
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	std::vector<Material> m_materials;

	aiScene* m_aiScene;
	std::vector<aiMesh*> m_aiMeshes;
	
	MATERIAL_SET m_materialSet;
};