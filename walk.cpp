//3350
//program: walk.cpp
//author:  Gordon Griesel
//date:    summer 2017
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
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

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//X Windows variables
Display *dpy;
Window win;

//function prototypes
void initXWindows(void);
void initOpengl(void);
void cleanupXWindows(void);
void checkResize(XEvent *e);
void checkMouse(XEvent *e);
void checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);

//-----------------------------------------------------------------------------
//Setup timers
class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	Timers() {
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
//-----------------------------------------------------------------------------

class Sprite {
public:
	int onoff;
	int frame;
	double delay;
	Vec pos;
	Ppmimage *image;
	GLuint tex;
	struct timespec time;
	Sprite() {
		onoff = 0;
		frame = 0;
		image = NULL;
		delay = 0.1;
	}
};


class Global {
public:
	unsigned char keys[65536];
	int done;
	int xres, yres;
	int movie, movieStep;
	int walk;
	int walkFrame;
	double delay;
	Ppmimage *walkImage;
	GLuint walkTexture;
	Vec box[20];
	Sprite exp;
	Sprite exp44;
	//camera is centered at (0,0) lower-left of screen. 
	Flt camera[2];
	Vec ball_pos;
	Vec ball_vel;
	~Global() {
		logClose();
	}
	Global() {
		logOpen();
		camera[0] = camera[1] = 0.0;
		ball_pos[0] = 500.0;
		ball_pos[1] = ball_pos[2] = 0.0;
		ball_vel[0] = ball_vel[1] = ball_vel[2] = 0.0;
		done=0;
		movie=0;
		movieStep=2;
		xres=800;
		yres=600;
		walk=0;
		walkFrame=0;
		walkImage=NULL;
		delay = 0.1;
		exp.onoff=0;
		exp.frame=0;
		exp.image=NULL;
		exp.delay = 0.02;
		exp44.onoff=0;
		exp44.frame=0;
		exp44.image=NULL;
		exp44.delay = 0.022;
		for (int i=0; i<20; i++) {
			box[i][0] = rnd() * xres;
			box[i][1] = rnd() * (yres-220) + 220.0;
			box[i][2] = 0.0;
		}
		memset(keys, 0, 65536);
	}
} gl;

class Level {
public:
	int dynamicHeight[180];
	unsigned char arr[16][80];
	int nrows, ncols;
	int tilesize[2];
	Flt ftsz[2];
	Flt tile_base;
	Level() {
		//Log("Level constructor\n");
		for (int i = 0; i < 180; i++)
		    dynamicHeight[i] = -1;
		tilesize[0] = 32;
		tilesize[1] = 32;
		ftsz[0] = (Flt)tilesize[0];
		ftsz[1] = (Flt)tilesize[1];
		tile_base = 220.0;
		//read level
		FILE *fpi = fopen("level1.txt","r");
		if (fpi) {
			nrows=0;
			char line[100];
			while (fgets(line, 100, fpi) != NULL) {
				removeCrLf(line);
				int slen = strlen(line);
				ncols = slen;
				//Log("line: %s\n", line);
				for (int j=0; j<slen; j++) {
					arr[nrows][j] = line[j];
				}
				++nrows;
			}
			fclose(fpi);
			//printf("nrows of background data: %i\n", nrows);
		}
		for (int i=0; i<nrows; i++) {
			for (int j=0; j<ncols; j++) {
				printf("%c", arr[i][j]);
			}
			printf("\n");
		}
	}
	void removeCrLf(char *str) {
		char *p = str;
		while (*p) {
			if (*p == 10 || *p == 13) {
				*p = '\0';
				break;
			}
			++p;
		}
	}
} lev;


int main(void)
{
	initXWindows();
	initOpengl();
	init();
	while (!gl.done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			checkResize(&e);
			checkMouse(&e);
			checkKeys(&e);
		}
		physics();
		render();
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	return 0;
}

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void setTitle(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "3350 - Walk Cycle");
}

void setupScreenRes(const int w, const int h)
{
	gl.xres = w;
	gl.yres = h;
}

void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XSetWindowAttributes swa;
	setupScreenRes(gl.xres, gl.yres);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		printf("\n\tcannot connect to X server\n\n");
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		printf("\n\tno appropriate visual found\n\n");
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
						StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
							vi->depth, InputOutput, vi->visual,
							CWColormap | CWEventMask, &swa);
	GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	setTitle();
}

void reshapeWindow(int width, int height)
{
	//window has been resized.
	setupScreenRes(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	setTitle();
}

unsigned char *buildAlphaData(Ppmimage *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	system("convert ./images/walk.gif ./images/walk.ppm");
	gl.walkImage = ppm6GetImage("./images/walk.ppm");
	int w = gl.walkImage->width;
	int h = gl.walkImage->height;
	//
	//create opengl texture elements
	glGenTextures(1, &gl.walkTexture);
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *walkData = buildAlphaData(gl.walkImage);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	free(walkData);
	unlink("./images/walk.ppm");
	//-------------------------------------------------------------------------
	system("convert ./images/exp.png ./images/exp.ppm");
	gl.exp.image = ppm6GetImage("./images/exp.ppm");
	w = gl.exp.image->width;
	h = gl.exp.image->height;
	//create opengl texture elements
	glGenTextures(1, &gl.exp.tex);
	//-------------------------------------------------------------------------
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	unsigned char *xData = buildAlphaData(gl.exp.image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);
	unlink("./images/exp.ppm");
	//-------------------------------------------------------------------------
	system("convert ./images/exp44.png ./images/exp44.ppm");
	gl.exp44.image = ppm6GetImage("./images/exp44.ppm");
	w = gl.exp44.image->width;
	h = gl.exp44.image->height;
	//create opengl texture elements
	glGenTextures(1, &gl.exp44.tex);
	//-------------------------------------------------------------------------
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	xData = buildAlphaData(gl.exp44.image);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);
	unlink("./images/exp44.ppm");
}

void checkResize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != gl.xres || xce.height != gl.yres) {
		//Window size did change.
		reshapeWindow(xce.width, xce.height);
	}
}

void init() {

}

void checkMouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

void screenCapture()
{
	static int fnum = 0;
	static int vid = 0;
	if (!vid) {
		system("mkdir ./vid");
		vid = 1;
	}
	unsigned char *data = (unsigned char *)malloc(gl.xres * gl.yres * 3);
    glReadPixels(0, 0, gl.xres, gl.yres, GL_RGB, GL_UNSIGNED_BYTE, data);
	char ts[32];
	sprintf(ts, "./vid/pic%03i.ppm", fnum);
	FILE *fpo = fopen(ts,"w");	
	if (fpo) {
		fprintf(fpo, "P6\n%i %i\n255\n", gl.xres, gl.yres);
		unsigned char *p = data;
		//go backwards a row at a time...
		p = p + ((gl.yres-1) * gl.xres * 3);
		unsigned char *start = p;
		for (int i=0; i<gl.yres; i++) {
			for (int j=0; j<gl.xres*3; j++) {
				fprintf(fpo, "%c",*p);
				++p;
			}
			start = start - (gl.xres*3);
			p = start;
		}
		fclose(fpo);
		char s[256];
		sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
		system(s);
		unlink(ts);
	}
	++fnum;
}

void checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = XLookupKeysym(&e->xkey, 0);
	gl.keys[key]=1;
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return;
	}
	if (e->type == KeyPress) {
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return;
		}
	} else {
		return;
	}
	if (shift) {}
	switch (key) {
		case XK_s:
			screenCapture();
			break;
		case XK_m:
			gl.movie ^= 1;
			break;
		case XK_w:
			timers.recordTime(&timers.walkTime);
			gl.walk ^= 1;
			break;
		case XK_e:
			gl.exp.pos[0] = 200.0;
			gl.exp.pos[1] = -60.0;
			gl.exp.pos[2] =   0.0;
			timers.recordTime(&gl.exp.time);
			gl.exp.onoff ^= 1;
			break;
		case XK_f:
			gl.exp44.pos[0] = 200.0;
			gl.exp44.pos[1] = -60.0;
			gl.exp44.pos[2] =   0.0;
			timers.recordTime(&gl.exp44.time);
			gl.exp44.onoff ^= 1;
			break;
		case XK_Left:
			break;
		case XK_Right:
			break;
		case XK_Up:
			break;
		case XK_Down:
			break;
		case XK_equal:
			gl.delay -= 0.005;
			if (gl.delay < 0.005)
				gl.delay = 0.005;
			break;
		case XK_minus:
			gl.delay += 0.005;
			break;
		case XK_Escape:
			gl.done=1;
			break;
	}
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void physics(void)
{
	if (gl.walk || gl.keys[XK_Right] || gl.keys[XK_Left]) {
		//man is walking...
		//when time is up, advance the frame.
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
		if (timeSpan > gl.delay) {
			//advance
			++gl.walkFrame;
			if (gl.walkFrame >= 16)
				gl.walkFrame -= 16;
			timers.recordTime(&timers.walkTime);
		}
		for (int i=0; i<20; i++) {
			if (gl.keys[XK_Left]) {
				gl.box[i][0] += 1.0 * (0.05 / gl.delay);
				if (gl.box[i][0] > gl.xres + 10.0)
					gl.box[i][0] -= gl.xres + 10.0;
				gl.camera[0] -= 2.0/lev.tilesize[0] * (0.05 / gl.delay);
				if (gl.camera[0] < 0.0)
					gl.camera[0] = 0.0;
			} else {
				gl.box[i][0] -= 1.0 * (0.05 / gl.delay);
				if (gl.box[i][0] < -10.0)
					gl.box[i][0] += gl.xres + 10.0;
				gl.camera[0] += 2.0/lev.tilesize[0] * (0.05 / gl.delay);
				if (gl.camera[0] < 0.0)
					gl.camera[0] = 0.0;
			}
		}
		if (gl.exp.onoff) {
			gl.exp.pos[0] -= 2.0 * (0.05 / gl.delay);
		}
		if (gl.exp44.onoff) {
			gl.exp44.pos[0] -= 2.0 * (0.05 / gl.delay);
		}
	}
	if (gl.exp.onoff) {
		//explosion is happening
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&gl.exp.time, &timers.timeCurrent);
		if (timeSpan > gl.exp.delay) {
			//advance explosion frame
			++gl.exp.frame;
			if (gl.exp.frame >= 23) {
				//explosion is done.
				gl.exp.onoff = 0;
				gl.exp.frame = 0;
			} else {
				timers.recordTime(&gl.exp.time);
			}
		}
	}
	if (gl.exp44.onoff) {
		//explosion is happening
		timers.recordTime(&timers.timeCurrent);
		double timeSpan = timers.timeDiff(&gl.exp44.time, &timers.timeCurrent);
		if (timeSpan > gl.exp44.delay) {
			//advance explosion frame
			++gl.exp44.frame;
			if (gl.exp44.frame >= 16) {
				//explosion is done.
				gl.exp44.onoff = 0;
				gl.exp44.frame = 0;
			} else {
				timers.recordTime(&gl.exp44.time);
			}
		}
	}
	//move the ball
	gl.ball_pos[1] += gl.ball_vel[1];
	gl.ball_vel[1] -= 0.7;
	Flt dd = lev.ftsz[0];
	int col = (int)(gl.camera[0] / dd) + (500.0 / lev.tilesize[0] + 1);
	col = col % lev.ncols;
	int hgt = 0;
	// linear search, fix to only search once
	if (lev.dynamicHeight[col] != -1) {
	    //set hgt to array value
	    hgt = lev.dynamicHeight[col];
	} else {
	    for (int i = 0; i < lev.nrows; i++) {
	        if (lev.arr[i][col] != ' ') {
		    hgt = i;
		    break;
		}
	    }
	    printf("col saved: %i\n", col);
	    // save height value in array
	    lev.dynamicHeight[col] = hgt;	    
	}
	//height of ball is (nrows-1-i)*tile_height + starting point
	Flt h= lev.tilesize[1]*(lev.nrows - hgt) + lev.tile_base;
	if (gl.ball_pos[1] <= h) {
	    gl.ball_vel[1] = 0.0;
	    gl.ball_pos[1] = h;
	}
}

void render(void)
{
	Rect r;
	//Clear the screen
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	float cx = gl.xres/2.0;
	float cy = gl.yres/2.0;
	//
	//show ground
	glBegin(GL_QUADS);
		glColor3f(0.2, 0.2, 0.2);
		glVertex2i(0,       220);
		glVertex2i(gl.xres, 220);
		glColor3f(0.4, 0.4, 0.4);
		glVertex2i(gl.xres,   0);
		glVertex2i(0,         0);
	glEnd();
	//
	//show boxes as background
	for (int i=0; i<20; i++) {
		glPushMatrix();
		glTranslated(gl.box[i][0],gl.box[i][1],gl.box[i][2]);
		glColor3f(0.2, 0.2, 0.2);
		glBegin(GL_QUADS);
			glVertex2i( 0,  0);
			glVertex2i( 0, 30);
			glVertex2i(20, 30);
			glVertex2i(20,  0);
		glEnd();
		glPopMatrix();
	}
	//
	//----------------------------
	//Render the level tile system
	//----------------------------
	Flt dd = lev.ftsz[0];
	Flt offy = lev.tile_base;
	int ncols_to_render = gl.xres / lev.tilesize[0] + 2;
	int col = (int)(gl.camera[0] / dd);
	col = col % lev.ncols;
	//Partial tile offset must be determined here.
	//The leftmost tile might be partially off-screen.
	//cdd: camera position in terms of tiles.
	Flt cdd = gl.camera[0] / dd;
	//flo: just the integer portion
	Flt flo = floor(cdd);
	//dec: just the decimal portion
	Flt dec = (cdd - flo);
	//offx: the offset to the left of the screen to start drawing tiles
	Flt offx = -dec * dd;
	//Log("gl.camera[0]: %lf   offx: %lf\n",gl.camera[0],offx);
	for (int j=0; j<ncols_to_render; j++) {
		int row = lev.nrows-1;
		for (int i=0; i<lev.nrows; i++) {
			if (lev.arr[row][col] == 'w') {
				glColor3f(0.8, 0.8, 0.6);
				glPushMatrix();
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr[0],tr[1],tr[2]);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				glBegin(GL_QUADS);
					glVertex2i( 0,  0);
					glVertex2i( 0, ty);
					glVertex2i(tx, ty);
					glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
			}
			if (lev.arr[row][col] == 'b') {
				glColor3f(0.9, 0.2, 0.2);
				glPushMatrix();
				Vec tr = { (Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0 };
				glTranslated(tr[0],tr[1],tr[2]);
				int tx = lev.tilesize[0];
				int ty = lev.tilesize[1];
				glBegin(GL_QUADS);
					glVertex2i( 0,  0);
					glVertex2i( 0, ty);
					glVertex2i(tx, ty);
					glVertex2i(tx,  0);
				glEnd();
				glPopMatrix();
			}
			--row;
		}
		++col;
		col = col % lev.ncols;
	}
	//draw ball
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(gl.ball_pos[0], gl.ball_pos[1], 0);
	glBegin(GL_QUADS);
		glVertex2i( 0,  0);
		glVertex2i( 0, 10);
		glVertex2i(10, 10);
		glVertex2i(10,  0);
	glEnd();
	glPopMatrix();
	//
	//fake shadow
	//glColor3f(0.25, 0.25, 0.25);
	//glBegin(GL_QUADS);
	//	glVertex2i(cx-60, 150);
	//	glVertex2i(cx+50, 150);
	//	glVertex2i(cx+50, 130);
	//	glVertex2i(cx-60, 130);
	//glEnd();
	//
	//
	float h = 200.0;
	float w = h * 0.5;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, gl.walkTexture);
	//
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	int ix = gl.walkFrame % 8;
	int iy = 0;
	if (gl.walkFrame >= 8)
		iy = 1;
	float tx = (float)ix / 8.0;
	float ty = (float)iy / 2.0;
	int rgt = 1;
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
	//
	//
	if (gl.exp.onoff) {
		h = 80.0;
		w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, gl.exp.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glTranslated(gl.exp.pos[0], gl.exp.pos[1], gl.exp.pos[2]);
		int ix = gl.exp.frame % 5;
		int iy = gl.exp.frame / 5;
		float tx = (float)ix / 5.0;
		float ty = (float)iy / 5.0;
		glBegin(GL_QUADS);
			glTexCoord2f(tx,     ty+0.2); glVertex2i(cx-w, cy-h);
			glTexCoord2f(tx,     ty);     glVertex2i(cx-w, cy+h);
			glTexCoord2f(tx+0.2, ty);     glVertex2i(cx+w, cy+h);
			glTexCoord2f(tx+0.2, ty+0.2); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
	//
	//
	if (gl.exp44.onoff) {
		h = 80.0;
		w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, gl.exp44.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		glTranslated(gl.exp44.pos[0], gl.exp44.pos[1], gl.exp44.pos[2]);
		int ix = gl.exp44.frame % 4;
		int iy = gl.exp44.frame / 4;
		float tx = (float)ix / 4.0;
		float ty = (float)iy / 4.0;
		glBegin(GL_QUADS);
			glTexCoord2f(tx,      ty+0.25); glVertex2i(cx-w, cy-h);
			glTexCoord2f(tx,      ty);      glVertex2i(cx-w, cy+h);
			glTexCoord2f(tx+0.25, ty);      glVertex2i(cx+w, cy+h);
			glTexCoord2f(tx+0.25, ty+0.25); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
	}
	unsigned int c = 0x00ffff44;
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, c, "W   Walk cycle");
	ggprint8b(&r, 16, c, "E   Explosion");
	ggprint8b(&r, 16, c, "+   faster");
	ggprint8b(&r, 16, c, "-   slower");
	ggprint8b(&r, 16, c, "right arrow -> walk right");
	ggprint8b(&r, 16, c, "left arrow  <- walk left");
	ggprint8b(&r, 16, c, "frame: %i", gl.walkFrame);
	if (gl.movie) {
		screenCapture();
	}
}







