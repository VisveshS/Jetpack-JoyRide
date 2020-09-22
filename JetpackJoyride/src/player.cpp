#include "player.h"
#include "main.h"

#define gravity 0.1

double pi=3.14159265359;
	
Player::Player(float x, float y, color_t color) {
	this->position = glm::vec3(x, y, 0);
	this->speed.x = 0;
	this->speed.y = 0;
	this->safe = true;
	this->safe1 = true;
	this->safe2 = true;
	this->protecc = true;
	// head = circle , body = some shape , jetpack = triangle , fire triangle
	// 31 triangles for head, 4 for body 1 for jetpack , 1 for fire
	GLfloat g_vertex_buffer_data[9009],g_vertex_buffer_data_f[10];

	int i,n=31;
	for(i=0;i<n*9;i+=9)
	{
		g_vertex_buffer_data[(int)(i)]      =   cos((2*i*pi)/n)*headsize;
		g_vertex_buffer_data[(int)(i+1)]    =   sin((2*i*pi)/n)*headsize;
		g_vertex_buffer_data[(int)(i+2)]    =   0;

		g_vertex_buffer_data[(int)(i+3)]    =   cos((2*(i+1)*pi)/n)*headsize;
		g_vertex_buffer_data[(int)(i+4)]    =   sin((2*(i+1)*pi)/n)*headsize;
		g_vertex_buffer_data[(int)(i+5)]    =   0;

		g_vertex_buffer_data[(int)(i+6)]    =   0;
		g_vertex_buffer_data[(int)(i+7)]    =   0;
		g_vertex_buffer_data[(int)(i+8)]    =   0;

	}
	//hand 1
	g_vertex_buffer_data[9*n]=-1*headsize/3;    g_vertex_buffer_data[n*9+1]=headsize/5-headsize;   g_vertex_buffer_data[n*9+2]=0;
	g_vertex_buffer_data[9*n+3]=-1*headsize/3;    g_vertex_buffer_data[n*9+4]=-4*headsize/5-headsize;   g_vertex_buffer_data[n*9+5]=0;
	g_vertex_buffer_data[9*n+6]=3*headsize/2;    g_vertex_buffer_data[n*9+7]=-4*headsize/5-headsize;   g_vertex_buffer_data[n*9+8]=0;

	//hand 2
	g_vertex_buffer_data[9*(n+1)]=-1*headsize/3;    g_vertex_buffer_data[(n+1)*9+1]=headsize/5-headsize;   g_vertex_buffer_data[(n+1)*9+2]=0;
	g_vertex_buffer_data[9*(n+1)+3]=-1*headsize/3;    g_vertex_buffer_data[(n+1)*9+4]=-4*headsize/5-headsize;   g_vertex_buffer_data[(n+1)*9+5]=0;
	g_vertex_buffer_data[9*(n+1)+6]=3*headsize/2;    g_vertex_buffer_data[(n+1)*9+7]=-headsize;   g_vertex_buffer_data[(n+1)*9+8]=0;

	//leg 1
	g_vertex_buffer_data[9*(n+2)]=-1*headsize/3;    g_vertex_buffer_data[(n+2)*9+1]=headsize/5-headsize;   g_vertex_buffer_data[(n+2)*9+2]=0;
	g_vertex_buffer_data[9*(n+2)+3]=-1*headsize/3;    g_vertex_buffer_data[(n+2)*9+4]=headsize/5-5*headsize/2;   g_vertex_buffer_data[(n+2)*9+5]=0;
	g_vertex_buffer_data[9*(n+2)+6]=3*headsize/2;    g_vertex_buffer_data[(n+2)*9+7]=headsize/5-4*headsize;   g_vertex_buffer_data[(n+2)*9+8]=0;

	//leg2
	g_vertex_buffer_data[9*(n+3)]=-1*headsize/3;    g_vertex_buffer_data[(n+3)*9+1]=headsize/5-headsize;   g_vertex_buffer_data[(n+3)*9+2]=0;
	g_vertex_buffer_data[9*(n+3)+3]=1*headsize/3;    g_vertex_buffer_data[(n+3)*9+4]=headsize/5-3*headsize;   g_vertex_buffer_data[(n+3)*9+5]=0;
	g_vertex_buffer_data[9*(n+3)+6]=-1*headsize/3;    g_vertex_buffer_data[(n+3)*9+7]=headsize/5-4*headsize;   g_vertex_buffer_data[(n+3)*9+8]=0;

	//jetpack
	g_vertex_buffer_data[9*(n+4)]=-1*headsize/2;    g_vertex_buffer_data[(n+4)*9+1]=-1*headsize;   g_vertex_buffer_data[(n+4)*9+2]=0;
	g_vertex_buffer_data[9*(n+4)+3]=-1*headsize/2;    g_vertex_buffer_data[(n+4)*9+4]=-2.5*headsize;   g_vertex_buffer_data[(n+4)*9+5]=0;
	g_vertex_buffer_data[9*(n+4)+6]=-1*headsize;    g_vertex_buffer_data[(n+4)*9+7]=-2.5*headsize;   g_vertex_buffer_data[(n+4)*9+8]=0;

	//fire
	g_vertex_buffer_data_f[0]=-3*headsize/4;    g_vertex_buffer_data_f[1]=-2.5*headsize;   g_vertex_buffer_data_f[2]=0;
	g_vertex_buffer_data_f[3]=-1*headsize/2;    g_vertex_buffer_data_f[4]=-4*headsize;   g_vertex_buffer_data_f[5]=0;
	g_vertex_buffer_data_f[6]=-1*headsize;    g_vertex_buffer_data_f[7]=-4*headsize;   g_vertex_buffer_data_f[8]=0;


	this->object = create3DObject(GL_TRIANGLES, (n+5)*3, g_vertex_buffer_data, color, GL_FILL);   
	this->fire = create3DObject(GL_TRIANGLES, 1*3, g_vertex_buffer_data_f, COLOR_RED, GL_FILL);   
	this->burnt = create3DObject(GL_TRIANGLES, (n+5)*3, g_vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Player::draw(glm::mat4 VP,int state) {
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translate = glm::translate (this->position);    // glTranslatef
	glm::mat4 rotate    = glm::rotate((float) (M_PI / 1.0f), glm::vec3(0, 1, 0));
	Matrices.model *= (translate);
	if(state == 2 || state == 3)
		Matrices.model *= (rotate);
	glm::mat4 MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	if((this->safe&&this->safe1&&this->safe2)||this->protecc)
		draw3DObject(this->object);
	if(state == 1|| state ==3)
		draw3DObject(this->fire);
	if(!this->protecc&&(!this->safe||!this->safe1||!this->safe2))
		draw3DObject(this->burnt);
}

void Player::set_position(float x, float y) {
	this->position = glm::vec3(x, y, 0);
}

void Player::tick(float ground) {
	
	{	this->position.y += (this->speed.y)/10;
		if(this->position.y > ground)
			this->speed.y -= gravity;
		if(this->position.y < ground)
		{
			this->position.y = ground;
			this->speed.y = 0;
		}
		if(this->position.y > 6 )
		{
			this->position.y = 6;
		}
	}
	this->player_box.x=this->position.x;
	this->player_box.y=this->position.y-headsize*3/2;
	this->player_box.width=headsize*2.5f;
	this->player_box.height=headsize*5.0f;
}

