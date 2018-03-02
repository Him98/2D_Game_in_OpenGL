#include "main.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H


class Porcupine
{
public:
    Porcupine(){}
    Porcupine(float x, float y, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double speed;
    void tick();
private:
    VAO *object;
};

#endif // PORCUPINE_H
