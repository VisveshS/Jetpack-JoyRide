#include "ring.h"
#include "main.h"
#include "math.h"

#define ring_size 4

Ring::Ring(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->birth=glfwGetTime();
    this->life=rand()%5+2;//ranges from 2 to 6 seconds
    int n=31,i;
    GLfloat vertex_buffer_data[200];
    for(i=0;i<16;i++)
    {
        vertex_buffer_data[3*i]=cos((2*i*M_PI)/n)*ring_size;
        vertex_buffer_data[3*i+1]=sin((2*i*M_PI)/n)*ring_size;
        vertex_buffer_data[3*i+2]=0;
    }

    GLfloat vertex_buffer_data2[200];
    for(i=-3;i<19;i++)
    {
        vertex_buffer_data2[3*i+9]=cos((2*i*M_PI)/n)*(ring_size-1);
        vertex_buffer_data2[3*i+10]=sin((2*i*M_PI)/n)*(ring_size-1);
        vertex_buffer_data2[3*i+11]=0;
    }

    this->object = create3DObject(GL_LINE_STRIP, 16, vertex_buffer_data, COLOR_BLACK, GL_FILL);
    this->object2 = create3DObject(GL_LINE_STRIP, 22, vertex_buffer_data2, COLOR_BLACK, GL_FILL);
}

void Ring::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object2);
}

void Ring::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ring::tick(Player *player) {
    // float yes=1,diff=this->position.y-(*player).position.y,dir=(diff)/sqrt(((this->position.x)*(this->position.x)+diff*diff));
    // if(this->position.x>4.0||this->position.x<-4.0)yes=0;
    // (*player).speed.y += yes*dir*0.5/(1+diff*diff+(this->position.x)*(this->position.x));
}

