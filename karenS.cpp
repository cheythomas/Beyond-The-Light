//Modified by: Karen Salinas
//Date: 6/21/2017

//On this program, I am in charge of the background.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"

#include <string>
#include "game.h"

using namespace std;

Timers() {
    physicsRate = 1.0 / 30.0;
    oobillion = 1.0 /1e9;
}

//This prototype allows background movement
void applyBackgroundMovement(void) {

    if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left]) {
        for (int i = 0; i < 20; i++) {
            if (gl.keys[XK_Left]) {
                gl.box[i][0] += 1.0 * (0.05 / gl.delay);
                if (gl.box[i][0] > gl.xres + 10.0)
                    gl.box[i][0] -= gl.xres + 10.0;
            } else {
                gl.box[i][0] -= 2.0 * (0.05 / gl.delay);
                if (gl.box[i][0] < -10.0)
                    gl.box[i][0] += gl.xres + 10.0;
            }
        }
    }
}

//rendering the Background
void renderBackground(void) {
    
    glBegin(GL_QUADS);
        glColor3f(0.2, 0.2, 0.2);
        glVertex2i(0,   200);
        glVertex2i(gl.xres,200);
        glVertex2i(0.4,0.4,0.4);
        glVertex2i(gl.xres,0);
        glVertex2i(0,   0);
    glEnd();
    //show boxes as background
    for (int i = 0; i < 20; i++) {
        glPushMatrix();
        glTranslated(gl.box[i][0], gl.box[i][1], gl.box[i][2]);
        glColor3f(0.2, 0.2, 0.2);
        glBegin(GL_QUADS);
                glVertex2i( 0, 0);
                glVertex2i( 0, 30);
                glVertex2i(20, 30);
                glVertex2i(20, 0);
        glEnd();
        glPopMatrix();
    }
}

