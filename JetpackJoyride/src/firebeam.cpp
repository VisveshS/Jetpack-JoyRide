#include "firebeam.h"
#include "main.h"
#define beam_size 0.3

Firebeam::Firebeam(int i) {
    this->position = glm::vec3(0, 0, 0);
    GLfloat vertex_buffer_data[]={
        -2,-1*beam_size,0,
        -1,0*beam_size,0,
        -1,-2*beam_size,0,

        3,-1*beam_size,0,
        2,0*beam_size,0,
        2,-2*beam_size,0,  

        -2,6+-1*beam_size,0,
        -1,6+0*beam_size,0,
        -1,6+-2*beam_size,0,

        3,6+-1*beam_size,0,
        2,6+0*beam_size,0,
        2,6+-2*beam_size,0,          
    };
    GLfloat vertex_buffer_data_l[]={
        -1,-0.5*beam_size,0,
        -1,-1.5*beam_size,0,
        2,-0.5*beam_size,0,

        2,-0.5*beam_size,0,
        -1,-1.5*beam_size,0,
        2,-1.5*beam_size,0,

        -1,6+-0.5*beam_size,0,
        -1,6+-1.5*beam_size,0,
        2,6+-0.5*beam_size,0,

        2,6+-0.5*beam_size,0,
        -1,6+-1.5*beam_size,0,
        2,6+-1.5*beam_size,0,
    };
    this->birth = glfwGetTime();
    this->state = 1;
    this->object = create3DObject(GL_TRIANGLES  , 4*3, vertex_buffer_data, COLOR_BLACK, GL_FILL);
    this->laser = create3DObject(GL_TRIANGLES  , 4*3, vertex_buffer_data_l, COLOR_YELLOW, GL_FILL);    
}

void Firebeam::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(this->state!=0)
        draw3DObject(this->object);
    if(this->state==2)
        draw3DObject(this->laser);
}

void Firebeam::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Firebeam::tick() {
    if(this->state==1)
    {
        if(this->birth+2.0<glfwGetTime())
        {
            this->state=2;
        }
    }
    if(this->state==2)
    {
        if(this->birth+6.0<glfwGetTime())
        {
            this->state=0;
        }
    }
}

