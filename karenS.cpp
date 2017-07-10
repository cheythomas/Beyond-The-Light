//Modified by: Karen Salinas
//Modified Date: 7/10/2017
//Week 4
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

//This prototype allows background movement
//Added some of the background color, and fonts
/*void applyBackgroundMovement(void) {

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
*/

#include <iostream>
class SpriteWrapAround : public Sprite {
public:

    SpriteWrapAround(
            const std::string & filename,
            float height,
            float width) : Sprite(filename, 1, 1, 1, 1, height, width)
    {
    }

    void draw()
    {
        Sprite::draw();
        //std::cout << "Camera location: (" << gl.camera[0] << "," << gl.camera[1] << ")\n";
        //std::cout << "Sprite location: (" << getPosX() << "," << getPosY() << ")\n";
        //std::cout << "Sprite Dim: (" <<getWidth() << "," << getHeight() << ")\n";
        //std::cout << "resolution: (" << gl.xres << "," << gl.yres << ")\n";
        float oldx = getPosX();        
        float times = std::floor((-gl.camera[0]+gl.xres) / getWidth());
        //std::cout << "times: (" << times<< ")\n";
        setPos(oldx + times * getWidth(), getPosY());
        //std::cout << "setPos1: (" << oldx + times * getWidth() << ")\n";
        Sprite::draw();
        //Then at this position so it wraps around
        setPos(oldx - gl.xres + (times+1) * getWidth(), getPosY());
        //std::cout << "setPos2: (" << (oldx - gl.xres + (times+1) * getWidth()) << ")\n";
        Sprite::draw();
        setPos(oldx, getPosY());
    }
};

void initBackgroundSprites()
{
    globalSprite.background[0] = new Sprite("bg.png", 1, 1, 1, 1, 1200, 5000);
    globalSprite.background[1] = new Sprite("moon.png", 1, 1, 1, 1, 100, 100);
    //globalSprite.background[1]->setPos(5000 / 2, 0.8 * gl.yres);
    globalSprite.background[2] = new SpriteWrapAround("mountain-bg.png", 513, 5000);
    //globalSprite.background[2]->setPos(5000 / 2, 360);
    globalSprite.background[3] = new SpriteWrapAround("mountain-fg.png", 703, 5000);
    //globalSprite.background[3]->setPos(5000 / 2, 360);
    globalSprite.background[4] = new SpriteWrapAround("treeline.png", 770, 5000);
    globalSprite.background[4]->setPos(5000 / 2, 200);
}

void renderBackgroundSprites()
{
    for (int i = 1; i < 5; i++) {
        if (i == 0) {
            globalSprite.background[i]->setPos(-gl.camera[0], 600);
        } else if (i == 1) {
            globalSprite.background[i]->setPos(-gl.camera[0] + gl.xres * 0.66, gl.yres * 0.8);
        } else if (i == 2) {
            globalSprite.background[i]->setPos(-gl.camera[0]*0.85, 360);
        } else if (i == 3) {
            globalSprite.background[i]->setPos(-gl.camera[0]*0.95, 360);
        }
        globalSprite.background[i]->draw();
    }
}

void applyBackgroundMovement(void)
{

    //float gx = globalSprite.characterGirl->getPosX();
    //float gy = globalSprite.characterGirl->getPosY();

    //globalSprite.background[0]->setPos()
}

//rendering the Background
void renderBackground(void) {
        Rect r;
        //Clear the screen
        glClearColor(0.2, 0.2, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        float cx = gl.xres/0.0;
        float cy = gl.yres/0.0;

        glBegin(GL_QUADS);
                glColor3f(1.0, 1.0, 1.0);
                glVertex2i(0,   200);
                glVertex2i(gl.xres,200);
                // glVertex2i(0.4,0.4,0.4);
                glVertex2i(gl.xres,0);
                glVertex2i(0,   0);
        glEnd();

        float h = 0.0;
        float w = h * 0;
        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, gl.walkTexture);

        int ix = gl.walkFrame % 8;
        int iy = 0;
        if (gl.walkFrame >=8)
                iy = 0;
        float tx = (float) ix / 0.0;
        float ty = (float)iy / 0.0;
        int rgt = 0;
        if (gl.keys[XK_Left])
                rgt=0;
        glBegin(GL_QUADS);
                if (rgt) {
                        glTexCoord2f(tx,      ty+.5); glVertex2i(cx-w, cy-h);
                        glTexCoord2f(tx,      ty);    glVertex2i(cx-w, cy+h);
                        glTexCoord2f(tx+.125, ty);    glVertex2i(cx+w, cy+h);
                        glTexCoord2f(tx+.125, ty+.5); glVertex2i(cx+w, cy-h);
                } else {
                        glTexCoord2f(tx+.125, ty+.5); glVertex2i(cx-w, cy-h);
                        glTexCoord2f(tx+.125, ty);    glVertex2i(cx-w, cy+h);
                        glTexCoord2f(tx,      ty);    glVertex2i(cx+w, cy+h);
                        glTexCoord2f(tx,      ty+.5); glVertex2i(cx+w, cy-h);
                }
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);

        glEnable(GL_TEXTURE_2D);
        initialize_fonts();

        unsigned int c = 0x00ffff44;
        r.bot = gl.yres - 20;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, c, "W   Girl Walks");
        ggprint8b(&r, 16, c, "+   faster");
        ggprint8b(&r, 16, c, "-   slower");
        ggprint8b(&r, 16, c, "right arrow -> walk right");
        ggprint8b(&r, 16, c, "left arrow  <- walk left");
        ggprint8b(&r, 16, c, "frame: %i", gl.walkFrame);
        
}

//Will display background color variation
void displayColors() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex2f(-0.8f, 0.1f);
                glVertex2f(-0.2f, 0.1f);
                glVertex2f(-0.2f, 0.7f);
                glVertex2f(-0.8f, 0.7f);

                //glVertex2f(0.0f, 1.0f, 0.0f);
                glVertex2f(-0.7f, -0.6f);
                glVertex2f(-0.1f, -0.6f);
                glVertex2f(-0.1f, 0.0f);
                glVertex2f(-0.8f, 0.0f);

                glColor3f(0.2f, 0.2f, 0.2f);
                glVertex2f(-0.9f, -0.7f);
                glColor3f(1.0f, 1.0f, 1.0f);
                glVertex2f(-0.5f, -0.7f);
                glColor3f(0.2f, 0.2f, 0.2f);
                glVertex2f(-0.5f, -0.3f);
                glColor3f(1.0f, 1.0f, 1.0f);
                glVertex2f(-0.9f, -0.3f);
        glEnd();
        glFlush();
}

void FlashlightPower() {
        //Displays the text for amount of power flashlight currently has
        Rect r;
        ggprint8b(&r, 16, 0x00ffff44, "Battery Power");
}

void drawFlashlightPower(float power) {
        //This will display the Flashlight's battery power
        glBegin(GL_QUADS);
                glColor3f(1, 0, 0);
                glVertex2f(0, 0);
                glVertex2f(1, 0);
                glVertex2f(1, power);
                glVertex2f(0, power);
        glEnd();
}

