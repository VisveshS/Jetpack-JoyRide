#include "main.h"
#include "timer.h"
#include "wall.h"
#include "player.h"
#include "coin.h"
#include "fireline.h"
#include "firebeam.h"
#include "balloon.h"
#include "magnet.h"
#include "bmrg.h"
#include "heart.h"
#include "sx2.h"
#include "display.h"
#include "ring.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

// classes to make code in rest of the program neater

class Walls
{
	private:    
		int no_of_walls;
		Wall wall[51];
	public:
		Walls() {}
		Walls(color_t color,int no_of_walls){
			this->no_of_walls=no_of_walls;
			color_t levelColor[5]={COLOR_WHITE,COLOR_YELLOW,COLOR_GREEN,COLOR_RED,COLOR_BLUE};
			for(int i=0;i<this->no_of_walls;i++)        
				wall[i] = Wall(5.99f*float(i), 0, levelColor[(i)/10]);        
		}
		void Draw(glm::mat4 VP) {
			for(int i=0;i<this->no_of_walls;i++)
				wall[i].draw(VP);
		}
		void Move(float x) {
			for(int i=0;i<this->no_of_walls;i++)
				wall[i].position.x+=x;        
		}
		void Tick(){
			for(int i=0;i<this->no_of_walls;i++)
				wall[i].tick();
		}
};

class Coins
{
	private:
		int no_of_coins;
		Coin coin[1000];
	public:
		Coins() {}
		Coins(int no_of_coins){
			this->no_of_coins=no_of_coins;
			float x,y,w,z;
			for(int i=0;i<this->no_of_coins;i++)
			{
				w=(rand()%10);w/=10;
				x=(rand()%48)*6.0f;x+=rand()%6;
				z=(rand()%10);z/=10;               
				y=rand()%6;
				coin[i] = Coin(x+z,y+w,int(rand()%3));
			}
		}
		void Draw(glm::mat4 VP) {
			for(int i=0;i<this->no_of_coins;i++)
				coin[i].draw(VP);
		}
		void Move(float x) {
			for(int i=0;i<this->no_of_coins;i++)
				coin[i].position.x+=x;
		}
		void Tick() {
			for(int i=0;i<this->no_of_coins;i++)
				coin[i].tick();
		}
		void bag_coin(Player player) {
			for(int i=0;i<this->no_of_coins;i++)
			{
				if(detect_collision(player.player_box,coin[i].coin_box)&&!coin[i].bagged)
				{
					coin[i].bagged=true;
					coin[i].score_increment();
				}
			}
		}
};

class Balloons
{
	private:
	public:
		vector<Balloon>balloon;
		bool throwing;
		Balloons(){this->throwing=false;}
		void Draw(glm::mat4 VP) {
			for (vector<Balloon>::iterator i=balloon.begin(); i!=balloon.end(); i++) 
				(*i).draw(VP);        
		}
		void Move(float x) {
			for (vector<Balloon>::iterator i=balloon.begin(); i!=balloon.end(); i++) 
				(*i).position.x+=x;
		}
		void Tick() {
			for (vector<Balloon>::iterator i=balloon.begin(); i!=balloon.end(); i++) 
				(*i).tick();
			// printf("%lu\n\r",balloon.size());
		}
		void Throw(float x, float y, float direction) {
			Balloon temp = Balloon(x,y,direction);
			balloon.push_back(temp);
		}
		void terminateballoon() {
			int j=0;
			for (vector<Balloon>::iterator i=balloon.begin(); i!=balloon.end(); i++,j++) 
				if((*i).position.y<-2.0f)
					balloon.erase(i--);
		}

};

void movePlayer(float x);

class Magnets
{
	private:
		vector<Magnet>magnet;
	public:
		Magnets() {}
		int initmgnt(float x, float y) {
			if(rand()%300)
				return 0;
			float v1,v2,v3;
			Magnet temp;
			y+=rand()%6;
			y=int(y)%6;
			v1=(rand()%10)/10;
			v2=rand()%6-2;
			v3=(rand()%10)/10;
			if(y+v1>1.5)
			{
				temp = Magnet(v2+v3,y+v1);
				magnet.push_back(temp);
			}
			return 0;
		}
		void Draw(glm::mat4 VP)
		{
			for (vector<Magnet>::iterator i=magnet.begin(); i!=magnet.end(); i++)
				(*i).draw(VP);
		}
		void Tick(Player *player,bool enable)
		{
			for (vector<Magnet>::iterator i=magnet.begin(); i!=magnet.end(); i++)
			{
				(*i).tick(player,enable);
				if((*i).birth+(*i).life<glfwGetTime())
					magnet.erase(i--);
			}
		}

		void Move(float x) {
			for (vector<Magnet>::iterator i=magnet.begin(); i!=magnet.end(); i++)                 
				(*i).position.x+=x; 
		}
		void attract(Player *player,bool disable)
		{
			float yes,diff,dir;
			for (vector<Magnet>::iterator i=magnet.begin(); i!=magnet.end(); i++)                 
			{
				yes=1;diff=(*i).position.y-(*player).position.y;
				if(disable)yes=0;
				dir=((*i).position.x)/sqrt((((*i).position.x)*((*i).position.x)+diff*diff));
				movePlayer(yes*dir*0.1/(1+diff*diff+((*i).position.x)*((*i).position.x)));
			}   
		}
};

bool firecollision(bounding_box_t box,float x, float y, float angle, float offset)
{
	float dot1,dot2[5],Cos,Sin,Max,Min;
	Cos=cos(angle * M_PI / 180.0f);
	Sin=sin(angle * M_PI / 180.0f);
	// printf("%f\n",Sin); 
	dot1=Cos*x+Sin*y;
	dot2[0]=(box.x+box.width/2)*Cos+(box.y+box.height/2)*Sin;
	dot2[1]=(box.x+box.width/2)*Cos+(box.y-box.height/2)*Sin;
	dot2[2]=(box.x-box.width/2)*Cos+(box.y+box.height/2)*Sin;
	dot2[3]=(box.x-box.width/2)*Cos+(box.y-box.height/2)*Sin;
	Max=dot2[0];Min=dot2[0];
	for(int i=0;i<4;i++)
	{
		if(Max<dot2[i])Max=dot2[i];
		if(Min>dot2[i])Min=dot2[i];
	}
	if(Max>=dot1-offset&&Min<dot1+offset)//2.1732f
		return true;    
	return false;
}

class Firelines {
	private:
		vector<Fireline>fireline;
	public:
		Firelines() {}
		Firelines(int no_of_firelines) {
			float x,y,w,z,v;
			Fireline temp;
			for (int i=0;i<no_of_firelines;i++) 
			{
				w=(rand()%10);w/=10;
				x=(rand()%48)*6.0f;x+=rand()%6;
				z=(rand()%10);z/=10;               
				y=(rand()%4)+1;
				v=(rand()%7)-3;
				// printf("_%f_\n",v);
				temp = Fireline(x+z,w+y,v);
				fireline.push_back(temp);
			}
		}
		void Draw(glm::mat4 VP) {
			for (vector<Fireline>::iterator i=fireline.begin(); i!=fireline.end(); i++)                 
				(*i).draw(VP);
		}
		void Tick() {
			for (vector<Fireline>::iterator i=fireline.begin(); i!=fireline.end(); i++)                 
				(*i).tick();
		}
		void Move(float x) {
			for (vector<Fireline>::iterator i=fireline.begin(); i!=fireline.end(); i++)                 
				(*i).position.x+=x; 
		}
		void killplayer(Player *player) {
			float dot1,dot2[5],Cos,Sin,Max,Min;
			bool safe=true;
			for (vector<Fireline>::iterator i=fireline.begin(); i!=fireline.end(); i++)                             
			{
				if(firecollision((*player).player_box,(*i).position.x,(*i).position.y,(*i).rotation,2.1f)&&firecollision((*player).player_box,(*i).position.x,(*i).position.y,(*i).rotation+90.0,-0.1f))
				{
					safe=false;
					break;
				}
			}
			if(player->protecc)safe=true;
			if(!safe)
			{
				if((*player).safe)
				{
					printf("fireline collision %d\n",collisions++);
					(*player).safe=false;
				}
			}
			else
				(*player).safe = true;
		}
		void extinguish(Balloons *balloons)
		{

			for (vector<Fireline>::iterator i=fireline.begin(); i!=fireline.end(); i++)
			{
				
				for (vector<Balloon>::iterator j=(*balloons).balloon.begin(); j!=(*balloons).balloon.end(); j++)
				{
				   if(firecollision((*j).balloon_box,(*i).position.x,(*i).position.y,(*i).rotation,2.1f)&&firecollision((*j).balloon_box,(*i).position.x,(*i).position.y,(*i).rotation+90.0,0.1f))
				   {
						printf("extinguished!\n");
						fireline.erase(i--);
						balloons->balloon.erase(j--);
				   }
				}
			}            
		}
};

class Boomerangs {
	private:
		vector<Bmrg>boomerang;
	public:
		bool enable;
		Boomerangs() {enable=true;}
		void initbmrg(float x,float y)
		{
			float z,z1;
			z=rand()%300;z1=rand()%6;
			if(z==0&&enable)
			{
				// Bmrg temp = Bmrg(x,y,COLOR_WHITE);
				boomerang.push_back(Bmrg(x,y,COLOR_WHITE));
			}
		}
		void Draw(glm::mat4 VP)
		{
			for (vector<Bmrg>::iterator i=boomerang.begin(); i!=boomerang.end(); i++)
				(*i).draw(VP);
		}
		void Tick(float limit)
		{
			for (vector<Bmrg>::iterator i=boomerang.begin(); i!=boomerang.end(); i++)
			{    
				(*i).tick();
				if((*i).position.x>=limit)
					boomerang.erase(i--);
			}
		}
		void Move(float x)
		{
			for (vector<Bmrg>::iterator i=boomerang.begin(); i!=boomerang.end(); i++)
				(*i).position.x+=x;            
		}
		void hitplayer(Player *player)
		{
			bool safe=true;
			for (vector<Bmrg>::iterator i=boomerang.begin(); i!=boomerang.end(); i++)                             
			{
				if((firecollision((*player).player_box,(*i).position.x,(*i).position.y,(*i).rotation,0.11f)&&firecollision((*player).player_box,(*i).position.x,(*i).position.y,(*i).rotation+90,0.4f))&&!player->protecc)
				{
					safe=false;
					break;
				}
			}
			if(!safe)
			{
				if((*player).safe1)
				{
					printf("boomerang collision %d\n",collisions++);
					(*player).safe1=false;
				}
			}
			else
				(*player).safe1 = true;
		}
};

class Hearts
{
	private:
		int no_of_heart;
		Heart heart[1000];
	public:
		Hearts() {}
		Hearts(int no_of_heart){
			this->no_of_heart=no_of_heart;
			float x,y,w,z;
			for(int i=0;i<this->no_of_heart;i++)
			{
				w=(rand()%10);w/=10;
				x=(rand()%48)*6.0f;x+=rand()%6;
				z=(rand()%10);z/=10;               
				y=rand()%6;
				heart[i] = Heart(x+z,y+w);
			}
		}
		void Draw(glm::mat4 VP) {
			for(int i=0;i<this->no_of_heart;i++)
				heart[i].draw(VP);
		}
		void Move(float x) {
			for(int i=0;i<this->no_of_heart;i++)
				heart[i].position.x+=x;
		}
		void Tick() {
			for(int i=0;i<this->no_of_heart;i++)
				heart[i].tick();
		}
		void bag_heart(Player player) {
			for(int i=0;i<this->no_of_heart;i++)
			{
				if(detect_collision(player.player_box,heart[i].heart_box)&&!heart[i].bagged)
				{
					heart[i].bagged=true;
					heart[i].score_increment();
					printf("heart\n");
					collisions--;
				}
				// heart[i].bagged=(heart[i].bagged)||(detect_collision(player.player_box,heart[i].heart_box));
			}
		}
};


class Sx2s
{
	private:
		vector<Sx2>sx2;
	public:
		Sx2s() {}
		Sx2s(int no_of_sx2){
			float x,y,w,z;
			for(int i=0;i<no_of_sx2;i++)
			{
				w=(rand()%10);w/=10;
				x=(rand()%48)*6.0f;x+=rand()%6;
				z=(rand()%10);z/=10;               
				y=rand()%6;
				sx2.push_back(Sx2(x+z,y+w));
			}
		}
		void Draw(glm::mat4 VP) {
			for (vector<Sx2>::iterator i=sx2.begin(); i!=sx2.end(); i++)
				(*i).draw(VP);
		}
		void Move(float x) {
			for (vector<Sx2>::iterator i=sx2.begin(); i!=sx2.end(); i++)
				(*i).position.x+=x;
		}
		void Tick() {
			score_multiplier=1;
			for (vector<Sx2>::iterator i=sx2.begin(); i!=sx2.end(); i++)
			{
				(*i).tick();
				if((*i).activate)
				{
					score_multiplier=2;
					break;
				}
			}
		}
		void bag_sx2(Player player) {
			int j=-1,k=0;;
			for (vector<Sx2>::iterator i=sx2.begin(); i!=sx2.end(); i++)
			{
				if(detect_collision(player.player_box,(*i).sx2_box)&&!(*i).activate)
				{
					(*i).activate=true;
					(*i).score_increment();
					printf("sx2\n");
				}			
			}
		}
		void Deactivate()
		{
			for (vector<Sx2>::iterator i=sx2.begin(); i!=sx2.end(); i++)
				if((*i).deactivate)
					this->sx2.erase(i--);
		}
};

class Firebeams
{
	private:
		int times;
		double timer;
		int next;
		vector<Firebeam>firebeam;
	public:
		Firebeams(){}
		Firebeams(int times)
		{
			this->next=8;
			this->times=times;
			this->timer=glfwGetTime();
		}
		void init(Boomerangs *boomerangs) {
			if(this->timer+next<glfwGetTime()&&(this->times)>0)
			{
				firebeam.push_back(Firebeam(2));
				this->next+=10;
				this->timer+=this->next+(rand()%10)/10;
			}
			if(firebeam.size()!=0)
				(*boomerangs).enable=false;
			else
				(*boomerangs).enable=true;
		}
		void Draw(glm::mat4 VP) {
			for(vector<Firebeam>::iterator i=firebeam.begin();i!=firebeam.end();i++)
			{
				if((*i).state==0)
					firebeam.erase(i--);
				else
					(*i).draw(VP);
			}

		}
		void Tick(Player *player) {
			for(vector<Firebeam>::iterator i=firebeam.begin();i!=firebeam.end();i++)
				(*i).tick();
			if(firebeam.size()==1)
			{	
				if(firebeam[0].state==2)
				{
					if((((*player).player_box.y-(*player).player_box.height/2<-0.5||(*player).player_box.y+(*player).player_box.height/2>5.5))&&!player->protecc)
					{
						if((*player).safe2)
							printf("firebeam collisions %d\n",collisions++);
						(*player).safe2=false;
					}
					else
						(*player).safe2=true;
				}
			}
			else
				(*player).safe2=true;

		}
};


class Rings
{
	public:
		float ground;
		Ring ring;
		Rings(){}
		Rings(int no_of_rings) {
			float x,z;
			x=(rand()%44)*6.0f+3.0f;x+=rand()%6;
			z=(rand()%10);z/=10;               
			ring=Ring(x+z,0);
		}
		void Draw(glm::mat4 VP){ring.draw(VP);}
		void Move(float x){ring.position.x+=x;}
		void Tick(float y,drawing_details *D,Player *player)
		{
			if(ring.position.x>-3&&ring.position.x<3)
				(*D).tunnel=true;
			else
				(*D).tunnel=false;
			if(16>(ring.position.x*ring.position.x+(ring.position.y-y)*(ring.position.y-y)))
				(*player).protecc=true;
			else
				(*player).protecc=false;				
		}
};

class DisplayScore
{
	private:
		Display digits[10];
	public:
		DisplayScore(){}
		DisplayScore(float x){
			digits[0]=Display((score%10),-2.3);
			digits[1]=Display(((score/10)%10),-2.7);
			digits[2]=Display(((score/100)%10),-3.1);
			digits[3]=Display(((score/1000)%10),-3.5);

			digits[4]=Display(20-collisions,3);
			digits[5]=Display(20-collisions,2.6);			
		}
		void Draw(glm::mat4 VP) {
			digits[0].draw(VP,score%10);
			digits[1].draw(VP,(score/10)%10);
			digits[2].draw(VP,(score/100)%10);
			digits[3].draw(VP,(score/1000)%10);

			digits[4].draw(VP,(20-collisions)%10);
			digits[5].draw(VP,(20-collisions)/10);

			if(collisions==20)
				quit(window);

		}
};

Walls walls;
Player player;
Coins coins;
Balloons balloons;
Firelines firelines;
Magnets magnets;
Boomerangs bmrgs;
Hearts hearts;
Sx2s sx2s;
Firebeams firebeams;
DisplayScore displayScore;
Rings rings;
drawing_details D;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = +3, absolute_position=0.0f;
double headsize=0.2, coin_radius=0.1;
int score=0;int collisions=0,lives=20,score_multiplier=1;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	glm::vec3 eye (0,0,1);//(1*cos(camera_rotation_angle*M_PI/180.0f), 0, 1*sin(camera_rotation_angle*M_PI/180.0f) );
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	glm::vec3 target (0, 0, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	// Don't change unless you are sure!!
	// Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render
	walls.Draw(VP);
	player.draw(VP,D.player_state);
	coins.Draw(VP);
	balloons.Draw(VP);
	firelines.Draw(VP);
	magnets.Draw(VP);
	hearts.Draw(VP);
	bmrgs.Draw(VP);
	sx2s.Draw(VP);
	firebeams.Draw(VP);
	displayScore.Draw(VP);
	rings.Draw(VP);
}

void movePlayer(float player_velocity) {
	player_velocity*=-1.0f;
	walls.Move(player_velocity);
	coins.Move(player_velocity);
	balloons.Move(player_velocity);
	firelines.Move(player_velocity);
	magnets.Move(player_velocity);
	hearts.Move(player_velocity);
	bmrgs.Move(player_velocity);
	sx2s.Move(player_velocity);
	rings.Move(player_velocity);
}

void tick_input(GLFWwindow *window) {
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int waterballoon  = glfwGetKey(window, GLFW_KEY_SPACE);
	int up = glfwGetKey(window, GLFW_KEY_UP);
	if (left) {
		movePlayer(-0.1f);
		if(D.player_state < 2)
			D.player_state += 2;
		if(D.tunnel)
		{
			rings.ground=sqrt(9.01-(rings.ring.position.x)*(rings.ring.position.x))+0.1;
		}
		else rings.ground=-0.6;
	}
	if(right) {
		movePlayer(0.1f);
		if(D.player_state > 1 && D.player_state < 5)
			D.player_state -= 2;
		if(D.tunnel)
		{
			rings.ground=sqrt(9.01-(rings.ring.position.x)*(rings.ring.position.x))+0.1;
		}
		else rings.ground=-0.6;	
	}
	if(waterballoon == GLFW_PRESS && !balloons.throwing) {
		float direction=1.0f;
		if(D.player_state > 1 && D.player_state < 5)direction=-1.0f;
		balloons.Throw(player.position.x,player.position.y,direction);
		balloons.throwing=true;
	}
	if(waterballoon == GLFW_RELEASE)
	{
		balloons.throwing=false;
	}
	if(up) {
		player.speed.y = 1;
		if(D.player_state%2 == 0)
			D.player_state += 1;
	}
	if(up == GLFW_RELEASE)
	{
		if(D.player_state%2 == 1 && D.player_state < 5)    
			D.player_state -= 1;
	}

	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)screen_center_y+=0.1;
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)screen_center_y-=0.1;
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)screen_center_x-=0.1;
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)screen_center_x+=0.1;
}

void tick_elements() {
	walls.Tick();
	player.tick(rings.ground+0.6);
	coins.bag_coin(player);
	coins.Tick();
	balloons.terminateballoon();
	balloons.Tick();
	firelines.killplayer(&player);
	firelines.extinguish(&balloons);
	firelines.Tick();
	magnets.attract(&player,D.tunnel);
	magnets.initmgnt(player.position.x,player.position.y);
	magnets.Tick(&player,D.tunnel);
	bmrgs.Tick(player.position.x+6.1f);
	bmrgs.hitplayer(&player);
	bmrgs.initbmrg(player.position.x+6.0,((player.position.y>4.5)?player.position.y:player.position.y+2.1));
	hearts.Tick();
	hearts.bag_heart(player);
	sx2s.Tick();
	sx2s.bag_sx2(player);
	sx2s.Deactivate();
	firebeams.init(&bmrgs);
	firebeams.Tick(&player);
	rings.Tick(player.position.y,&D,&player);
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models

	walls       = Walls(COLOR_GREEN,50);
	coins       = Coins(200);
	player      = Player(0, 2.0f, COLOR_MAGENTA);
	firelines    = Firelines(30);
	firebeams     = Firebeams(10);
	hearts       = Hearts(10);
	sx2s 		= Sx2s(5);
	displayScore 	= DisplayScore(-3.5);
	rings 			= Rings(3);
	D.player_state=0;D.tunnel=false;

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (0.0f , 0.0f , 0.0f, 1.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
	srand(time(0));
	int width  = 650;
	int height = 650;

	window = initGLFW(width, height);

	initGL (window, width, height);

	/* Draw in loop */
	while (!glfwWindowShouldClose(window)) {
		// Process timers

		if (t60.processTick()) {
			// 60 fps
			// OpenGL Draw commands
			draw();
			// Swap Frame Buffer in double buffering
			glfwSwapBuffers(window);
			reshapeWindow (window, width, height);

			tick_elements();
			tick_input(window);
		}

		// Poll for Keyboard and mouse events
		glfwPollEvents();
	}

	quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
	return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
		   (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
	float top    = screen_center_y + 4 / screen_zoom;
	float bottom = screen_center_y - 4 / screen_zoom;
	float left   = screen_center_x - 4 / screen_zoom;
	float right  = screen_center_x + 4 / screen_zoom;
	Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
