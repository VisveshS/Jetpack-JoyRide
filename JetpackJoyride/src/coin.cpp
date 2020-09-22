#include "coin.h"
#include "main.h"

Coin::Coin(float x, float y, int type) {
    this->position = glm::vec3(x, y, 0);
    this->type=type;
    color_t palette[4]={COLOR_GREEN,COLOR_RED,COLOR_CYAN};
    GLfloat vertex_buffer_data[300],vertex_buffer_data1[300];
    GLfloat color_buffer_data[300];
    int i,n=31;
    for(i=0;i<n*9;i+=9)
    {
        vertex_buffer_data[(int)(i)]      =   cos((2*i*M_PI)/n)*coin_radius;
        vertex_buffer_data[(int)(i+1)]    =   sin((2*i*M_PI)/n)*coin_radius;
        vertex_buffer_data[(int)(i+2)]    =   0;

        vertex_buffer_data[(int)(i+3)]    =   cos((2*(i+1)*M_PI)/n)*coin_radius;
        vertex_buffer_data[(int)(i+4)]    =   sin((2*(i+1)*M_PI)/n)*coin_radius;
        vertex_buffer_data[(int)(i+5)]    =   0;

        vertex_buffer_data[(int)(i+6)]    =   0;
        vertex_buffer_data[(int)(i+7)]    =   0;
        vertex_buffer_data[(int)(i+8)]    =   0;

    }
    for(i=0;i<n*9;i+=9)
    {
        vertex_buffer_data1[(int)(i)]      =   cos((2*i*M_PI)/n)*1.5*coin_radius;
        vertex_buffer_data1[(int)(i+1)]    =   sin((2*i*M_PI)/n)*1.5*coin_radius;
        vertex_buffer_data1[(int)(i+2)]    =   0;

        vertex_buffer_data1[(int)(i+3)]    =   cos((2*(i+1)*M_PI)/n)*1.5*coin_radius;
        vertex_buffer_data1[(int)(i+4)]    =   sin((2*(i+1)*M_PI)/n)*1.5*coin_radius;
        vertex_buffer_data[(int)(i+5)]    =   0;

        vertex_buffer_data1[(int)(i+6)]    =   0;
        vertex_buffer_data1[(int)(i+7)]    =   0;
        vertex_buffer_data1[(int)(i+8)]    =   0;

    }
    for(i=0;i<n*9;i+=9)
    {
        color_buffer_data[(int)(i+0)]    =   0.2*palette[type].r/255;
        color_buffer_data[(int)(i+1)]    =   0.2*palette[type].g/255;
        color_buffer_data[(int)(i+2)]    =   0.2*palette[type].b/255;


        color_buffer_data[(int)(i+3)]    =   0.2*palette[type].r/255;
        color_buffer_data[(int)(i+4)]    =   0.2*palette[type].g/255;
        color_buffer_data[(int)(i+5)]    =   0.2*palette[type].b/255;

        color_buffer_data[(int)(i+6)]    =   1.0*palette[type].r/255;
        color_buffer_data[(int)(i+7)]    =   1.0*palette[type].g/255;
        color_buffer_data[(int)(i+8)]    =   1.0*palette[type].b/255;

    }
    this->bagged=false;
    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color_buffer_data, GL_FILL);
    this->objectbig = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data1, color_buffer_data, GL_FILL);
}

void Coin::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    if(!this->bagged)
    {
        if(score_multiplier==1)
        {    draw3DObject(this->object);this->coin_box.width*=1.5;this->coin_box.height*=1.5;}
        else
        {    draw3DObject(this->objectbig);this->coin_box.width*=1.0;this->coin_box.height*=1.0;}
    }
}

void Coin::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Coin::tick() {
    this->coin_box.x=this->position.x;
    this->coin_box.y=this->position.y;
    this->coin_box.width=coin_radius;
    this->coin_box.height=coin_radius;
}

void Coin::score_increment()
{
    int coin_value[4]={5,7,10};
    score+=coin_value[this->type]*score_multiplier;
    printf("%d\n", score);
}