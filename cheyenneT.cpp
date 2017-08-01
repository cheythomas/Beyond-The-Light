// Author by: Cheyenne Thomas
// Date: 6-23-17
// Purpose: Create light, battery, endgame, etc
//
// Week 4: Draw the battery and its battery bars, 4. 
// 	   write a function that shows the flashlight running out of bars
//         draw flashlight
//
// Week 5: fixed functions
// 
// Week 6: added sprite, called functions, get game to gameover
//
// Week 7: added lightning sprite and health/energy bar. 
//	   Got game over when once health/energy bar reaches empty.
//
// Week 8: Made lightning gif to move with character sprite.
//	   Do the credits for the game. 
//
// Week 9: Made Highscore screen, added to Credit Screen. Changed gameOver to a state. 
// 	   Produce scores and show live on screen. Added a hard mode gameplay.
#include "game.h"

class myTimers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec gameTime, overTime, modeTime;
	myTimers() {
		physicsRate = 1.0 / 30.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
} timers;

//refactoring
void Battery::drawBattery(float wid, float len, float x, float y, float z)
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
		glVertex2i(-wid, -len);
		glVertex2i(-wid,  len);
		glVertex2i( wid,  len);
		glVertex2i( wid, -len);
	glEnd();
	glPopMatrix();
}


// a energy bar on the ground
void Battery::energybarAppears()
{
	if (gl.keepTrack == 3 || gl.keepTrack == 5 || gl.keepTrack == 7
			|| gl.keepTrack == 9 || gl.keepTrack == 10) {
		float w, h, z, x, y;
		w = 34; 
		h = 8;
		z = 0.0;
		x = 740; 
		y = 50; 
		glColor3f(0.0, 1.0, 0.0);
	       	
		//refractored	
		drawBattery(w, h, x, y, z);
		drawBattery(w, h, 4000, y, z);
		drawBattery(w, h, 2000, y, z);

		Rect r;
		r.bot = 80; 
		r.left = 691; // x-axis
		r.center = 0;
		unsigned int c = 0x00ffff44;
		ggprint8b(&r, 16, c, "Press R to collect");
		r.left = 2941; // x-axis
		ggprint8b(&r, 16, c, "Press R to collect");
	}
}

void renderText()
{
	Rect r;
	unsigned int c = 0x00ffff44;
	if (gl.hardSelection == 0) {
		r.left = -gl.camera[0] + 100; // x
		r.bot = gl.yres-20;  // y axis
		r.center = 0;
		ggprint8b(&r, 16, c, "h - turn on/off Hard Mode");
	
		int pts = gl.points;
		r.left = -gl.camera[0] + (gl.xres*.84); // x
		r.bot = gl.yres*.034;  // y axis
		r.center = 0;
		ggprint16(&r, 16, c, "score: %d", pts);
	} else if(gl.hardSelection == 1) {
		int pts = gl.points;
		r.left = -gl.camera[0] + (gl.xres*.84); // x
		r.bot = gl.yres*.04;  // y axis
		r.center = 0;
		ggprint16(&r, 16, c, "score: %d", pts);
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
	globalSprite.life[0] = new Sprite("10.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[1] = new Sprite("9.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[2] = new Sprite("8.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[3] = new Sprite("7.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[4] = new Sprite("6.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[5] = new Sprite("5.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[6] = new Sprite("4.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[7] = new Sprite("3.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[8] = new Sprite("2.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[9] = new Sprite("1.png", 1, 10, 1, 1, 40, 105);
	globalSprite.life[10] = new Sprite("0.png", 1, 10, 1, 1, 40, 105);
}

void renderLifeBarSprite()
{       
	int x = gl.xres-72; //800 
	int y = gl.yres-20;  //600
	
	// full to empty
	if (gl.keyCount == 0 || 101 > gl.keyCount) {
		globalSprite.life[0]->draw();
		globalSprite.life[0]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 0;
	}

	if (gl.keyCount > 100) {
		globalSprite.life[1]->draw();
		globalSprite.life[1]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 1;
	}
	
	if (gl.keyCount > 200) {
		globalSprite.life[2]->draw();
		globalSprite.life[2]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 2;
	}
	
	if (gl.keyCount > 300) {
		globalSprite.life[3]->draw();
		globalSprite.life[3]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 3;
	}
	
	if (gl.keyCount > 400) {
		globalSprite.life[4]->draw();
		globalSprite.life[4]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 4;
	}
	
	if (gl.keyCount > 500) {
		globalSprite.life[5]->draw();
		globalSprite.life[5]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 5;
	}
	
	if (gl.keyCount > 600) {
		globalSprite.life[6]->draw();
		globalSprite.life[6]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 6;
	}
	
	if (gl.keyCount > 700) {
		globalSprite.life[7]->draw();
		globalSprite.life[7]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 7;
	}
	
	if (gl.keyCount > 800) {
		globalSprite.life[8]->draw();
		globalSprite.life[8]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 8;
	}
	
	if (gl.keyCount > 900) {
		globalSprite.life[9]->draw();
		globalSprite.life[9]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 9;
	}
	
	if (gl.keyCount == 1000 || gl.keyCount > 1000) { 
		globalSprite.life[10]->draw();
		globalSprite.life[10]->setPos(-gl.camera[0] + x, y);
		gl.keepTrack = 10;
		gl.state = STATE_GAMEOVER;
		restart();
	}

	// adds a life bar after R key press	
	int pressedR = 0;
	if (gl.keepTrack == 3 || gl.keepTrack == 5 || gl.keepTrack == 7
			|| gl.keepTrack == 9 || gl.keepTrack == 10) {
		if (pressedR == 0) {
			Rect r;
			r.bot = gl.yres-20; // y-axis
			r.left = gl.xres-110; // x-axis
			r.center = 0;
			unsigned int c = 0x00ffff44;
			if (gl.keys[XK_r] || gl.keys[XK_R]) {
				if (gl.keepTrack == 0) {
					ggprint8b(&r, 16, c, "Already full");
				}	 
				else if (gl.keepTrack > 0) {
					ggprint8b(&r, 16, c, "nice!");
					pressedR = 1;
					playGrab();
				}
			}
		}
		if (pressedR == 1) {
			gl.keyCount -= 100;
			gl.keepTrack = gl.keepTrack - 1;
			globalSprite.life[gl.keepTrack]->draw();
		} 
	} else {
		// do nothing
	}
}

class GameOver : public Sprite {
	
	public:
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
	int x = gl.xres*0.538;  
	int y = gl.yres*0.5;  

	if (gl.keepTrack == 10) {
		globalSprite.gameover->draw();
		globalSprite.gameover->setPos(x, y);
	}
}

void Battery::gameOver()
{
	float h, w;
	if (gl.keepTrack == 10) {
		h = gl.yres;
		w = gl.xres;
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0, 1.0, 1.0, 0.7);
		glTranslated(gl.xres, gl.yres, 0);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w,  h);
			glVertex2i( w,  h);
			glVertex2i( w, -h);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		//Rect r;
		//unsigned int c = 0x00FF0000;
		//r.left = gl.xres*0.33; 
		//r.bot = gl.yres*.20;  
		//r.center = 0;
		//ggprint40(&r, 16, c, "Press spacebar");
	
		if (gl.spacebar == 1) {
			gl.state = STATE_HIGHSCORE;
		}
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
	globalSprite.light[0]->setVisible(false);
}

void renderLightSprite()
{       
		globalSprite.light[0]->draw();
}

void physicsLightSprite()
{
	globalSprite.light[0]->physics();	
	Sprite* m = globalSprite.mortana;
	Sprite* l = globalSprite.light[0];
	int mortDir = m->getDirection();
	float cmx = gl.mortanaPos[0],
		cmy = gl.mortanaPos[1];
	bool keyPressed = false;
	
	if (gl.keepTrack == 10) {
		// do nothing
		l->setVisible(false);
	} else {
		if (gl.keys[XK_d] || gl.keys[XK_D] || gl.keys[XK_a] || gl.keys[XK_A]) {
			playPoint();
			l->setVisible(true);
			if (mortDir == 1) {	/* FACING RIGHT DIR: forward attack */
				l->setPos(cmx + 120, cmy);
				l->setAngle(0);
				gl.lightning = 1;
			} else { /* FACING LEFT: forward attack*/
				l->setPos(cmx - 120, cmy);		
				l->setAngle(180);
				gl.lightning = 5;
			}
			keyPressed = true;
		} else if (gl.keys[XK_w] || gl.keys[XK_W]) {
			playPoint();
			l->setVisible(true);	
			if (mortDir == 1) {	/* ATTACK UPWARD */
				l->setPos(cmx, cmy + 128);
			} else {
				l->setPos(cmx, cmy + 128);		
			}
			l->setAngle(90);
			keyPressed = true;
			gl.lightning = 3;
		} else if (gl.keys[XK_q] || gl.keys[XK_Q]) {
			playPoint();
			l->setVisible(true);	
			if (mortDir == 1) {	/* FACING RIGHT: attack left diagonally */
				l->setPos(cmx - 150*0.707, cmy +128*0.707);
			} else { /* FACING LEFT */ 
				l->setPos(cmx - 150*0.707, cmy +128*0.707);		
			}
			l->setAngle(135);
			keyPressed = true;
			gl.lightning = 4;
		} else if (gl.keys[XK_e] || gl.keys[XK_E]) {
			playPoint();
			l->setVisible(true); /* attack right diagonally */
			if (mortDir == 1) {	
				l->setPos(cmx + 150*0.707, cmy + 128*0.707);
			} else {
				l->setPos(cmx + 150*0.707, cmy + 128*0.707);		
			}
			l->setAngle(45);
			keyPressed = true;
			gl.lightning = 2;
		} else {
			l->setVisible(false);
			gl.lightning = 0;
		}
	}

	if (keyPressed) {
		gl.keyCount = gl.keyCount + 1;
		//printf("keyCount: %d\n", gl.keyCount);
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
	globalSprite.credits[0] = new Sprite("creditTitle2.png", 1, 1, 1, 1, 140, 700);
	globalSprite.credits[1] = new Sprite("creditKaren.png", 1, 1, 1, 1, 50, 250);
	globalSprite.credits[2] = new Sprite("creditKaren1.png", 1, 1, 1, 1, 35, 330);
	globalSprite.credits[3] = new Sprite("creditAurora.png", 1, 1, 1, 1, 50, 250);
	globalSprite.credits[4] = new Sprite("creditAurora1.png", 1, 1, 1, 1, 30, 215);
	globalSprite.credits[5] = new Sprite("creditCheyenne2.png", 1, 1, 1, 1, 50, 200);
	globalSprite.credits[6] = new Sprite("creditAurora1.png", 1, 1, 1, 1, 30, 215);
	globalSprite.credits[7] = new Sprite("creditThanks.png", 1, 1, 1, 1, 50, 200);
	globalSprite.credits[8] = new Sprite("creditG.png", 1, 1, 1, 1, 30, 200);
	globalSprite.credits[9] = new Sprite("creditCats.png", 1, 1, 1, 1, 30, 200);
	globalSprite.credits[10] = new Sprite("creditJosie.png", 1, 1, 1, 1, 30, 200);
}

void renderCreditBackground()
{
	if (gl.state == STATE_CREDITS) {
		// background color
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
		
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.329412, 0.329412, 0.329412, 0.3);
		glBegin(GL_QUAD_STRIP);
			glVertex2i(0,                       0);
			glVertex2i(50,                     50);
			glVertex2i(gl.xres,                 0);
			glVertex2i(gl.xres*.938,           50);
			glVertex2i(gl.xres,           gl.yres);
			glVertex2i(gl.xres*.938, gl.yres*.917);
			glVertex2i(0,                 gl.yres);
			glVertex2i(50,           gl.yres*.917);
			glVertex2i(0,                       0);
			glVertex2i(50,                     50);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();

		// Font
		int x = gl.xres/2;  
		int y = gl.yres*.9;  
		globalSprite.credits[0]->draw();
		globalSprite.credits[0]->setPos(x, y);
		x = gl.xres/2;  
		y = gl.yres*.76;  
		for (int i = 1; i < 11; i++) {
			globalSprite.credits[i]->draw();
			globalSprite.credits[i]->setPos(x, y);
			if (i%2 == 1 && i < 8) {
				y -= 35;
			} else if (i%2 != 1 && i < 8) {
				y -= 70; 
			}
			if (i > 7) {
				y -= 35;
			}
		}
	}
}

class highScores : public Sprite {
	
	public:
	highScores (const std::string & filename, float height, float width) :
		Sprite(filename, 1, 1, 1, 1, height, width) { }

	void draw() {
		Sprite::draw();
	}
};

void initHighScores()
{
	globalSprite.scores[0] = new Sprite("scoresTitle.png", 1, 1, 1, 1, 200, 500);
}

void renderHighScores()
{       
	int x = gl.xres/2;  
	int y = gl.yres*.83;  

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glColor4f(1.0, 0.25, 0.0, 0.3);
	glColor4f(0.329412, 0.329412, 0.329412, 0.3);
	glBegin(GL_QUAD_STRIP);
		glVertex2i(0,                       0);
		glVertex2i(50,                     50);
		glVertex2i(gl.xres,                 0);
		glVertex2i(gl.xres*.938,           50);
		glVertex2i(gl.xres,           gl.yres);
		glVertex2i(gl.xres*.938, gl.yres*.917);
		glVertex2i(0,                 gl.yres);
		glVertex2i(50,           gl.yres*.917);
		glVertex2i(0,                       0);
		glVertex2i(50,                     50);
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	
	globalSprite.scores[0]->setPos(x, y);
	globalSprite.scores[0]->draw();
	
	int score = 0;
	score = gl.savescore;

	Rect r;	
	unsigned int c = 0x00FF0000;
	r.left = gl.xres*0.28; 
	r.bot = gl.yres*.58;  
	r.center = 0;
	
	ggprint40(&r, 16, c, "Top Scores");
	ggprint17(&r, 16, c, " ");
	//printf("Rendering %d scores\n", gl.scores.size());
	for (unsigned int i = 0; i < gl.scores.size(); i++) {
		Score s = gl.scores[i];
		ggprint17(&r, 32, c, "%s - %d", s.name.c_str(), s.score);
	}
	
	r.bot = gl.yres*.24;  
	ggprint40(&r, 16, c, "Your high score            %d", score);
	r.bot = gl.yres*.20;  

	r.left = gl.xres*0.33; 
	r.bot = gl.yres*.04;  
	r.center = 0;
	ggprint16(&r, 16, c, "Press ESC for Main Menu");
	
}

// Screen flashes once down to last life bar
void redScreenFlash()
{
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = 
	timers.timeDiff(&timers.gameTime, &timers.timeCurrent);
	
	float w, h, z, x, y;
	w = gl.xres;
	h = gl.yres; 
	z = 0;
	x = -gl.camera[0]; 
	y = gl.yres/2; 
	int flash = 0;

	if (timeSpan > 1.0) {
		timers.timeCopy(&timers.gameTime, &timers.timeCurrent);
		flash++;
	}
	// flashes screen
	if (gl.keepTrack == 9 && flash >= 1) {
		glColor4f(1.0, 0.0, 0.0, 0.5); 
		glPushMatrix();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTranslatef(x, y, z);
		glBegin(GL_QUADS);
			glVertex2i(-w, -h);
			glVertex2i(-w,  h);
			glVertex2i( w,  h);
			glVertex2i( w, -h);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
	}	
}

void hardMode()
{
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = 
		timers.timeDiff(&timers.modeTime, &timers.timeCurrent);
	if (gl.hardSelection == 1 && (gl.state == STATE_GAMEPLAY)) {
		float w, h, z, x, y;
		w = gl.xres; // width size
		h = gl.yres; // length size
		z = 0;
		x = -gl.camera[0]; // x-axis
		y = gl.yres/2; // y-axis
		int flash = 0;

		if (timeSpan > 0.5) {
			timers.timeCopy(&timers.modeTime, &timers.timeCurrent);
			flash++;
		}
		// darken screen
		if (gl.keepTrack >= 0) {
			glColor4f(0.0, 0.0, 0.0, 0.87); 
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
				glVertex2i(-w, -h);
				glVertex2i(-w,  h);
				glVertex2i( w,  h);
				glVertex2i( w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
		}
		// flash screen
		if (timeSpan > 0.2) {
			glColor4f(0.0, 0.0, 0.0, 0.96);
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
				glVertex2i(-w, -h);
				glVertex2i(-w,  h);
				glVertex2i( w,  h);
				glVertex2i( w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
		} 
	}
}

void trackKills(int)
{
	if (gl.hardSelection == 0) {
		gl.points += 10;
	} else if (gl.hardSelection == 1) {
		gl.points += 20;
	}
	gl.ghostKilled++;
	gl.savescore = gl.points;
}

// When game is over, reset values for Restart game/Play again
void cheyRestart()
{
	gl.points = 0;
	gl.keyCount = 0;
	globalSprite.life[10]->draw();
	gl.hardSelection = 0;
}

// special
void disco()
{
	timers.recordTime(&timers.timeCurrent);
	double timeSpan = 
		timers.timeDiff(&timers.modeTime, &timers.timeCurrent);
	if (gl.danceParty == 1 && (gl.state == STATE_GAMEPLAY)) {
		float w, h, z, x, y;
		w = gl.xres; // width size
		h = gl.yres; // length size
		z = 0;
		x = -gl.camera[0]; // x-axis
		y = gl.yres/2; // y-axis
		int flash = 0;

		if (timeSpan > 0.5) {
			timers.timeCopy(&timers.modeTime, &timers.timeCurrent);
			flash++;
		}
		
		if (gl.keepTrack >= 0) {
			glColor4f(1.0, 0.0, 0.0, 0.4); 
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
				glVertex2i(-w, -h);
				glVertex2i(-w,  h);
				glVertex2i( w,  h);
				glVertex2i( w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
		}
		// flash screen
		if (timeSpan > 0.2) {
			glColor4f(0.0, 1.0, 0.0, 0.4);
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
				glVertex2i(-w, -h);
				glVertex2i(-w,  h);
				glVertex2i( w,  h);
				glVertex2i( w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
		} 
		if (timeSpan > 0.1) {
			glColor4f(0.0, 0.0, 1.0, 0.4);
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
				glVertex2i(-w, -h);
				glVertex2i(-w,  h);
				glVertex2i( w,  h);
				glVertex2i( w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
		}
		if (timeSpan > 0.3) {
			glColor4f(1.0, 1.0, 1.0, 0.4);
			glPushMatrix();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslatef(x, y, z);
			glBegin(GL_QUADS);
				glVertex2i(-w, -h);
				glVertex2i(-w,  h);
				glVertex2i( w,  h);
				glVertex2i( w, -h);
			glEnd();
			glDisable(GL_BLEND);
			glPopMatrix();
		}
	}
}
