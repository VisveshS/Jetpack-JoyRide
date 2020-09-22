#include "main.h"

#ifndef SX2_H
#define SX2_H


class Sx2 {
public:
    Sx2() {}
    Sx2(float x, float y);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bool activate,deactivate;
    double start;
    bounding_box_t sx2_box;
    void score_increment();
private:
    VAO *object;
};

#endif // SX2_H
