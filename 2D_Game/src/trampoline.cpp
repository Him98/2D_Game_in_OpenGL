#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    GLfloat g_vertex_buffer_data[10000],vertex_buffer_data1[10000];
    static const GLfloat vertex_buffer_data[] = {
            1, 0, 0, // vertex 1
            1.1,  0, 0, // vertex 2
            1.1,  -1.6, 0, // vertex 3

            1.1,  -1.6, 0, // vertex 3
            1.0, -1.6, 0, // vertex 4
            1.0, 0, 0, // vertex 1

        -1, 0, 0, // vertex 1
        -1.1, 0, 0, // vertex 2
        -1.1, -1.6, 0, // vertex 3

        -1.1, -1.6, 0, // vertex 3
        -1, -1.6, 0, // vertex 4
        -1, 0, 0 // vertex 1
        };
    int j=0,k=0,r=1;
    for (int i=180;i<360;i++)
    {
        vertex_buffer_data1[k]=r * cos((i+1)*3.14159/180);
        k++;
        vertex_buffer_data1[k]=r * sin((i+1)*3.14159/180);
        k++;
        vertex_buffer_data1[k]=0.0f;
        k++;
        vertex_buffer_data1[k]=0.0f;
        k++;
        vertex_buffer_data1[k]=0.0f;
        k++;
        vertex_buffer_data1[k]=0.0f;
        k++;
        vertex_buffer_data1[k]=r * cos(i*3.14159/180);
        k++;
        vertex_buffer_data1[k]=r * sin(i*3.14159/180);
        k++;
        vertex_buffer_data1[k]=0.0f;
        k++;
    }
    //this->object = create3DObject(GL_TRIANGLES, 540, vertex_buffer_data1, color, GL_FILL);
    k=0;
    for(int i = 0;i < 1656;i++){
        if(i<18){
            g_vertex_buffer_data[i] = vertex_buffer_data[i];
            k=i+1;
        }
        else if(i>=1638){
            g_vertex_buffer_data[i] = vertex_buffer_data[k];
            k++;
        }
        else{
            g_vertex_buffer_data[i] = vertex_buffer_data1[j];
            j++;
            g_vertex_buffer_data[i+1] = vertex_buffer_data1[j];
            j++;
            g_vertex_buffer_data[i+2] = vertex_buffer_data1[j];
            j++;
            i+=2;
        }
    }
      this->object = create3DObject(GL_TRIANGLES, 580, g_vertex_buffer_data, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Trampoline::tick(){
}

