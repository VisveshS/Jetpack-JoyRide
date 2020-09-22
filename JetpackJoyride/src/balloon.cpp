#include "balloon.h"
#include "main.h"

#define gravity 0.1

Balloon::Balloon(float x, float y, float direction) {
    this->position = glm::vec3(x, y, 0.1);
    this->speed.x = 0.1;
    GLfloat vertex_buffer_data[100];
    vertex_buffer_data[0]=0.0f;
    vertex_buffer_data[1]=0.0f;
    vertex_buffer_data[2]=0.0f;

    int i,n=31;for(i=0;i<=n;i++)
    {
        vertex_buffer_data[3*i+3]=cos((2*i*M_PI/n))*headsize/2;
        vertex_buffer_data[3*i+4]=sin((2*i*M_PI/n))*headsize/2;
        vertex_buffer_data[3*i+5]=0.0f;        
    }

    this->object = create3DObject(GL_TRIANGLE_FAN, n+2, vertex_buffer_data, COLOR_LIGHTBLUE, GL_FILL);
    this->speed.x=0.2*direction;
    this->speed.y=0;
}

void Balloon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate );
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Balloon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Balloon::tick() {
    this->balloon_box.x=this->position.x;
    this->balloon_box.y=this->position.y;
    this->balloon_box.width=headsize;
    this->balloon_box.height=headsize;
    this->position.x += this->speed.x;
    this->position.y += this->speed.y/10;
    this->speed.y -= gravity;
}

