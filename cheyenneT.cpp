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

// the more key press to kill ghost, battery drains
bool Battery::deleteBattery()
{	
	bool empty = false;
	if (gl.keyCount > 10) {
		//arr[0] = 0; // fix later
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
		empty = true;
	}
	return empty;
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


	//Sprite(shock.gif,5,1,1,1,10,20);

}

void initShock(void)
{
	system("convert ./images/shock.gif ./images/shock.ppm");
	gl.shockImage = ppm6GetImage("./images/shock.ppm");

	int w2 = gl.shockImage->width;
	int h2 = gl.shockImage->height;
 
//	glGenTextures(1, &gl.shockTexture);
//	glBindTexture(GL_TEXTURE_2D, gl.shockTexture);
	unsigned char *shockData = buildAlphaData(gl.shockImage); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, shockData);

	free(shockData);
	unlink("./images/shock.ppm");
}

void physicsShock(void)
{
	if (gl.shock || gl.keys[XK_e]) {
//		timers.recordTime(&timers.timeCurrent);
//		double timeSpan = 
//			timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
		if (0 > gl.delay) {
			//advance
			++gl.shockFrame;
			if (gl.shockFrame >= 9) {
				gl.shockFrame = 0;
				gl.shock = 0;
			}
//			timers.recordTime(&timers.shockTime);
		}
	}
}

void renderShock(void)
{
	float cx = gl.xres/2.0;
	float cy = gl.yres/2.0;
	
	if (gl.keys[XK_e]) {
		float h2 = 200.0;
		float w2 = 200 * 0.5;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
//		glBindTexture(GL_TEXTURE_2D, gl.shockTexture);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		int ix = gl.shockFrame % 9;
		int iy = 0;
//		if (gl.shockFrame >= 9)
//			iy = 1;
		float tx2 = (float)ix / 8.0;
		float ty2 = (float)iy / 2.0;
		glBegin(GL_QUADS);
		if (gl.keys[XK_e]) {
			glTexCoord2f(tx2,      ty2+.5); glVertex2i(cx-w2, cy-h2);
			glTexCoord2f(tx2,      ty2);    glVertex2i(cx-w2, cy+h2);
			glTexCoord2f(tx2+.125, ty2);    glVertex2i(cx+w2, cy+h2);
			glTexCoord2f(tx2+.125, ty2+.5); glVertex2i(cx+w2, cy-h2);
		}
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
}

 
void Battery::gameOver()
{
	Rect r;
	float h, w;
	//if (bcount == 0 /* || collisionWithGhost */) {
	// testing to get screen to popup
	if (gl.keyCount > 200) {
		h = 100.0;
		w = 200.0;
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0, 1.0, 0.0, 0.8);
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
		r.bot = gl.yres/2 + 80;
		r.left = gl.xres/2;
		r.center = 1;
		ggprint8b(&r, 16, 0, "GAME OVER");
	}
}

// not using yet
void LightCollison()
{
    //   
}


