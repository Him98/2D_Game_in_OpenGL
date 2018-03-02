#include "pond.h"
#include "main.h"

Pond::Pond(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    r = 1.5;
    GLfloat g_vertex_buffer_data[2500];
    int k=0,i;
    for (i=180;i<=360;i++)
    {
        g_vertex_buffer_data[k]=r * cos((i+1)*3.14159/180);
        k++;
        g_vertex_buffer_data[k]=r * sin((i+1)*3.14159/180);
        k++;
        g_vertex_buffer_data[k]=0.0f;
        k++;
        g_vertex_buffer_data[k]=0.0f;
        k++;
        g_vertex_buffer_data[k]=0.0f;
        k++;
        g_vertex_buffer_data[k]=0.0f;
        k++;
        g_vertex_buffer_data[k]=r * cos(i*3.14159/180);
        k++;
        g_vertex_buffer_data[k]=r * sin(i*3.14159/180);
        k++;
        g_vertex_buffer_data[k]=0.0f;
        k++;
    }
    this->object = create3DObject(GL_TRIANGLES, 540, g_vertex_buffer_data, color, GL_FILL);
}

void Pond::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Pond::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Pond::tick(){
}
