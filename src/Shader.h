#ifndef __SHADER_H__
#define __SHADER_H__

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader 
{
private:
	//Program ID
	unsigned int ID;
	void checkCompileErrors(GLuint shader, std::string type);
public:
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();
	void SetBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat1f(const std::string& name, float value) const;
	void setFloat4(const std::string& name, float f0, float f1, float f2, float f3) const;
	void setMat4f(const std::string& name, glm::mat4 transform) const;
	void setVec3f(const std::string& name, glm::vec3 vec) const;
	void setVec3f(const std::string& name, float f0, float f1, float f2) const;
	void setVec2f(const std::string& name, glm::vec2 vec) const;
	void setVec2f(const std::string& name, float f0, float f1) const;
	unsigned int getID() const;
};

#endif