#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>
#include <string>
#include <vector>

class Texture
{
private:
	unsigned int m_ID;
	std::string type;
public:
	Texture();
	Texture(int num);
	~Texture();
	void LoadTexture2D(const char* TexturePath);
	void LoadCubeMap(const std::vector<std::string> faces);
	void LoadHdrMap(const std::string& TexturePath);
	void ActivateTexture(int tex_idx);
	void EnableVertFlip();
	void DisableVertFlip();
	void Bind2D();
	void BindCM();
	void unBind();
};

#endif