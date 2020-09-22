#include "heart.h"
#include "main.h"
#define heart_size 0.16

Heart::Heart(float x, float y) {
	this->position = glm::vec3(x, y, 0);
	this->bagged=false;
	color_t color={255, 60, 60};
	static const GLfloat vertex_buffer_data[] = {
		0*heart_size,0*heart_size,0,
		1*heart_size,1*heart_size,0,
		1*heart_size,1.5*heart_size,0,
		0.5*heart_size,2*heart_size,0,
		0*heart_size,1.5*heart_size,0,
		-0.5*heart_size,2*heart_size,0,
		-1*heart_size,1.5*heart_size,0,
		-1*heart_size,1*heart_size,0,
   };
	this->object = create3DObject(GL_TRIANGLE_FAN, 8, vertex_buffer_data, color, GL_FILL);
}

void Heart::draw(glm::mat4 VP) {
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translate = glm::translate (this->position);    // glTranslatef
	Matrices.model *= (translate);
	glm::mat4 MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	if(!this->bagged)
		draw3DObject(this->object);
}

void Heart::set_position(float x, float y) {
	this->position = glm::vec3(x, y, 0);
}

void Heart::tick() {
	this->heart_box.x=this->position.x;
	this->heart_box.y=this->position.y+1*heart_size;
	this->heart_box.width=2*heart_size;
	this->heart_box.height=2*heart_size;
}

void Heart::score_increment()
{

}

