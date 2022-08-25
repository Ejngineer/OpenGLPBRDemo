#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ID);
	Bind();
}

VertexArray::VertexArray(int num)
{
	//TODO SETUP VERTEX BUFFER CLASS TO CONSTRUCT ARRAY OF BUFFERS
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_ID);
}

void VertexArray::UnBind()
{
	glBindVertexArray(0);
}
