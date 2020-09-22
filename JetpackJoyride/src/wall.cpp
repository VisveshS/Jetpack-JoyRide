#include "wall.h"
#include "main.h"

Wall::Wall(float x, float y, color_t color) {
    this->position = glm::vec3(x, y+0.2, -1);
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        0.0f, -1.0f, 0.0f,
        0.0f, 6.0f, 0.0f,
        2.0f, -1.0f, 0.0f, 
        2.0f, 6.0f, 0.0f,
        0.0f, 6.0f, 0.0f,
        2.0f, -1.0f, 0.0f,

        -2.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 
        0.0f, 6.0f, 0.0f,
        -2.0f, -1.0f, 0.0f,
        -2.0f, 6.0f, 0.0f,        
        0.0f, 6.0f, 0.0f,

        2.0f, -1.0f, 0.0f,
        2.0f, 6.0f, 0.0f,
        4.0f, -1.0f, 0.0f, 
        4.0f, 6.0f, 0.0f,
        2.0f, 6.0f, 0.0f,
        4.0f, -1.0f, 0.0f,

    };
    GLfloat g_color_buffer_data[] ={
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,

        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*0.8f, color.g/255*0.8f, color.b/255*0.8f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,

        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,
        color.r/255*0.8f, color.g/255*0.8f, color.b/255*0.8f,
        color.r/255*1.0f, color.g/255*1.0f, color.b/255*1.0f,
        color.r/255*0.1f, color.g/255*0.1f, color.b/255*0.1f,

    };

    static const GLfloat vertex_buffer_data_p[] = {
        -4,-1,1,
        -4,-1.1,1,
        4,-1,1,
        4,-1.1,1,
    };

    GLfloat g_color_buffer_data_p[] = {
        1,1,1,
        0,0,0,
        1,1,1,
        0,0,0,
    };

    static const GLfloat vertex_buffer_data_d[] = {
        -4,6,1,
        -4,8,1,
        4,6,1,
        4,8,1,
    };

    this->object = create3DObject(GL_TRIANGLES, 6*3, vertex_buffer_data, g_color_buffer_data, GL_FILL);
    this->platform = create3DObject(GL_TRIANGLE_STRIP, 4, vertex_buffer_data_p, g_color_buffer_data_p, GL_FILL);
    this->ceiling = create3DObject(GL_TRIANGLE_STRIP, 4, vertex_buffer_data_d, g_color_buffer_data_p, GL_FILL);    
    this->Ceiling = create3DObject(GL_TRIANGLE_STRIP, 4, vertex_buffer_data_d,COLOR_ORANGE, GL_FILL);
}

void Wall::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->platform);
    if(score_multiplier==1)
        draw3DObject(this->ceiling);
    else
        draw3DObject(this->Ceiling);
}

void Wall::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Wall::tick() {
    // printf("\n");
}

