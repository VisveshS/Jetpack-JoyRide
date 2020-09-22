#include "fireline.h"
#include "main.h"

#define hex_size 0.2f
#define len 20.0f

Fireline::Fireline(float x, float y, float rotation_direction) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 90;
    this->rotation_direction=rotation_direction;
    static const GLfloat vertex_buffer_data[] = {
        //hexagon1
        hex_size*0.0f,     hex_size*0.0f, 0.0f,
        hex_size*0.866f,   hex_size*0.5f, 0.0f,
        hex_size*0.866f,   hex_size*1.5f, 0.0f,

        hex_size*0.0f,     hex_size*0.0f, 0.0f,
        hex_size*0.866f,   hex_size*1.5f, 0.0f,
        hex_size*0.0f,     hex_size*2.0f, 0.0f,

        hex_size*0.0f,     hex_size*0.0f, 0.0f,
        hex_size*0.0f,     hex_size*2.0f, 0.0f,
        hex_size*-0.866f,  hex_size*1.5f, 0.0f,

        hex_size*0.0f,     hex_size*0.0f, 0.0f,
        hex_size*-0.866f,  hex_size*1.5f, 0.0f,
        hex_size*-0.866f,  hex_size*0.5f, 0.0f,

        // //hexagon2
        hex_size*(0.0f+len),  hex_size*0.0f, 0.0f,
        hex_size*(0.866f+len),   hex_size*0.5f, 0.0f,
        hex_size*(0.866f+len),   hex_size*1.5f, 0.0f,

        hex_size*(0.0f+len),     hex_size*0.0f, 0.0f,
        hex_size*(0.866f+len),   hex_size*1.5f, 0.0f,
        hex_size*(0.0f+len),     hex_size*2.0f, 0.0f,

        hex_size*(0.0f+len),     hex_size*0.0f, 0.0f,
        hex_size*(0.0f+len),     hex_size*2.0f, 0.0f,
        hex_size*(-0.866f+len),  hex_size*1.5f, 0.0f,

        hex_size*(0.0f+len),     hex_size*0.0f, 0.0f,
        hex_size*(-0.866f+len),  hex_size*1.5f, 0.0f,
        hex_size*(-0.866f+len),  hex_size*0.5f, 0.0f,

        // //connecting line
        hex_size*(0.866f), hex_size*(0.8f), 0.0f,
        hex_size*(0.866f), hex_size*(1.2f), 0.0f,
        hex_size*(0.866f+len),hex_size*(1.2f), 0.0f,

        hex_size*(0.866f+len), hex_size*0.8f, 0.0f,
        hex_size*(0.866f+len), hex_size*1.2f, 0.0f,
        hex_size*(0.866),      hex_size*0.8f, 0.0f,
    };

    this->object = create3DObject(GL_TRIANGLES, 10*3, vertex_buffer_data, COLOR_YELLOW, GL_FILL);
}

void Fireline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3((-10.866)*hex_size, -1*hex_size, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Fireline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

// void 

void Fireline::tick() {
    this->rotation +=this->rotation_direction/3;
}

