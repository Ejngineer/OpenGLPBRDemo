#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <vector>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

struct ModelTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh 
{
private:
	std::vector<Vertex> vertices;
	std::vector<ModelTexture> textures;
	unsigned int VBO;
	unsigned int EBO;
	void setupMesh();
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures);
	void Draw(Shader shader);
	std::vector<unsigned int> indices;
	unsigned int VAO;
};