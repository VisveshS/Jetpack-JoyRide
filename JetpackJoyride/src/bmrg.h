#include "main.h"

#ifndef BMRG_H
#define BMRG_H


class Bmrg {
public:
    Bmrg() {}
    Bmrg(float x, float y,  color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
private:
    VAO *object;
};

#endif // BMRG_H
