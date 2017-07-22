//Modified by: Karen Salinas
//Modified Date: 6/24/2017
//Week 4
//On this program, I am in charge of the background.
//===============================================================
//Week 8
//Changed made: Added code for tiles, still need to edit
//
//===============================================================
//Week 9
//Changes made: Fixed parallax scrolling
//              Added the ground by rendering it
//              Tiles are currently being fixed
//===============================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cmath>
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
        //Calculate the sprite frame and size
        //and location
        float camX = -gl.camera[0];
        float cx = posX;
        float cy = posY;
        float h2 = height / 2.0;
        float repeatX = (camX - cx) / gl.xres;
        float xc0 = 0 + repeatX;
        float xc1 = (float) gl.xres / origWidth + repeatX;

        glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, glTexture);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255, 255, 255, 255);

        glBegin(GL_QUADS);
        glTexCoord2f(xc0, 1);
        glVertex2i(camX, cy - h2);
        glTexCoord2f(xc0, 0);
        glVertex2i(camX, cy + h2);
        glTexCoord2f(xc1, 0);
        glVertex2i(gl.xres + camX, cy + h2);
        glTexCoord2f(xc1, 1);
        glVertex2i(gl.xres + camX, cy - h2);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
};

void initBackgroundSprites()
{
    globalSprite.background[0] = new Sprite("bg.png", 1, 1, 1, 1, 1200, 5000);
    globalSprite.background[1] = new Sprite("moon.png", 1, 1, 1, 1, 100, 100);
    globalSprite.background[1]->setPos(5000 / 2, 0.8 * gl.yres);
    globalSprite.background[2] = new SpriteWrapAround("mountain-fg.png", 513, 5000);
    globalSprite.background[2]->setPos(5000 / 2, 360);
    globalSprite.background[3] = new SpriteWrapAround("mountain-bg.png", 703, 5000);
    globalSprite.background[3]->setPos(5000 / 2, 360);
    globalSprite.background[4] = new SpriteWrapAround("treeline.png", 770, 5000);
    globalSprite.background[4]->setPos(5000 / 2, 200);

}

void renderBackgroundSprites()
{
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            globalSprite.background[i]->setPos(-gl.camera[0], 600);
        } else if (i == 1) {
            globalSprite.background[i]->setPos(-gl.camera[0] + gl.xres * 0.66, gl.yres * 0.8);
        } else if (i == 2) {
            globalSprite.background[i]->setPos(-gl.camera[0]*0.1, 360);
        } else if (i == 3) {
            globalSprite.background[i]->setPos(-gl.camera[0]*0.50, 360);
        } else {
            globalSprite.background[i]->setPos(gl.camera[0], 250);
        }
        globalSprite.background[i]->draw();
    }

    glColor3ub(24, 24, 24);
    glBegin(GL_QUADS);
    glVertex2i(-gl.camera[0], 0);
    glVertex2i(-gl.camera[0], 100);
    glVertex2i(gl.xres + -gl.camera[0], 100);
    glVertex2i(gl.xres + -gl.camera[0], 0);
    glEnd();
}

void applyBackgroundMovement(void)
{

    //float gx = globalSprite.characterGirl->getPosX();
    //float gy = globalSprite.characterGirl->getPosY();

    //globalSprite.background[0]->setPos()
}

void Level::renderBackground(void)
{
    Flt dd = ftsz[0];
    Flt offy = tile_base;
    int ncols_to_render = gl.xres / tilesize[0] + 2;
    int col = (int) (gl.camera[0] / dd);
    col = col % ncols;

    Flt cdd = gl.camera[0] / dd;

    Flt flo = floor(cdd);

    Flt dec = (cdd - flo);

    Flt offx = -dec * dd;

    for (int j = 0; j < ncols_to_render; j++) {
        int row = nrows - 1;
        for (int i = 0; i < nrows; i++) {
            if (arr[row][col] == 'w') {
                glColor3f(0.8, 0.8, 0.6);
                glPushMatrix();
                glBindTexture(GL_TEXTURE_2D, 0);
                Vec tr = {(Flt) j * dd + offx, (Flt) i * ftsz[1] + offy, 0};
                glTranslated(tr[0], tr[1], tr[2]);
                int tx = tilesize[0];
                int ty = tilesize[1];
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 1.0);
                glVertex2i(0, 0);
                glTexCoord2f(0.0, 1.0);
                glVertex2i(0, ty);
                glTexCoord2f(0.0, 1.0);
                glVertex2i(tx, ty);
                glTexCoord2f(0.0, 1.0);
                glVertex2i(tx, 0);
                glEnd();
                glPopMatrix();
                glBindTexture(GL_TEXTURE_2D, 0);

            }

            if (arr[row][col] == 'b') {
                glColor3f(0.9, 0.2, 0.2);
                glPushMatrix();
                Vec tr = {(Flt) j * dd + offx, (Flt) i * ftsz[i] + offy, 0};
                glTranslated(tr[0], tr[1], tr[2]);
                int tx = tilesize[0];
                int ty = tilesize[1];
                glBegin(GL_QUADS);
                glVertex2i(0, 0);
                glVertex2i(0, ty);
                glVertex2i(tx, ty);
                glVertex2i(tx, 0);
                glEnd();
                glPopMatrix();
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            --row;

        }
        ++col;
        col = col % ncols;
    }
}






//rendering the Background
/*void renderBackground(void) {
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
 */
