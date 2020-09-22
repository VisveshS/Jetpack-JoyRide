#include "magnet.h"
#include "main.h"
#include "math.h"

#define magnet_size 0.1

Magnet::Magnet(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->birth=glfwGetTime();
    this->life=rand()%5+2;//ranges from 2 to 6 seconds
    GLfloat vertex_buffer_data[]={
        -1.0f*magnet_size, 4.0f*magnet_size, 0.0f,
        0.0f*magnet_size, 4.0f*magnet_size, 0.0f,
        -1.0f*magnet_size, 1.0f*magnet_size, 0.0f,
        0.0f*magnet_size, 1.0f*magnet_size, 0.0f,
        0.0f*magnet_size, 0.0f*magnet_size, 0.0f,
        1.0f*magnet_size, 1.0f*magnet_size, 0.0f,
        1.0f*magnet_size, 0.0f*magnet_size, 0.0f,
        2.0f*magnet_size, 1.0f*magnet_size, 0.0f,
        1.0f*magnet_size, 4.0f*magnet_size, 0.0f,
        2.0f*magnet_size, 4.0f*magnet_size, 0.0f,
    };
    this->object = create3DObject(GL_TRIANGLE_STRIP, 10, vertex_buffer_data, COLOR_RED, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Magnet::tick(Player *player,bool disable) {
    float yes=1,diff=this->position.y-(*player).position.y,dir=(diff)/sqrt(((this->position.x)*(this->position.x)+diff*diff));
    if((this->position.x>4.0||this->position.x<-4.0)&&disable)yes=0;
    (*player).speed.y += yes*dir*0.5/(1+diff*diff+(this->position.x)*(this->position.x));
}

