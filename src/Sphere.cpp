#include "Sphere.h"
#include <iostream>

const int MIN_SECTOR_COUNT  = 3;
const int MIN_STACK_COUNT  = 2;


Sphere::Sphere(float radius, int sectors, int stacks, bool smooth)
{
	setSphere(radius, sectors, stacks, smooth);
}

void Sphere::setSphere(float radius, int sectors, int stacks, bool smooth)
{
	this->radius = radius;
	this->sectors = sectors;
	if (sectors < MIN_SECTOR_COUNT)
		this->sectors = MIN_SECTOR_COUNT;
	this->stacks = stacks;
	if (stacks < MIN_STACK_COUNT)
		this->stacks = MIN_STACK_COUNT;

	this->smooth = smooth;

	if (smooth)
		buildSmooth();
	else
		buildFlat();
}

void Sphere::printSelf() const
{
	std::cout << "===== Sphere =====\n"
		<< "        Radius: " << radius << "\n"
		<< "  Sector Count: " << sectors << "\n"
		<< "   Stack Count: " << stacks << "\n"
		<< "Smooth Shading: " << (smooth ? "true" : "false") << "\n"
		<< "Triangle Count: " << getTriangleCount() << "\n"
		<< "   Index Count: " << getIndexCount() << "\n"
		<< "  Vertex Count: " << getVertexCount() << "\n"
		<< "  Normal Count: " << getNormalCount() << "\n"
		<< "TexCoord Count: " << getTexCoordCount() << std::endl;
}

void Sphere::Draw()
{
	VAO.Bind();
	VBO.Bind();

	glDrawElements(GL_TRIANGLES, getIndexCount() , GL_UNSIGNED_INT, (void*)0);

	VBO.UnBind();
	VAO.UnBind();
}

void Sphere::buildSmooth()
{
	const float PI = acos(-1);
	float x, y, z, s, t, xy;
	float nx, ny, nz, lengthInv = 1.0f / radius;

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;

	float sectorAngle, stackAngle;

	for (int i = 0; i <= stacks; i++)
	{
		stackAngle = PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);
		for (int j = 0; j <= sectors; j++)
		{
			sectorAngle = j * sectorStep;

			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			addVertex(x, y, z);

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			addNormal(nx, ny, nz);

			s = (float)j / sectors;
			t = (float)i / stacks;
			addTexCoords(s, t);
		}
	}

	int i, j;
	unsigned int k1, k2;
	for (i = 0; i < stacks; i++)
	{
		k1 = i * (sectors + 1);
		k2 = k1 + (sectors + 1);

		for (j = 0; j < sectors; j++, k1++, k2++)
		{
			if (i != 0)
			{
				addIndices(k1, k2, k1 + 1);
			}
			if (i != (stacks - 1))
			{
				addIndices(k1 + 1, k2, k2 + 1);
			}
		}
	}
	buildInterleavedVertices();
	setupSphere();
}

void Sphere::buildFlat()
{
	const float PI = acos(-1);

	struct Vertex
	{
		float x, y, z, s, t;
	};
	std::vector<Vertex> tmpVertices;

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;
	float sectorAngle;
	float stackAngle;

	for (int i = 0; i <= stacks; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for(int j = 0; j <= sectors; ++j)
		{
			sectorAngle = j * sectorStep;

			Vertex vertex;
			vertex.x = xy * cosf(sectorAngle);
			vertex.y = xy * sinf(sectorAngle);
			vertex.z = z;
			vertex.s = (float)j / sectors;
			vertex.t = (float)i / stacks;

			tmpVertices.push_back(vertex);
		}
	}

	clearArrays();

	Vertex v1, v2, v3, v4;
	std::vector<float>n;

	int i, j, k, vi1, vi2;
	int index = 0;
	for (i = 0; i < stacks; ++i)
	{
		vi1 = i * (sectors + 1);
		vi2 = (i + 1) * (sectors + 1);

		for (j = 0; j < sectors; ++j, ++vi1, ++vi2)
		{
			v1 = tmpVertices[vi1];
			v2 = tmpVertices[vi2];
			v3 = tmpVertices[vi1 + 1];
			v4 = tmpVertices[vi2 + 1];

			if (i == 0)
			{
				addVertex(v1.x, v1.y, v1.z);
				addVertex(v2.x, v2.y, v2.z);
				addVertex(v4.x, v4.y, v4.z);
				
				addTexCoords(v1.s, v1.t);
				addTexCoords(v2.s, v2.t);
				addTexCoords(v4.s, v4.t);
				

				n = computeFaceNormals(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
				for (k = 0; k < 3; ++k)
				{
					addNormal(n[0], n[1], n[2]);
				}

				addIndices(index, index + 1, index + 2);

				lineIndices.push_back(index);
				lineIndices.push_back(index+1);

				index += 3;
			}
			else if (i == (stacks - 1))
			{
				
				 addVertex(v1.x, v1.y, v1.z);
				 addVertex(v2.x, v2.y, v2.z);
				 addVertex(v3.x, v3.y, v3.z);

				 addTexCoords(v1.s, v1.t);
				 addTexCoords(v2.s, v2.t);
				 addTexCoords(v3.s, v3.t);
				

				n = computeFaceNormals(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
				for (k = 0; k < 3; ++k)
				{
					addNormal(n[0], n[1], n[2]);
				}

				addIndices(index, index + 1, index + 2);

				lineIndices.push_back(index);
				lineIndices.push_back(index + 1);
				lineIndices.push_back(index);
				lineIndices.push_back(index + 2);

				index +=3;
			}
			else {
				
				 addVertex(v1.x, v1.y, v1.z);
				 addVertex(v2.x, v2.y, v2.z);
				 addVertex(v3.x, v3.y, v3.z);
				 addVertex(v4.x, v4.y, v4.z);

				 addTexCoords(v1.s, v1.t);
				 addTexCoords(v2.s, v2.t);
				 addTexCoords(v3.s, v3.t);
				 addTexCoords(v4.s, v4.t);
				

				n = computeFaceNormals(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
				for (k = 0; k < 4; ++k)
				{
					addNormal(n[0], n[1], n[2]);
				}

				addIndices(index, index + 1, index + 2);
				addIndices(index + 2, index + 1, index + 3);

				lineIndices.push_back(index);
				lineIndices.push_back(index + 1);
				lineIndices.push_back(index);
				lineIndices.push_back(index + 2);

				index += 4;
			}
		}

	}
	buildInterleavedVertices();

	setupSphere();
}

void Sphere::buildInterleavedVertices()
{
	std::vector<float>().swap(interleavedVertices);

	std::size_t i, j;
	std::size_t count = vertices.size();

	for (i = 0, j = 0; i < count; i += 3, j += 2)
	{
		interleavedVertices.push_back(vertices[i]);
		interleavedVertices.push_back(vertices[i + 1]);
		interleavedVertices.push_back(vertices[i + 2]);

		interleavedVertices.push_back(normals[i]);
		interleavedVertices.push_back(normals[i + 1]);
		interleavedVertices.push_back(normals[i + 2]);

		interleavedVertices.push_back(texCoords[j]);
		interleavedVertices.push_back(texCoords[j + 1]);
	}
}

void Sphere::addVertex(float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

void Sphere::addNormal(float x, float y, float z)
{
	normals.push_back(x);
	normals.push_back(y);
	normals.push_back(z);
}

void Sphere::addTexCoords(float s, float t)
{
	texCoords.push_back(s);
	texCoords.push_back(t);
}

void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

void Sphere::setupSphere()
{
	VAO.Bind();
	VBO.Bind();
	VBO.getData(&interleavedVertices[0], (unsigned int)interleavedVertices.size() * sizeof(float));
	glGenBuffers(1, &EBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndices(), GL_STATIC_DRAW);
}

std::vector<float> Sphere::computeFaceNormals(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{

	const float EPSILON = 0.000001f;

	std::vector<float> normal(3, 0.0f);
	float nx, ny, nz;

	float ex1, ey1, ez1, ex2, ey2, ez2;
	
	ex1 = x2 - x1;
	ey1 = y2 - y1;
	ez1 = z2 - z1;
	ex2 = x3 - x1;
	ey2 = y3 - y1;
	ez2 = z3 - z1;

	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	float length = sqrtf(nx * nx + ny * ny + nz * nz);
	if (length > EPSILON)
	{
		float lengthInv = 1.0f / length;
		normal[0] = nx * lengthInv;
		normal[1] = ny * lengthInv;
		normal[2] = nz * lengthInv;
	}

	return normal;
}



//Private member functions
void Sphere::clearArrays()
{
	std::vector<float>().swap(vertices);
	std::vector<float>().swap(normals);
	std::vector<float>().swap(texCoords);
	std::vector<unsigned int>().swap(indices);
	std::vector<unsigned int>().swap(lineIndices);
}

