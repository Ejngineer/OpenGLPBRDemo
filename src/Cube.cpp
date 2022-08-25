#include "Cube.h"

Cube::Cube(float x, float y, float z)
{
    float vertices[288] =
    {
        //NDC coords                  //Normals            //Texture coords
       -x/2.0f, -y/2.0f, -z/2.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        x/2.0f, -y/2.0f, -z/2.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        x/2.0f,  y/2.0f, -z/2.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        x/2.0f,  y/2.0f, -z/2.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
       -x/2.0f,  y/2.0f, -z/2.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
       -x/2.0f, -y/2.0f, -z/2.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

       -x/2.0f, -y/2.0f,  z/2.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        x/2.0f, -y/2.0f,  z/2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        x/2.0f,  y/2.0f,  z/2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        x/2.0f,  y/2.0f,  z/2.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
       -x/2.0f,  y/2.0f,  z/2.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
       -x/2.0f, -y/2.0f,  z/2.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

       -x/2.0f,  y/2.0f,  z/2.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
       -x/2.0f,  y/2.0f, -z/2.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
       -x/2.0f, -y/2.0f, -z/2.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
       -x/2.0f, -y/2.0f, -z/2.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
       -x/2.0f, -y/2.0f,  z/2.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
       -x/2.0f,  y/2.0f,  z/2.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        x/2.0f,  y/2.0f,  z/2.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        x/2.0f,  y/2.0f, -z/2.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        x/2.0f, -y/2.0f, -z/2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        x/2.0f, -y/2.0f, -z/2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        x/2.0f, -y/2.0f,  z/2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        x/2.0f,  y/2.0f,  z/2.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

       -x/2.0f, -y/2.0f, -z/2.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        x/2.0f, -y/2.0f, -z/2.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        x/2.0f, -y/2.0f,  z/2.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        x/2.0f, -y/2.0f,  z/2.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
       -x/2.0f, -y/2.0f,  z/2.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
       -x/2.0f, -y/2.0f, -z/2.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

       -x/2.0f,  y/2.0f, -z/2.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        x/2.0f,  y/2.0f, -z/2.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        x/2.0f,  y/2.0f,  z/2.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        x/2.0f,  y/2.0f,  z/2.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
       -x/2.0f,  y/2.0f,  z/2.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
       -x/2.0f,  y/2.0f, -z/2.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

	VBO.getData(&vertices, sizeof(vertices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    VAO.UnBind();
    VBO.UnBind();
}

Cube::~Cube()
{
}

void Cube::Draw()
{
    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    VAO.UnBind();
}