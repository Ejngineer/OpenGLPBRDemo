#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture()
{
	glGenTextures(1, &m_ID);
}

Texture::Texture(int num)
{
	glGenTextures(num, &m_ID);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::LoadTexture2D(const char* TexturePath)
{
	int width, height, nrComponents;
	unsigned char* data = stbi_load(TexturePath, &width, &height, &nrComponents, 0);
	if (data)
	{

		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		Bind2D();
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::LoadCubeMap(const std::vector<std::string> faces)
{
	int width, height, nrChannels = 0;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else 
		{
			std::cout << "Cubemap failed to load at path" << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void Texture::LoadHdrMap(const std::string& TexturePath)
{
	int width, height, nrcomponents;
	float *data = stbi_loadf(TexturePath.c_str(), &width, &height, &nrcomponents, 0);

	if (data)
	{
		Bind2D();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		std::cout << "FAILED TO LOAD HDR TEXTURE." << std::endl;
	}
	stbi_image_free(data);
}

void Texture::ActivateTexture(int tex_idx)
{
	glActiveTexture(GL_TEXTURE0 + tex_idx);
}

void Texture::EnableVertFlip()
{
	stbi_set_flip_vertically_on_load(true);
}

void Texture::DisableVertFlip()
{
	stbi_set_flip_vertically_on_load(false);
}

void Texture::BindCM()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void Texture::unBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind2D()
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}
