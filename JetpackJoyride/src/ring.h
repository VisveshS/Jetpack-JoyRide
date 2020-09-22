#include "main.h"
#include "player.h"

#ifndef RING_H
#define RING_H


class Ring {
public:
    Ring() {}
    Ring(float x, float y);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(Player *player);
    double birth;
    float life;
private:
    VAO *object;
    VAO *object2;
};

#endif // RING_H
