// Modified by: Cheyenne Thomas
// Date: 6-20-17
// Purpose: Create flashlight/spotlight
//
// Week 4:
// draw battery and maybe get flashlight working
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
#include "game.h"
#define MAX_LIGHT 40

typedef float Vec[5];

struct Shape {
    float width, height;
    float radius;
    Vec center;
};

struct Light { //particle
    Shape s;   
    Vec velocity;
};

class Game {
    public:
    Shape box;
    Light light[MAX_LIGHT];
    int n;
    Game(){
        n = 0;
    }
};


class FlashLight {
	public:
		//Vec lightPosition;
		float lightPosition;
		float lightDiffuse;
		float lightSpecular;
		float lightAmbient;
		float lightDirection;
		FlashLight() {
			lightPosition = 100;
			lightDiffuse = 20;
			lightSpecular = 20;
			lightAmbient = 20;
			lightDirection = 20;
		
		}
};


void drawLight(void)
{

	float cx = 800/2.0;
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glVertex2i(cx-60, 550);
		glVertex2i(cx+50, 550);
		glVertex2i(cx+50, 530);
		glVertex2i(cx-60, 530);
	glEnd();

}



void LightCollison()
{
   
   
    /*
    Light *l;
  
    if (game->n <= 0)
        return;

    for (int i = 0; i<game->n; i++) {
        l = &game->light[i];
	l->velocity.y -= GRAVITY;
        l->s.center.x += l->velocity.x;
        l->s.center.y += l->velocity.y;

        //check for collision with shapes...
        Shape *s = &game->box;
	if (l->s.center.y < s->center.y + s->height && 
        l->s.center.x > s->center.x - s->width && 
        l->s.center.x < s->center.x + s->width ) {
        l->s.center.y = s->center.y + s->height;  
            l->velocity.y = -l->velocity.y;
            l->velocity.y *= 0.5;
        }
        //check for off-screen
        if (l->s.center.y < 0.0 || l->s.center.y > WINDOW_HEIGHT) {
            //std::cout << "off screen" << std::endl;
            game->light[i] = game->light[game->n-1];
            game->n--;
        }
    } 
    */

}
