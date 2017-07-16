// Author by: Cheyenne Thomas
// Date: 6-23-17
// Purpose: Create flashlight, battery, endgame, etc
//
// Week 4:
// draw the battery and its battery bars, 4. 
// write a function that shows the flashlight running out of bars
// draw flashlight
//
// Week 5: fixed functions
// 
// Week 6: added sprite, called functions, get game to gameover
//
#include "game.h"

// not being called yet
void Battery::chargeUp()
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
	ggprint8b(&r, 16, c, "");
}

// needs to be fixed
void Battery::grabCharge()
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

// old version
// the more key press to kill ghost, battery drains
// function not in use, will be deleted
void Battery::healthBar()
{	
	if (gl.keyCount > 10) {
		bcount = bcount - 1;
	}

	else if (gl.keyCount > 20) {
		// delete 2
		bcount = bcount - 1;
	}
	else if (gl.keyCount > 30) {
		// delete 3
		bcount = bcount - 1;
	}
	else if (gl.keyCount > 40) {
		// delete 4 
		bcount = bcount - 1;
	}
}

// new version
class energyBar : public Sprite {
	
	public:
	// Note: height = makes it skinnier or thicker
	energyBar (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	}
};

void initLifeBarSprite()
{
	globalSprite.life[0] = new Sprite("10.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[1] = new Sprite("9.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[2] = new Sprite("8.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[3] = new Sprite("7.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[4] = new Sprite("6.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[5] = new Sprite("5.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[6] = new Sprite("4.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[7] = new Sprite("3.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[8] = new Sprite("2.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[9] = new Sprite("1.png", 1, 1, 1, 1, 38, 378);
	globalSprite.life[10] = new Sprite("0.png", 1, 1, 1, 1, 38, 378);
}

void renderLifeBarSprite()
{        
	// full to empty
	if (gl.keyCount > 10) 
		globalSprite.life[0]->draw();
	
	if (gl.keyCount > 20) 
		globalSprite.life[1]->draw();
	
	if (gl.keyCount > 30) 
		globalSprite.life[2]->draw();
	
	if (gl.keyCount > 40) 
		globalSprite.life[3]->draw();
	
	if (gl.keyCount > 50) 
		globalSprite.life[4]->draw();
	
	if (gl.keyCount > 60) 
		globalSprite.life[5]->draw();
	
	if (gl.keyCount > 70) 
		globalSprite.life[6]->draw();
	
	if (gl.keyCount > 80) 
		globalSprite.life[7]->draw();
	
	if (gl.keyCount > 90) 
		globalSprite.life[8]->draw();
	
	if (gl.keyCount > 100) 
		globalSprite.life[9]->draw();
	
	if (gl.keyCount > 110) 
		globalSprite.life[10]->draw();
}


// old version
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
	// fix magic numbers
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

class GameOver : public Sprite {
	
	public:
	// (file, frameCount, row, col, delay, h, w)
	GameOver (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	  
		float cx = gl.xres/2.0;
		float cy = gl.yres/2.0;
		float w = 200*.5;
		float h = 200.0;

		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255, 255, 255, 255);
		glBegin(GL_QUADS);
			glVertex2i(cx - w, cy - h);
			glVertex2i(cx - w, cy + w);
			glVertex2i(cx + w, cy + h);
			glVertex2i(cx + w, cy - h);
		glEnd();
		glPopMatrix();
	}
};


void initGameOverSprite()
{
	globalSprite.gameover = new Sprite("go.png", 1, 1, 1, 1, 127, 430);
	globalSprite.gameover->setPos(gl.xres/2, gl.yres/2); 
}

void renderGameOverSprite()
{     
	if (gl.keyCount > 110) {
		globalSprite.gameover->draw();
	}
}

// old version/background
void Battery::gameOver()
{
	Rect r;
	float h, w;
	//if (bcount == 0 /* || collisionWithGhost */) {
	// testing to get screen to popup
	if (gl.keyCount > 110) {
		h = 600.0;
		w = 800.0;
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0, 1.0, 1.0, 0.7);
		glTranslated(gl.xres/2, gl.yres/2, 0);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w,  h);
			glVertex2i( w,  h);
			glVertex2i( w, -h);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		// unsigned int c = 0x00ffff44;
		r.bot = 100;  // y axis
		r.left = gl.xres/2;
		r.center = 1;
		ggprint8b(&r, 16, 0, "ESC to quit");
	}
}

class Lightning : public Sprite {
	
	public:
	// Note: height = makes it skinnier or thicker
	Lightning (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	}
};

void initLightSprite()
{
	globalSprite.light = new Sprite("electricity.png", 1, 1, 1, 1, 200, 600);
	//globalSprite.light->setPos(gl.xres / 2, gl.yres / 2);
}

void renderLightSprite()
{        
	//globalSprite.light->setPos(-gl.camera[0], 100);
	globalSprite.light->draw();
}

void physicsLightSprite()
{
    static float pos = 0;
    Sprite* sp = globalSprite.light;
    if (pos > gl.xres + sp->getWidth()) {
        pos = -sp->getWidth();
    }
    sp->setPos(pos, gl.yres / 2);
    pos += 10;
}


