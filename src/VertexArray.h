#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include <glad/glad.h>

class VertexArray
{
private:
	unsigned int m_ID;
public:
	VertexArray();
	VertexArray(int num);
	~VertexArray();
	void Bind();
	void UnBind();
};

#endif