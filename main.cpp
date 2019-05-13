//#define GLFW_INCLUDE_VULKAN
//#include <iostream>
 //#define __LCC__
 //#define GLFWAPI __declspec(dllimport)
#include "include/GL/glew.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
//#include <iostream>
#include <string>
//#include <fstream>


using namespace std;

int screenx, screeny;

double player_y, enemy_y;
double oldplayer_y, oldenemy_y;
double player_sy, enemy_sy;
bool player = false, enemy =false;
double ball_x, ball_y;
double ball_sx, ball_sy;
int xscale, yscale;
int score1, score2;
char lastletter = 0;
int state =  0 , laststate = 1;
int menuitem = 0;
double ai_speed = 90;
double ai_acc = 3;
bool vsync = false;
double speed = 90;
double ball_speed = 1.05;
int small = 0;
bool fullscreen = false;
double scaling = 15;
double startup_ani = -18;

double* temp;
string option_name = "";

bool keys[500];
GLFWwindow* window;

double delta;
double start;

double timeout[10];

double pointer = menuitem;

bool setresolution (int scrx, int scry);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ball ();

void render (float x1, float x2, float y1, float y2);
void text (float x, float y, float scale, char letter);
void render_loop();
void menu (float x, float y, float scale, string menuitems[8], int amount);

void ai (double& y, double& sy);



/* program entry */
int main(int argc, char *argv[])
{



    screenx = 800;
    screeny = 600;




    for(int i = 1; i < argc; i++){
        if(i >= argc) {break;}
        char tempor = *argv[i];
        if(tempor == 'x') {
            string str = argv[i+1];
            screenx = stoi( str );
        }
        if(tempor == 'y') {
            string str = argv[i+1];
            screeny = stoi (str);
        }
        string strin = argv[i];
        if(strin == "fullscreen") {
            fullscreen = true;
        }
        if(strin == "vsync") {
            vsync = true;
        }
        if(strin == "ballspeed") {
            string str = argv[i+1];
            ball_speed = stoi (str);
        }
        if(strin == "aiacc") {
            string str = argv[i+1];
            ai_acc = stoi (str);
        }
        if(strin == "aispeed") {
            string str = argv[i+1];
            ai_speed = stoi (str);
        }
        if(strin == "speed") {
            string str = argv[i+1];
            speed = stoi (str);
        }
    }

    if(!setresolution(screenx, screeny)){
		return 1;
	}

	ball_x = xscale * 50;
	ball_y = yscale * 50;
	ball_sx = 25;
	ball_sy = 0;



	start = glfwGetTime();
	int cycle = 0;
	// Main loop
	while( !glfwWindowShouldClose(window))
	{
	    int counting;

		render_loop();


		const float* axes = glfwGetJoystickAxes(0, &counting);
        const float* axes2 = glfwGetJoystickAxes(1, &counting);
        const unsigned char* butts = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &counting);

		float deadzone = 0.3;
		if(state == 1){
			ball();
			oldenemy_y = enemy_y;
            oldplayer_y = player_y;

            /*
			if(keys[265] || axes[1] > deadzone) {
				enemy = true;
				enemy_y += yscale * delta * speed;
			}

			if(keys[264] || axes[1] < -deadzone) {
				enemy = true;
				enemy_y -= yscale * delta * speed;
			}

			if(keys[87] || axes2[1] > deadzone) {
                player = true;
				player_y += yscale * delta * speed;

			}
			if(keys[83] || axes2[1] < -deadzone) {
                player = true;
				player_y -= yscale * delta * speed;
			}
            */

            if(keys[265] ) {
				enemy = true;
				enemy_y += yscale * delta * speed;
			}

			if(keys[264]) {
				enemy = true;
				enemy_y -= yscale * delta * speed;
			}

			if(keys[87] ) {
                player = true;
				player_y += yscale * delta * speed;

			}
			if(keys[83] ) {
                player = true;
				player_y -= yscale * delta * speed;
			}


            if(!player && ball_sx < 0) ai( player_y, player_sy);
            if(!enemy && ball_sx > 0) ai( enemy_y, enemy_sy);


            if(player_y < 0) player_y=0;
            if(player_y + yscale * 15 > screeny) player_y=screeny - yscale * 15;
            if(enemy_y < 0) enemy_y=0;
            if(enemy_y + yscale * 15 > screeny) enemy_y=screeny - yscale * 15;

		}

		const float amountoftimeout = 0.4;

		if(state != 1) {
            double timeouts[10];
            if((axes[1] > deadzone || axes2[1] > deadzone) && timeouts[0] < 0.1) {
                timeouts[0] = amountoftimeout;
                key_callback(window, 87, 0, 1, 0);
            }
            if((axes[1] < -deadzone || axes2[1] < -deadzone) && timeouts[1] < 0.1) {
                timeouts[1] = amountoftimeout;
                key_callback(window, 83, 0, 1, 0);
            }

            if((butts[0] == 1 || butts[7] == 1) && timeouts[2] < 0.1) {
                timeouts[2] = amountoftimeout;
                key_callback(window, 32, 0, 1, 0);
            }
		}





		// Swap buffers
        laststate = state;
		glFlush();
        glfwSwapBuffers(window);
		glfwPollEvents();

        if(state == 1 && (keys[256]  )){ //|| butts[7] == 1 -- removed because it caused crashes
            player = false;
            enemy = false;
            state = 2;
            menuitem = 0;
        }

		if(cycle == 20) {
            double now = glfwGetTime();
            delta = (now - start) / 20;
            start = now;
            for(int i = 0; i < 10; i++) {
            if(timeout[i] > 0 ) timeout[i] -= delta * 20;
            }
            if(state == -1) {
                break;
            }
            cycle = 0;
		}
		cycle++;

	}
	// Terminate GLFW
	fullscreen = false;
	setresolution(100, 100);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void menu (float x, float y, float scale, string menuitems[10], int amount)
{
    if(state != laststate) {
        scaling = 5;
    }else if(scaling < 14.98){
        scaling += (15 - scaling) * delta * 10;
    }
    if(amount > 6 && pointer > 6) {
        y -= -2 - (pointer - 6) * scaling + 2;
    }


    for(int i = 0; i < amount; i++)
    {
        if(menuitems[i] == "") {break;}
        if(menuitem == i) scale *= 1.3;
        for(int j =0; j < (unsigned long long)menuitems[i].length(); j++)
        {
            text(x + (5 * j) * scale, y - (scaling * i), scale, menuitems[i][j]);
        }
        if(menuitem == i) scale /= 1.3;
        if(menuitems[i] == "vsync" && vsync) {
            render(80 * xscale, 84 * xscale, (y - (i * scaling)) * yscale, (y + 4 - (i * scaling)) * yscale);
        }
        if(menuitems[i] == "fullscreen" && fullscreen) {
            render(98 * xscale, 94 * xscale, (y - (i * scaling)) * yscale, (y + 4 - (i * scaling)) * yscale);
        }
    }




    pointer += (menuitem - pointer) * 20 * delta;
    render((x - 5) * xscale, (x - 8) *xscale, (y - pointer* scaling + 2) * yscale, (y - pointer* scaling + 5)  * yscale);

    if(menuitem < 0) menuitem = amount-1;
    else if(menuitem > amount-1) menuitem = 0;
}

bool setresolution (int scrx, int scry) {
	screenx = scrx;
	screeny = scry;
	player_y = screeny / 2;
	enemy_y = screeny / 2;

	if(ball_x == 50 * xscale && ball_y == 50 * yscale) {ball_x = screenx /2; ball_y = screeny/2;}

	xscale = screenx / 100;
	yscale = screeny / 100;

	glfwTerminate();
	if( !glfwInit() )
	{
		return false;
	}
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE );
	glfwWindowHint(GLFW_STEREO, GL_FALSE);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 0);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_RELEASE_BEHAVIOR_NONE);

	if(!fullscreen) {
        window = glfwCreateWindow( screenx, screeny, "Pong (OpenGL)", NULL, NULL );
	}else {
        window = glfwCreateWindow( screenx, screeny, "Pong (OpenGL)", glfwGetPrimaryMonitor(), nullptr);
	}
	if (!window)
	{
		glfwTerminate();
		return false;
	}


	glfwMakeContextCurrent(window);

	printf("this is pretty close to the end now");


	// set up view
	glViewport( 0, 0, screenx, screeny );
	glMatrixMode( GL_NONE );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
	glLoadIdentity();

	glOrtho(0.0,screenx,0.0,screeny,0.0,1.0); // this creates a canvas you can do 2D drawing on

	glfwSetKeyCallback(window, key_callback);

	if(vsync) glfwSwapInterval( 1 ); else glfwSwapInterval( 0 );
	return true;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	char lastletter = 0;
	lastletter = key;
	//cout << key << ' ' << lastletter << endl;

	if(action == GLFW_PRESS) {keys[key] = true;}
	if(action == GLFW_RELEASE) {keys[key] = false; lastletter = 0; return;}
	if(state != 1 && action == GLFW_PRESS && (key == 265 || key == 87)) menuitem--;
	else if(state !=1 && action == GLFW_PRESS && (key == 264 || key == 83)) menuitem++;
	else if(state ==0 && action == GLFW_PRESS && (key == 257 || key == 32)) {
		switch(menuitem){
			case 0:
				//play
				state = 1;
			break;
			case 1:
				//options
				state = 3;
				menuitem = 7;
			break;
			case 2:
				//quit
				state = -1;
			break;

		}
	}else if(state ==3 && action == GLFW_PRESS && (key == 257 || key == 32)) {
		switch(menuitem){
			case 0:
				//change screen resolution
				state = 4;
				screenx = 0;
			break;
			case 1:
				//ai_acc
				state = 6;
				temp = &ai_acc;
				*temp = 0;
				small = 1;
				option_name = "ai acc";
			break;
			case 2:
				//ai_speed
				small = 1;
				state = 6;
				temp = &ai_speed;
				*temp = 0;
				option_name = "ai speed";
			break;
			case 3:
			    //vsync
				if(vsync)vsync = false;
				else vsync = true;
                setresolution(screenx, screeny);
				menuitem = 6;
			break;
			case 4:
			    small = 1;
				//speed
				state = 6;
				temp = &speed;
				*temp = 0;
				option_name = "speed";
			break;
			case 5:
			    small = 100;
				//ballspeed
				state = 6;
				temp = &ball_speed;
				*temp = 0;
				option_name = "ball speed";
			break;
			case 6:
			    //fullscreen
				if(fullscreen)fullscreen = false;
				else fullscreen = true;
				setresolution(screenx, screeny);
				menuitem = 6;
			break;
			case 7:
				//back
				state = 0;
				if(ball_y != 50 * yscale && ball_x != 50 * xscale) state = 2;
			break;

		}
	}else if(state ==4 && action == GLFW_PRESS && key > 47 && key < 58) {
		//change screen resolution x
		screenx = (lastletter - '0') + (screenx * 10);
	}else if(state == 4 && action ==GLFW_PRESS && (key == 257 || key == 32)) {
		// ^^ if pressed enter
		state = 5;
		screeny = 0;
	}else if(state ==5 && action == GLFW_PRESS && key > 47 && key < 58) {
		//change screen resolution y
		screeny = (lastletter - '0') + (screeny * 10);
	}else if(state == 5 && action ==GLFW_PRESS && (key == 257 || key == 32)) {
		//^^ if pressed enter
		setresolution(screenx, screeny);
		state = 3;
	}else if(state ==6 && action == GLFW_PRESS && key > 47 && key < 58) {
		//change screen resolution y
		double tempor = *temp;
		double tempory = ((lastletter - '0') + (tempor * 10));
		*temp = tempory;
		//
	}else if(state == 6 && action ==GLFW_PRESS && (key == 257 || key == 32)) {
		//^^ if pressed enter
		*temp = *temp / small;
		state = 3;
	}else if(state ==2 && action == GLFW_PRESS && (key == 257 || key == 32)) {
		switch(menuitem){
			case 0:
				//continue
				state = 1;
			break;
			case 1:
				//options
				state = 3;
			break;
			case 2:
				//back
				state = 0;
				enemy_y = 50 * yscale;
				player_y = 50 * yscale;
				ball_x = 50 * xscale;
				ball_y = 50 * yscale;
				ball_sx = 25;
				ball_sy = 0;

			break;

		}
	}
	return;
}

void ball () {


	if(ball_y > screeny - xscale * 4) {ball_sy = -ball_sy; ball_y = screeny - xscale * 4;}
	else if(ball_y < 0) {ball_sy = -ball_sy; ball_y = 0;}

	if( ball_x + (ball_sx * xscale * delta) > xscale * 90 && ball_x < xscale * 97 &&
        (ball_y - enemy_y > 0 - 4 * yscale || ball_y - oldenemy_y > 0 - 4 * yscale)&&
        (ball_y - (enemy_y + 15 * yscale) < 0 || ball_y - (oldenemy_y + 15 * yscale) < 0))
    {
		ball_x = xscale*90;
		ball_sx = -ball_sx * ball_speed;
		ball_sy = (ball_y - (enemy_y + (yscale * 7.5)));
	}else if( ball_x + ball_sx * xscale * delta < xscale * 6 && ball_x > xscale * 3 &&
        (ball_y - oldplayer_y > 0 - 4 * yscale || ball_y - player_y > 0 - 4 * yscale)&&
        (ball_y - (oldplayer_y + 15 * yscale) < 0 || (ball_y - (player_y + 15 * yscale) < 0)))
    {
		ball_x = xscale*6;
		ball_sx = -ball_sx * ball_speed;
		ball_sy = (ball_y - (player_y + (yscale * 7.5)));
	}else {
        ball_x += ball_sx * xscale * delta;
        ball_y += ball_sy * 5 * ball_speed * delta;
	}

    glColor4f(1.0, 0.0, 0.0, 0.3);
    const float behind = 0.3;
    /*
    if(ball_sx < -50) {
        render(ball_x + (xscale * 4), ball_x + (xscale * 4 + ball_sx * behind), ball_y, ball_y + 4*xscale);
    }
    if(ball_sx > 50) {
        render(ball_x, ball_x - ball_sx * behind, ball_y, ball_y + 4*yscale);
    }
    */



	if(ball_x + xscale * 4 < 0) {
		score1++;
		ball_x = screenx / 2;
		ball_y = screeny / 2;
		ball_sx = 25 * ball_speed;
		ball_sy = 0;
		player_y = screeny /2;
		enemy_y = screeny /2;
	}else if(ball_x > screenx) {
		score2++;
		ball_x = screenx / 2;
		ball_y = screeny / 2;
		ball_sx = -25 * ball_speed;
		ball_sy = 0;
		player_y = screeny /2;
		enemy_y = screeny /2;
	}
	//cout << ball_sx << endl;
}


void render (float x1, float x2, float y1, float y2) {
	glBegin(GL_QUADS);
		glVertex2f( x1 , y1);
		glVertex2f( x2 , y1);
		glVertex2f( x2 , y2);
		glVertex2f( x1 , y2);
		glVertex2f( x1 , y1);
		glVertex2f( x2 , y2);
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
			textsides[1] = true;
			textsides[2] = true;
			textsides[3] = true;
			textsides[4] = true;
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

void ai (double& y, double& sy)
{

    if(ball_y < y - 5 * yscale){
        sy -= ai_speed * delta * ai_acc;
    }else if(ball_y > y + 5 * yscale){
        sy += ai_speed * delta * ai_acc;
    }else {
        sy *= 1 - (delta * 1.4);
    }

    if(sy > ai_speed) sy = ai_speed;
    if(sy < -ai_speed) sy = -ai_speed;

    y += sy * yscale * delta;
}


void render_loop()
{

	glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(10);
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);

	if(state == 1) {
		//game
		render(3 * xscale, 6 * xscale, player_y, player_y + 15 * yscale);
        int temp_score2 = score2 / 10;
        int temp_score1 = score1 / 10;
		text(30, 85, 2, '0' + score2 - temp_score2 * 10);
		if(temp_score2 != 0)    text(20, 85, 2, '0' + temp_score2);
		text(70, 85, 2, '0' + score1 - temp_score1 * 10);
		if(temp_score1 != 0)    text(60, 85, 2, '0' + temp_score1);

		render(ball_x, ball_x + 4 * xscale, ball_y, ball_y + 4 * yscale);
		render(94 * xscale, 97 * xscale, enemy_y, enemy_y + 15 * yscale);

	}else if(state == 0) {
		//start screen

        startup_ani += (8 - startup_ani) * delta * (1/0.7);

        if(startup_ani > 5) {
            startup_ani = -5;
        }
        char letters[4];
        letters[0] = 'p';
        letters[1] = 'o';
        letters[2] = 'n';
        letters[3] = 'g';
        float scale = 0;
        if( 0 < startup_ani) {
                scale = 0 - startup_ani;
            }else {
                scale = startup_ani;
        }
        scale /= 12;
        for(int i = 0; i < 4; i++) {


            text(25 + (13 * i), 70 + scale * 4, scale + 3, letters[i]);
        }

        string menus [5];
        menus[0] = "play";
        menus[1] = "options";
        menus[2] = "quit";
        menu(35, 35, 1, menus, 3);

	}else if(state == 3) {
		//options
		string menus [9];
        menus[0] = "screen";
        menus[1] = "ai acc";
        menus[2] = "ai speed";
        menus[3] = "vsync";
        menus[4] = "speed";
        menus[5] = "ball speed";
        menus[6] = "fullscreen";
        menus[7] = "back";
        menu(20, 90, 1, menus, 8);



	}else if(state == 4) {
		//resolution x
		if(menuitem < 0) menuitem = 2;
		if(menuitem > 2) menuitem = 0;
		float scale = 1;
		if(menuitem == 0) scale = 1.4;
        text(35 + 5 * scale, 50, scale, 'x');
		scale = 1;
		int i = screenx;
		int j = 0;
		while(i != 0) {
            j ++;
            i /= 10;
		}
		string number = to_string(screenx);
		if(menuitem == 2) scale = 1.4;
		for(i= 0; i < j; i++){
            if(i != 0 || number[i] != '0')
			text(28 + i * 4.7 * scale, 35, scale, number[i]);
		}
	}else if(state == 5) {
		//resolution y
		float scale = 1;
		if(menuitem == 0) scale = 1.4;
        text(35 + 5 * scale, 50, scale, 'y');
		scale = 1;
		int i = screeny;
		int j = 0;
		while(i != 0) {
            j ++;
            i /= 10;
		}
		string number = to_string(screeny);
		if(menuitem == 2) scale = 1.4;
		for(i= 0; i < j; i++){
            if(i != 0 || number[i] != '0')
			text(28 + i * 4.7 * scale, 35, scale, number[i]);
		}
	}else if(state == 6) {
		//settings
		float scale = 1;
		if(menuitem == 0) scale = 1.4;
        string name[8];
        name[0] = option_name;
        menu(28, 60, 1, name, 1);
		scale = 1;
		int i = *temp;
		int j = 0;
		while(i != 0) {
            j ++;
            i /= 10;
		}
		int tempory = *temp;
		string number = to_string(tempory);
        if(menuitem == 2) scale = 1.4;
		for(i= 0; i < j; i++){
            if(i != 0 || number[i] != '0')
			text(28 + i * 4.7 * scale, 35, scale, number[i]);
		}
    }else if(state == 2) {
		//pause screen
		string menus [5];
        menus[0] = "continue";
        menus[1] = "options";
        menus[2] = "back";
        menu(35, 55, 1, menus, 3);

	}




}
