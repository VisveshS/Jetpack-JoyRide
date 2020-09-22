#include "main.h"

#ifndef HEART_H
#define HEART_H


class Heart {
public:
    Heart() {}
    Heart(float x, float y);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool bagged;
    bounding_box_t heart_box;
    void score_increment();
private:
    VAO *object;
};

#endif // HEART_H
