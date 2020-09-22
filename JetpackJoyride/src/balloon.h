#include "main.h"

#ifndef BALLOON_H
#define BALLOON_H


class Balloon {
public:
    Balloon() {}
    Balloon(float x, float y, float direction);
    glm::vec3 position;
    glm::vec3 speed;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t balloon_box;
private:
    VAO *object;
};

#endif // BALLOON_H
