#include "bmrg.h"
#include "main.h"
#define size 0.1
#define pseudogravity 0.005

Bmrg::Bmrg(float x, float y,color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->speed = glm::vec3(-0.3, -0.01, 0);
    // this->speed = glm::vec3(0, 0, 0);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        3*size, 5*size, 0,
        -1*size, 2*size, 0,
        1*size, 2*size, 0,
        -1*size, -2*size, 0,
        1*size, -2*size, 0,
        3*size, -5*size, 0,
    };

    this->object = create3DObject(GL_TRIANGLE_STRIP, 6, vertex_buffer_data, color, GL_FILL);
}

void Bmrg::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Bmrg::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Bmrg::tick() {
    this->rotation += 6.0f;
    this->position.x += this->speed.x;
    this->speed.x += pseudogravity;
    this->position.y += this->speed.y;
}

