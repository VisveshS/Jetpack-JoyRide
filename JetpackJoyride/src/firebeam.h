#include "main.h"

#ifndef FIREBEAM_H
#define FIREBEAM_H


class Firebeam {
public:
    Firebeam(){};
    Firebeam(int i);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double birth;
    int state;//0->before,	1->warn,	2->laser
private:
    VAO *object;
    VAO *laser;
};

#endif // FIREBEAM_H
