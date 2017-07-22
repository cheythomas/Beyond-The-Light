// Author by: Cheyenne Thomas
// Date: 6-23-17
// Purpose: Create light, battery, endgame, etc
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
// Week 7: added lightning sprite and health/energy bar. 
//		   Got game over when once health/energy bar reaches empty.
//
// Week 8: Made lightning gif to move with character sprite.
//		Do the credits for the game. 
//
#include "game.h"

void Battery::chargeObject()
{
	// a battery on the ground
	float w, h, z, x, y;
	w = 34; // width size
	h = 12; // length size
	z = 0.0;
	x = 740; // x-axis
	y = 50; // y-axis
	glColor3f(0.0, 1.0, 0.0); 
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
	ggprint8b(&r, 16, c, "Hit R to grab");
}

void Battery::grabCharge()
{
	Rect r;
	r.bot = 60; // y-axis
	r.left = 700; // x-axis
	r.center = 0;
	unsigned int c = 0x00ffff44;
	if (gl.keys[XK_r] || gl.keys[XK_R]) {
		if (gl.keepTrack == 0) {
			ggprint8b(&r, 16, c, "Already full");
		} 
		else if (gl.keepTrack > 0) {
			//add something here
			ggprint8b(&r, 16, c, "nice!");
		}
	}
}

void Battery::healthBar()
{	
	// not sure yet
}

// old 
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
		//bcount++;
	}
}


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
	globalSprite.life[0] = new Sprite("10.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[1] = new Sprite("9.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[2] = new Sprite("8.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[3] = new Sprite("7.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[4] = new Sprite("6.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[5] = new Sprite("5.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[6] = new Sprite("4.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[7] = new Sprite("3.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[8] = new Sprite("2.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[9] = new Sprite("1.png", 1, 10, 1, 1, 15, 105);
	globalSprite.life[10] = new Sprite("0.png", 1, 10, 1, 1, 15, 105);
}

void renderLifeBarSprite()
{       
	int x = gl.xres-72; //800 
	int y = gl.yres-20;  //600
	
	// full to empty
	if (gl.keyCount == 0 || 51 > gl.keyCount) {
		globalSprite.life[0]->draw();
		globalSprite.life[0]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 0;
	}

	if (gl.keyCount > 50) {
		globalSprite.life[1]->draw();
		globalSprite.life[1]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 1;
	}
	
	if (gl.keyCount > 100) {
		globalSprite.life[2]->draw();
		globalSprite.life[2]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 2;
	}
	
	if (gl.keyCount > 250) {
		globalSprite.life[3]->draw();
		globalSprite.life[3]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 3;
	}
	
	if (gl.keyCount > 300) {
		globalSprite.life[4]->draw();
		globalSprite.life[4]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 4;
	}
	
	if (gl.keyCount > 350) {
		globalSprite.life[5]->draw();
		globalSprite.life[5]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 5;
	}
	
	if (gl.keyCount > 400) {
		globalSprite.life[6]->draw();
		globalSprite.life[6]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 6;
	}
	
	if (gl.keyCount > 450) {
		globalSprite.life[7]->draw();
		globalSprite.life[7]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 7;
	}
	
	if (gl.keyCount > 500) {
		globalSprite.life[8]->draw();
		globalSprite.life[8]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 8;
	}
	
	if (gl.keyCount > 550) {
		globalSprite.life[9]->draw();
		globalSprite.life[9]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 9;
	}
	
	if (gl.keyCount == 600 || gl.keyCount > 600) { 
		globalSprite.life[10]->draw();
		globalSprite.life[10]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 10;
	}
}

class GameOver : public Sprite {
	
	public:
	// (file, frameCount, row, col, delay, h, w)
	GameOver (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	}
};

void initGameOverSprite()
{
	globalSprite.gameover = new Sprite("go.png", 1, 1, 1, 1, 200, 400);
}

void renderGameOverSprite()
{     
	int x = gl.xres-370; //800 
	int y = gl.yres-300;  //600

	if (gl.keepTrack == 10e9) {
		globalSprite.gameover->draw();
		globalSprite.gameover->setPos(-gl.camera[0] + x, y);
	}
}

void Battery::gameOver()
{
	Rect r;
	float h, w;
	if (gl.keepTrack == 10e9) {
		h = gl.yres;
		w = gl.xres;
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
		ggprint8b(&r, 16, 0, "");
	}
}

class Lightning : public Sprite {
	
	public:
	Lightning (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	}
};

void initLightSprite()
{
	globalSprite.light[0] = new Sprite("lightning.gif", 10, 10, 1, 0.055555, 148, 203);
	//globalSprite.light[0] = new Sprite("electricityForward.gif", 4, 1, 4, 4.0f/16.0f, 200, 220);
	//globalSprite.light[1] = new Sprite("electricityUp.gif", 4, 1, 4, 4.0f/16.0f, 360, 140);
	//globalSprite.light[2] = new Sprite("electricityDiagonalLeft.gif", 4, 1, 4, 4.0f/16.0f, 360, 140);
	//globalSprite.light[3] = new Sprite("electricityDiagonalRight.gif", 4, 1, 4, 4.0f/16.0f, 360, 140);
	//	for (int i = 1; i < 4; i++) {
	globalSprite.light[0]->setVisible(false);
	//	}
}

void renderLightSprite()
{       
	//for (int i = 0; i < 4; i++) {
		globalSprite.light[0]->draw();
	//}
}

void physicsLightSprite()
{
	//for (int i = 0; i < 4; i++) {
	globalSprite.light[0]->physics();	
	//}
	//	int dirX = 100;
	//	int dirY = 100;
	//	int half = 256;
	Sprite* m = globalSprite.mortana;
	Sprite* l = globalSprite.light[0];
	int mortDir = m->getDirection();
	float cmx = gl.mortanaPos[0],
		cmy = gl.mortanaPos[1];
	//	if (globalSprite.mortana->getDirection() == 1) { //direction to the right
	//		globalSprite.light[0]->setPos(gl.mortanaPos[0], gl.xres/2, gl.mortanaPos[1] + 100);
	//		globalSprite.light[1]->setPos(gl.mortanaPos[0], gl.mortanaPos[1] + dirY);
	//		globalSprite.light[2]->setPos(gl.mortanaPos[0] - dirX, gl.mortanaPos[1] + dirY);
	//		globalSprite.light[3]->setPos(gl.mortanaPos[0] + dirX, gl.mortanaPos[1] + dirY);
	//	} else {
		//l->setPos(gl.mortanaPos[0]);
		//	globalSprite.light[1]->setPos(gl.mortanaPos[0], gl.mortanaPos[1] + dirY);
		//	globalSprite.light[2]->setPos(gl.mortanaPos[0] + dirX - half, gl.mortanaPos[1] + dirY);
		//	globalSprite.light[3]->setPos(gl.mortanaPos[0] - dirX + half, gl.mortanaPos[1] + dirY);
	//	}
	bool keyPressed = false;
	if (gl.keys[XK_d] || gl.keys[XK_D] || gl.keys[XK_a] || gl.keys[XK_A]) {
		l->setVisible(true);
		if (mortDir == 1) {	
			l->setPos(cmx + 120, cmy);
			l->setAngle(0);
		} else {
			l->setPos(cmx - 120, cmy);		
			l->setAngle(180);
		}
		keyPressed = true;
	} else if (gl.keys[XK_w] || gl.keys[XK_W]) {
		l->setVisible(true);	
		if (mortDir == 1) {	
			l->setPos(cmx, cmy + 128);
		} else {
			l->setPos(cmx, cmy + 128);		
		}
		l->setAngle(90);
		keyPressed = true;
	} else if (gl.keys[XK_q] || gl.keys[XK_Q]) {
		l->setVisible(true);	
		if (mortDir == 1) {	
			l->setPos(cmx - 150*0.707, cmy +128*0.707);
		} else {
			l->setPos(cmx - 150*0.707, cmy +128*0.707);		
		}
		l->setAngle(135);
		keyPressed = true;
	} else if (gl.keys[XK_e] || gl.keys[XK_E]) {
		l->setVisible(true);	
		if(mortDir == 1) {	
			l->setPos(cmx + 150*0.707, cmy + 128*0.707);
		} else {
			l->setPos(cmx + 150*0.707, cmy + 128*0.707);		
		}
		l->setAngle(45);
		keyPressed = true;
	} else {
		l->setVisible(false);
	}
	
	if(keyPressed) {
		gl.keyCount = gl.keyCount + 1;
		printf("keyCount: %d\n", gl.keyCount);
	}
}

class Credits: public Sprite {
	
	public:
	Credits (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	}
};

void initCreditBackground()
{
	globalSprite.credits[0] = new Sprite("creditTitle.png", 1, 1, 1, 1, 148, 203);
	globalSprite.credits[1] = new Sprite("creditKaren.png", 1, 1, 1, 1, 148, 203);
	globalSprite.credits[2] = new Sprite("creditAurora.png", 1, 1, 1, 1, 148, 203);
	globalSprite.credits[3] = new Sprite("creditCheyenne.png", 1, 1, 1, 1, 148, 203);
}

void renderCreditBackground()
{
	if (gl.state == STATE_CREDITS) {
		if (gl.keys[XK_Return]) {
		// black background  
		float h, w;
		h = gl.yres;
		w = gl.xres;
		glPushMatrix();
		glColor3f(0.0, 0.0, 0.0);
		glTranslated(gl.xres/2, gl.yres/2, 0);
		glBegin(GL_QUADS); 
			glVertex2i(-w, -h);
			glVertex2i(-w,  h);
			glVertex2i( w,  h);
			glVertex2i( w, -h);
		glEnd();
		glPopMatrix();

		// Font
		int x = gl.xres/2; //800 
		int y = gl.yres-30;  //600
		for (int i = 0; i < 4; i++) {
			globalSprite.credits[i]->draw();
			globalSprite.life[0]->setPos(-gl.camera[0] + x, y);
			y -= 100;
		}
	}
	}
}
