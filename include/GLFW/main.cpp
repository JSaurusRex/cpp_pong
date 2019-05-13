#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

using namespace std;

int screenx, screeny;

double player_y, enemy_y;
double ball_x, ball_y;
double ball_sx, ball_sy;
bool enemy = true;
int xscale, yscale;
int score1, score2;
char lastletter = 0;
int state=  0;
int menuitem = 0;
bool shouldexit = false;
int restate = 0;

bool keys[500];
GLFWwindow* window;

double delta;
double start;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	char lastletter = key;
	cout << key << ' ' << lastletter << endl;
	
	if(action == GLFW_PRESS) {keys[key] = true;}
	if(action == GLFW_RELEASE) {keys[key] = false; lastletter = 0;}
	if(state != 1 && action == GLFW_PRESS && (key == 265 || key == 87)) menuitem++;
	else if(state !=1 && action == GLFW_PRESS && (key == 264 || key == 83)) menuitem--;
	else if(state ==0 && action == GLFW_PRESS && (key == 257 || key == 32)) {
		switch(menuitem){
			case 0:
				state = 1;
			break;
			case 2:
				state = 3;
			break;
			case 1:
				state = -1;
				cout << "shutting down" << endl;
			break;
				
		}
	}else if(state ==3 && action == GLFW_PRESS && (key == 257 || key == 32)) {
		switch(menuitem){
			case 0:
				state = 4;
				restate = 0;
			break;
			case 2:
				//nothing
			break;
			case 1:
				state = 0;
			break;
				
		}
	}
}

void ball () {
	float ball_speed;

	
	if(ball_y < 0 || ball_y > screeny - xscale * 4) ball_sy = -ball_sy;
	
	if(ball_x + (ball_sx * xscale * delta) > xscale * 90 && ball_x < xscale * 97 && ball_y - enemy_y > 0 - 4 * yscale && ball_y - (enemy_y + 15 * yscale) < 0) {
		ball_x = xscale*90;
		ball_sx = -ball_sx * 1.1;
		ball_sy = (ball_y - (enemy_y + (yscale * 7.5)));
	}
	if(ball_x + ball_sx * xscale * delta < xscale * 6 && ball_x > xscale * 3 &&ball_y - player_y > 0 - 4 * yscale && ball_y - (player_y + 15 * yscale) < 0) {
		ball_x = xscale*6;
		ball_sx = -ball_sx * 1.15;
		ball_sy = (ball_y - (player_y + (yscale * 7.5)));
	}
	ball_x += ball_sx * xscale * delta;
	ball_y += ball_sy * yscale * delta;
	
	if(ball_x + xscale * 4 < 0) {
		score1++;
		ball_x = screenx / 2;
		ball_y = screeny / 2;
		ball_sx = 16;
		ball_sy = 0;
		player_y = screeny /2;
		enemy_y = screeny /2;
	}
	if(ball_x > screenx) {
		score2++;
		ball_x = screenx / 2;
		ball_y = screeny / 2;
		ball_sx = 16;
		ball_sy = 0;
		player_y = screeny /2;
		enemy_y = screeny /2;
	}
	//cout << ball_sx << endl;
}


void render (float x1, float x2, float y1, float y2) {
	glBegin(GL_TRIANGLES);
		glVertex3f( x1 , y1 , 0.0);
		glVertex3f( x2 , y1 , 0.0);
		glVertex3f( x2 , y2 , 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);
		glVertex3f( x1 , y2 , 0.0);
		glVertex3f( x1 , y1 , 0.0);
		glVertex3f( x2 , y2 , 0.0);
	glEnd();
}

void text (float x, float y, float scale, char letter)
{
	x /= scale;
	y /= scale;
	bool textsides[7];
	textsides[0] = false;
	textsides[1] = false;
	textsides[2] = false;
	textsides[3] = false;
	textsides[4] = false;
	textsides[5] = false;
	textsides[6] = false;
	
	switch(letter){
		case '0':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
		break;
		case '1':
			textsides[3] = true;
			textsides[4] = true;
		break;
		case '2':
			textsides[0] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case '3':
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case '4':
			textsides[1] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case '5':
			textsides[1] = true;
			textsides[2] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case '6':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case '7':
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
		break;
		case '8':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case '9':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'a':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case 'b':
			textsides[0] = true;
			textsides[1] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'c':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[5] = true;
		break;
		case 'd':
			textsides[0] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'e':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'f':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[6] = true;
		break;
		case 'g':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[4] = true;
			textsides[5] = true;
		break;
		case 'h':
			textsides[0] = true;
			textsides[1] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case 'i':
			textsides[0] = true;
			textsides[1] = true;
		break;
		case 'j':
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
		break;
		case 'k':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case 'l':
			textsides[0] = true;
			textsides[1] = true;
			textsides[5] = true;
		break;
		case 'm':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
		break;
		case 'n':
			textsides[0] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case 'o':
			textsides[0] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'p':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[6] = true;
		break;
		case 'q':
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case 'r':
			textsides[0] = true;
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[6] = true;
		break;
		case 's':
			textsides[1] = true;
			textsides[2] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 't':
			textsides[0] = true;
			textsides[1] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'u':
			textsides[0] = true;
			textsides[4] = true;
			textsides[5] = true;
		break;
		case 'v':
			textsides[0] = true;
			textsides[1] = true;
			textsides[4] = true;
			textsides[5] = true;
		break;
		case 'w':
			textsides[0] = true;
			textsides[1] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
		break;
		case 'x':
			textsides[0] = true;
			textsides[1] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[6] = true;
		break;
		case 'y':
			textsides[1] = true;
			textsides[3] = true;
			textsides[4] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
		case 'z':
			textsides[0] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[5] = true;
			textsides[6] = true;
		break;
	}
	
	/*	_
		2
	1|	_	|3
		6
	0|	_	|4
		5
	*/
	
	
	//cout << textsides[0] << " " << textsides[1] << " " << textsides[2] << " " << textsides[3] << " " << textsides[4] << " " << textsides[5] << " " << textsides[6] << endl;
	if(textsides[0]){
		render((x * xscale * scale), (x + 1) * xscale * scale, (y+1) * yscale * scale, (y + 3) * yscale * scale);
	}
	if(textsides[1]){
		render(x * xscale* scale, (x + 1) * xscale* scale, (y+4) * yscale* scale, (y + 6)* yscale * scale);
	}
	if(textsides[2]){
		render((x+1) * xscale* scale, (x + 3) * xscale* scale, (y+6) * yscale* scale, (y + 7)* yscale * scale);
	}
	if(textsides[3]){
		render((x+3) * xscale* scale, (x + 4) * xscale* scale, (y+4) * yscale* scale, (y + 6)* yscale * scale);
	}
	if(textsides[4]){
		render((x+3) * xscale* scale, (x + 4) * xscale* scale, (y+1) * yscale* scale, (y + 3)* yscale * scale);
	}
	if(textsides[5]){
		render((x+1) * xscale* scale, (x + 3) * xscale* scale, (y) * yscale* scale, (y + 1)* yscale * scale);
	}
	if(textsides[6]){
		render((x+1) * xscale* scale, (x + 3) * xscale* scale, (y+3) * yscale* scale, (y + 4)* yscale * scale);
	}
	
}

void render_loop()
{
	
	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(10);
	glLineWidth(2.5); 
	glColor3f(1.0, 0.0, 0.0);
	
	if(state == 1) {
		//game
		render(3 * xscale, 6 * xscale, player_y, player_y + 15 * yscale);

		text(30, 85, 2, '0' + score2);
		text(60, 85, 2, '0' + score1);

		render(ball_x, ball_x + 4 * xscale, ball_y, ball_y + 4 * yscale);
		render(94 * xscale, 97 * xscale, enemy_y, enemy_y + 15 * yscale);
	}else if(state == 0) {
		//start screen
		if(menuitem < 0) menuitem = 2;
		if(menuitem > 2) menuitem = 0;
		char texts[10];
		texts[0] = 'p';
		texts[1] = 'l';
		texts[2] = 'a';
		texts[3] = 'y';
		float scale = 1;
		if(menuitem == 0) scale = 1.4;
		for(int i= 0; i < 4; i++){
			text(35 + i * 5 * scale, 50, scale, texts[i]);
		}
		texts[0] = 'o';
		texts[1] = 'p';
		texts[2] = 't';
		texts[3] = 'i';
		texts[4] = 'o';
		texts[5] = 'n';
		texts[6] = 's';
		scale = 1;
		if(menuitem == 2) scale = 1.4;
		for(int i= 0; i < 7; i++){
			text(28 + i * 4.7 * scale, 35, scale, texts[i]);
		}
		texts[0] = 'q';
		texts[1] = 'u';
		texts[2] = 'i';
		texts[3] = 't';
		scale = 1;
		if(menuitem == 1) scale = 1.4;
		for(int i= 0; i < 4; i++){
			text(35 + i * 4.7 * scale, 20, scale, texts[i]);
		}
		
	}else if(state == 3) {
		//options
		if(menuitem < 0) menuitem = 2;
		if(menuitem > 2) menuitem = 0;
		char texts[10];
		texts[0] = 'r';
		texts[1] = 'e';
		texts[2] = 's';
		texts[3] = 'o';
		texts[4] = 'l';
		texts[5] = 'u';
		texts[6] = 't';
		texts[7] = 'i';
		texts[8] = 'o';
		texts[9] = 'n';
		float scale = 1;
		if(menuitem == 0) scale = 1.4;
		for(int i= 0; i < 10; i++){
			text(35 + i * 5 * scale, 50, scale, texts[i]);
		}
		texts[0] = 'c';
		texts[1] = 'o';
		texts[2] = 'l';
		texts[3] = 'o';
		texts[4] = 'r';
		texts[5] = 's';
		scale = 1;
		if(menuitem == 2) scale = 1.4;
		for(int i= 0; i < 7; i++){
			text(28 + i * 4.7 * scale, 35, scale, texts[i]);
		}
		texts[0] = 'b';
		texts[1] = 'a';
		texts[2] = 'c';
		texts[3] = 'k';
		scale = 1;
		if(menuitem == 1) scale = 1.4;
		for(int i= 0; i < 4; i++){
			text(35 + i * 4.7 * scale, 20, scale, texts[i]);
		}
		
	}else if(state == 4) {
		//resolution x
		if(menuitem < 0) menuitem = 2;
		if(menuitem > 2) menuitem = 0;
		char texts[10];
		texts[0] = 'x';
		float scale = 1;
		if(menuitem == 0) scale = 1.4;
		for(int i= 0; i < 1; i++){
			text(35 + i * 5 * scale, 50, scale, texts[i]);
		}
		scale = 1;
		if(menuitem == 2) scale = 1.4;
		for(int i= 0; i < 7; i++){
			text(28 + i * 4.7 * scale, 35, scale, texts[i]);
		}
		texts[0] = 'b';
		texts[1] = 'a';
		texts[2] = 'c';
		texts[3] = 'k';
		scale = 1;
		if(menuitem == 1) scale = 1.4;
		for(int i= 0; i < 4; i++){
			text(35 + i * 4.7 * scale, 20, scale, texts[i]);
		}
		
	}
	
	
	glFlush();

}

bool setresolution (int scrx, int scry) {
	screenx = scrx;
	screeny = scry;
	player_y = screeny / 2;
	enemy_y = screeny / 2;
	
	xscale = screenx / 100;
	yscale = screeny / 100;
	
	glfwTerminate();
	if( !glfwInit() )
	{
		return false;
	}
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );
	window = glfwCreateWindow( screenx, screeny, "Pong (OpenGL)", NULL, NULL );
	if (!window)
	{
		glfwTerminate();
		return false;
	}


	glfwMakeContextCurrent(window);
	//glfwSwapInterval( 1 );

	// set up view
	//glViewport( 0, 0, screenx, screeny );
	glMatrixMode( GL_NONE );
	//glLoadIdentity();

	glOrtho(0.0,screenx,0.0,screeny,0.0,1.0); // this creates a canvas you can do 2D drawing on
	
	glfwSetKeyCallback(window, key_callback);
	return true;
}


/* program entry */
int main(int argc, char *argv[])
{
	
	if(!setresolution(800, 600)){
		return 1;
	}
	
	ball_x = xscale * 50;
	ball_y = yscale * 50;
	ball_sx = 16;
	ball_sy = 3;


	
	start = glfwGetTime();
	double lowestdelta = 10;
	// Main loop
	while( !glfwWindowShouldClose(window))
	{
		
		
		render_loop();
		if(state == 1){
			ball();
			const int speed = 60;
			if(keys[265]) { 
				enemy = false;
				enemy_y += yscale * delta * speed;
			}
			if(keys[264]) { 
				enemy = false;
				enemy_y -= yscale * delta * speed;
			}
			if(keys[87]) {
				player_y += yscale * delta * speed;

			}
			if(keys[83]) { 

				player_y -= yscale * delta * speed;
			}
		}
		
		if(player_y < 0) player_y=0;
		if(player_y + yscale * 15 > screeny) player_y=screeny - yscale * 15;
		if(enemy_y < 0) enemy_y=0;
		if(enemy_y + yscale * 15 > screeny) enemy_y=screeny - yscale * 15;

		// Swap buffers
		//glfwSwapBuffers(window);
		glfwPollEvents();
		double now = glfwGetTime();
		delta = now - start;
		start = now;
		if(delta < lowestdelta) {
			lowestdelta = delta;
		}
		if(state == -1) {
			break;
		}
	}
	// Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	cout << 1/lowestdelta << endl;
	cout << lowestdelta*1000 << endl;
	// Exit program
	return 0;
}