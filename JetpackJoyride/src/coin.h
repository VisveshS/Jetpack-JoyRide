#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, int type);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void score_increment();
    bool bagged;
    int type;
    bounding_box_t coin_box;
private:
    VAO *object;
    VAO *objectbig;
};

#endif // COIN_H
