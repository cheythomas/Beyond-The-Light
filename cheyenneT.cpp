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
void Battery::battbarAppear()
{
    // a battery on the ground
    float w, h, z, x, y;
    w = 34; // width size
    h = 12; // length size
    z = 0.0;
    x = 740; // x-axis
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
    r.bot = 70; // y-axis
    r.left = 700; // x-axis
    r.center = 0;
    unsigned int c = 0x00ffff44;
    ggprint8b(&r, 16, c, "Hit A to collect");
}

void Battery::grabBattery()
{
    Rect r;
   	r.bot = 90; // y-axis
   	r.left = 700; // x-axis
   	r.center = 0;
    unsigned int c = 0x00ffff44;
	if (bcount == 4) {
    	ggprint8b(&r, 16, c, "Already full");
	}
	
	if (bcount < 4) {
    	ggprint8b(&r, 16, c, "nice!");
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
		bcount++;
    }
}

void Battery::drawFlashlight()
{
    float cx = 800 / 8.0;
    float w, h, z, x, y;
	z = 0;
    
	glColor3f(0, 0, 0);
    // dont forget to fix magic numbers
    glPushMatrix();
    glBegin(GL_QUADS);
    	glVertex2i(cx - 60, 178);
    	glVertex2i(cx + 50, 178);
    	glVertex2i(cx + 50, 158);
    	glVertex2i(cx - 60, 158);
    glEnd();
    glPopMatrix();

	/*light coming out*/	
    w = 85; // width size
    h = 80; // length size
    z = 0.0;
    x = 260; // x-axis
    y = 162; // y-axis
    //glColor3f(1, 1, 1); 
    glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 0.4);
	glTranslatef(x, y, z);
    glRotatef(313,0.0,0,1);
    glBegin(GL_TRIANGLES);
       	glVertex2i(-w, -h);
        glVertex2i(-w,  h);
        //glVertex2i( w,  h);
        glVertex2i( w, -h);
    glEnd();
	glDisable(GL_BLEND);
    glPopMatrix();

}

// not using till light works
void LightCollison()
{
    //   
}


void gameOver()
{

}



/*
// Light values and coordinates
GLfloat  lightPos[] = { 0.0f, 0.0f, 75.0f, 1.0f };
GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f};
GLfloat  spotDir[] = { 0.0f, 0.0f, -1.0f };

// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
//void Battery::drawFlashlight()
void SetupRC()
{
        glEnable(GL_DEPTH_TEST);   // Hidden surface removal
        glFrontFace(GL_CCW);       // Counterclockwise polygons face out
        glEnable(GL_CULL_FACE);    // Do not try to display the back sides

        // Enable lighting
        glEnable(GL_LIGHTING);

        // Set up and enable light 0
        // Supply a slight ambient light so the objects can be seen
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

        // The light is composed of just diffuse and specular components
        glLightfv(GL_LIGHT0,GL_DIFFUSE,ambientLight);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
        glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

        // Specific spot effects
        // Cut off angle is 60 degrees
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,60.0f);

        // Fairly shiny spot
        glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,100.0f);

        // Enable this light in particular
        glEnable(GL_LIGHT0);

        // Enable color tracking
        glEnable(GL_COLOR_MATERIAL);

        // Set Material properties to follow glColor values
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        // All materials hereafter have full specular reflectivity
        // with a high shine
        glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
        glMateriali(GL_FRONT, GL_SHININESS,128);

        // Black background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

	// Specific spot effects
	// Cut off angle is 60 degrees
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,60.0f);

	// Fairly shiny spot
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,100.0f);
}
*/
