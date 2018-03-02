#include "porcupine.h"
#include "main.h"

Porcupine::Porcupine(float x, float y, color_t color)
{
    this->position = glm::vec3(x, y, 0);
    speed = 0.01;
    static const GLfloat vertex_buffer_data[] = {
//            -0.3, -0.1, 0, // vertex 1
//            0, -0.1, 0, // vertex 2
//            -0.15, 0.2, 0, // vertex 3

//         0, -0.1, 0, // vertex 1
//         0.3, -0.1, 0, // vertex 2
//         0.15, 0.2, 0, // vertex 3

//        -0.3, -0.1, 0, // vertex 1
//        -0.6, -0.1, 0, // vertex 2
//        -0.45, 0.2, 0, // vertex 3
                 0.15, -0.1, 0, // vertex 1
                 0.45, -0.1, 0, // vertex 2
                 0.3, 0.2, 0, // vertex 3

                 0.15, -0.1, 0, // vertex 1
                 -0.15, -0.1, 0, // vertex 2
                 0, 0.2, 0, // vertex 3

                -0.15, -0.1, 0, // vertex 1
                -0.45, -0.1, 0, // vertex 2
                -0.3, 0.2, 0, // vertex 3

    };
    this->object = create3DObject(GL_TRIANGLES, 9, vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Porcupine::tick(){
    if(this->position.x + speed <= -2.75)
        speed = -speed;
    else if(this->position.x + speed >= -2.35)
        speed = -speed;
    else
        this->position.x += speed;
}
