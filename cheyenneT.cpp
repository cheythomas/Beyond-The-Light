// Author by: Cheyenne Thomas
// Date: 6-23-17
// Purpose: Create flashlight, battery, endgame, etc
//
// Week 4:
// draw the battery and its battery bars, 4. 
// write a function that shows the flashlight running out of bars
// draw flashlight
//
// Week 5: figure out how to create spotlight effect
//
#include "game.h"

// not being called yet, need to figure out keys
void Battery::grabObject(int k)
{
    // a battery on the ground
    float w, h, z, x, y;
    w = 34; // width size
    h = 12; // length size
    z = 0.0;
    x = 750; // x-axis
    y = 50; // y-axis
    glColor3ub(40, 230, 90); 
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
       	glVertex2i(-w, -h);
        glVertex2i(-w,  h);
        glVertex2i( w,  h);
        glVertex2i( w, -h);
    glEnd();
    glPopMatrix();
    
    Rect r;
    r.bot = 580;
    r.left = 10;
    r.center = 0;
    //bool bat = false;
    unsigned int c = 0x00ffff44;
    ggprint8b(&r, 16, c, "Hit a to collect");

    switch (k) {
    case 'a':
        break;
    }
}

// not being called yet, still needs work
void Battery::deleteBattery()
{
    if (points > 500) {
        arr[0] = 0; // fix later
    }

    if (points > 1000) {
        // delete [1]
    }
    if (points > 1500) {
        // delete [2]
    }
    if (points > 2000) {
        // delete [3]
    }
}

void Battery::drawBattery(void)
{
    float w, h, z, x, y;
    w = 40; // width size
    h = 70; // length size
    z = 0.0;
    x = 750; // x-axis
    y = 490; // y-axis

    /**draw battery**/
    glColor3ub(0, 0, 0);
    //glColor3f(20,20,121);
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    	glVertex2i(-w, -h);
    	glVertex2i(-w,  h);
    	glVertex2i( w,  h);
    	glVertex2i( w, -h);
    glEnd();
    glPopMatrix();

    /**battery border**/
    glColor3ub(0, 100, 0);
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_LINES);
    	glVertex2i(-w, -h);
    	glVertex2i(-w,  h);
    	glVertex2i( w,  h);
    	glVertex2i( w, -h);
    glEnd();
    glPopMatrix();

    /**top of battery**/
    w = 20; // width size
    h = 15; // length size
    z = 0.0;
    x = 749; // x-axis
    y = 575; // y-axis
    glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    	glVertex2i(-w, -h);
    	glVertex2i(-w,  h);
    	glVertex2i( w,  h);
    	glVertex2i( w, -h);
    glEnd();
    glPopMatrix();

    /**battery border**/
    glColor3ub(0, 100, 0);
    //glColor3f(20,20,121);
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_LINES);
    	glVertex2i(-w, -h);
    	glVertex2i(-w,  h);
    	glVertex2i( w,  h);
    	glVertex2i( w, -h);
    glEnd();
    glPopMatrix();

    // battery bars
    w = 34; // width size
    h = 12; // length size
    z = 0.0;
    x = 750; // x-axis
    y = 540; // y-axis
    for (int i = 0; i < 4; i++) {
        glColor3ub(40, 230, 90); // make brighter
        glPushMatrix();
        glTranslatef(x, y, z);
        glBegin(GL_QUADS);
        	glVertex2i(-w, -h);
        	glVertex2i(-w,  h);
        	glVertex2i( w,  h);
        	glVertex2i( w, -h);
        glEnd();
        glPopMatrix();
        y -= 35; // y-axis
        arr[i] = y;
    }
}

void Battery::drawFlashlight()
{
    float cx = 800 / 2.0;
    glColor3f(1, 1, 1);
    // dont forget to fix magic numbers
    glBegin(GL_QUADS);
    	glVertex2i(cx - 60, 550);
    	glVertex2i(cx + 50, 550);
    	glVertex2i(cx + 50, 530);
    	glVertex2i(cx - 60, 530);
    glEnd();
    glPopMatrix();
}

// not using till light works
void LightCollison()
{
    //   
}

