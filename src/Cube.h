#ifndef __CUBE_H__
#define __CUBE_H__

#include "glad/glad.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class Cube
{
private:
	VertexArray VAO;
	VertexBuffer VBO;
    
public:
	Cube(float x, float y, float z);
	~Cube();
	void Draw();
};

#endif 