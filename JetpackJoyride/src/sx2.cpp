#include "sx2.h"
#include "main.h"
#define sx2_size 0.16

Sx2::Sx2(float x, float y) {
	this->position = glm::vec3(x, y, 0);
	this->activate=false;
	this->deactivate=false;
	this->start=-1.0;
	color_t color={0, 0, 55};
	static const GLfloat vertex_buffer_data[] = {
	1*sx2_size,1*sx2_size,0,
	-1*sx2_size,1*sx2_size,0,
	0*sx2_size,2*sx2_size,0,

	0.5*sx2_size,0*sx2_size,0,
	0.5*sx2_size,1*sx2_size,0,
	-0.5*sx2_size,0*sx2_size,0,

	-0.5*sx2_size,0*sx2_size,0,
	0.5*sx2_size,1*sx2_size,0,
	-0.5*sx2_size,1*sx2_size,0,
   };
	this->object = create3DObject(GL_TRIANGLES, 3*3, vertex_buffer_data, color, GL_FILL);
}

void Sx2::draw(glm::mat4 VP) {
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translate = glm::translate (this->position);    // glTranslatef
	Matrices.model *= (translate);
	glm::mat4 MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	if(!this->activate)
		draw3DObject(this->object);
}

void Sx2::set_position(float x, float y) {
	this->position = glm::vec3(x, y, 0);
}

void Sx2::tick() {
	this->sx2_box.x=this->position.x;
	this->sx2_box.y=this->position.y+1*sx2_size;
	this->sx2_box.width=2*sx2_size;
	this->sx2_box.height=2*sx2_size;
	if(this->activate&&this->start==-1.0f)
	{
		this->start=glfwGetTime();
		printf("activate 2x bonus\n");
	}
	if(this->activate&&(this->start+5.0f)<glfwGetTime())
	{
		this->deactivate=true;
		printf("deactivate 2x bonus\n");
	}
}

void Sx2::score_increment()
{

}
