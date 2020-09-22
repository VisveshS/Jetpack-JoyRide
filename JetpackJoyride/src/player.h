#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    void draw(glm::mat4 VP,int state);//ground=0,flying=1
    void set_position(float x, float y);
    void tick(float ground);
    bool safe,safe1,safe2;
    bool protecc;
    bounding_box_t player_box;
private:
    VAO *object;
    VAO *burnt;
    VAO *fire;
};

#endif // PLAYER_H
