#include "main.h"

#ifndef DISPLAY_H
#define DISPLAY_H


class Display {
public:
    Display() {}
    Display(int n,float pos);
    glm::vec3 position;
    void draw(glm::mat4 VP,int n);
    void set_position(float x, float y);
    void tick();
    int n;
private:
    VAO *up;
    VAO *middle;
    VAO *down;
    VAO *upleft;
    VAO *upright;
    VAO *downleft;
    VAO *downright;
};

#endif // DISPLAY_H
