#include "main.h"

#ifndef POND_H
#define POND_H


class Pond
{
public:
    Pond(){}
    Pond(float x, float y, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double r,speed;
    float rotation;
private:
    VAO *object;
};

#endif // POND_H
