#include "main.h"

#ifndef GROUND_H
#define GROUND_H


class Ground
{
public:
    Ground(){}
    Ground(float x, float y, color_t color);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float rotation;
private:
    VAO *object;
};

#endif // GROUND_H
