#include "main.h"
#include "player.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(Player *player,bool disable);
    double birth;
    float life;
private:
    VAO *object;
};

#endif // MAGNET_H
