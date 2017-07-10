#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <string>
#include <vector>

#include "log.h"
#include "ppm.h"
#include "fonts.h"


//macros
#define MAX_BARS 3

typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]

//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1


//Time helper functions

inline double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double) (end->tv_sec - start->tv_sec) +
            (double) (end->tv_nsec - start->tv_nsec) * 1.0e-9;
}

inline void timeCopy(struct timespec *dest, struct timespec *source)
{
    memcpy(dest, source, sizeof (struct timespec));
}

inline void recordTime(struct timespec *t)
{
    clock_gettime(CLOCK_REALTIME, t);
}

/**
 * Sprite animation class
 */
class Sprite {
    GLuint glTexture;
    int frameCount;
    int rows;
    int cols;
    int origHeight;
    int origWidth;
    float height;
    float width;
    int currentFrame;
    float posX;
    float posY;
    double delay;
    struct timespec time;
public:
    Sprite(
            const std::string & filename,
            int frameCount,
            int rows,
            int cols,
            double delay,
            float height,
            float width
            );

    ~Sprite();

    void setPos(float x, float y);
    void setSize(float height, float width);
    float getHeight();
    float getWidth();
    double getDelay();
    float getPosY();
    float getPosX();
    void draw();
};

/*
 * This creates a class called GlobalSprite, has one variable of type Sprite*,
 * named characterGirl. It also at the same time makes it accessible globally 
 *  by the name of globalSprite
 */
struct GlobalSprite {
    Sprite* characterGirl;
};

extern GlobalSprite globalSprite;

class Game {
public:
    int n;

    Game()
    {
        n = 0;
    }
};

class Battery {
    public:
        int arr[MAX_BARS];
        int points;
	int bcount; //check how many bars;

        Battery() {
            arr[MAX_BARS] = 540;
	    bcount = 0;
        }
        void battbarAppear();
        void drawBattery(void);
        void drawFlashlight();
        void grabBattery();
        bool deleteBattery();
	void gameOver();
};


/**
 * Menu classes
 */
// class for sub menu items
class MenuItem {
public:
    MenuItem(std::string txt, int x, int y, int w, int h);
    void draw(); 

private:
    std::string text;

    int posX;
    int posY;

    int width;
    int height;

};

class Menu {
public:
    Menu();

    virtual ~Menu();

    void add(MenuItem item);

    void draw();


protected:
    //stores the menu items
    std::vector<MenuItem> menuItems;
};

class MainMenu : public Menu {
public:
    MainMenu();

    void keyboardInput(int key);
};
/**/

class Global {
public:
    Display *dpy;
    Window win;
    unsigned char keys[65536];
    int done;
    int xres, yres;
    //camera is centered at (0,0) lower-left of screen. 
    Flt camera[2];
    Vec ball_pos;
    Vec ball_vel;
    Battery batt;
	int keyCount;
    int walkFrame;
    int walk;
    int shock;
    int shockFrame;
    Vec box[20];
    double delay;
    double shockDelay;
    GLuint walkTexture;
    //Gluint shockTexture;
    Ppmimage *shockImage;
    MainMenu mainMenu;
    bool mainMenuOpen;
    struct timespec timeCurrent;
    struct timespec shockTime;

    ~Global()
    {
        logClose();
    }

    Global()
    {
        logOpen();
        camera[0] = camera[1] = 0.0;
        done = 0;
        xres = 800;
        yres = 600;
		keyCount = 0;
        shock = 0;
        shockFrame = 0;
        shockImage = NULL;
        shockDelay = 0.1; // for now
        memset(keys, 0, 65536);
        mainMenuOpen = true;
    }
};

extern Global gl;

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
void initCharacterSprites();
unsigned char *buildAlphaData(Ppmimage *img);

//All function and class prototypes go here

//function prototypes
void renderBackground(void); //render prototype
void applyBackgroundMovement(void);
void initCharacterSprites(); // for Sprite characters
void renderCharacterSprites(); // for Sprite characters
void physicsCharacterSprites(); //Temporary test function for moving sprites
void drawLight(void);
void LightCollision();
void displayColors(); //different colors for background
void FlashlightPower(); //Displays how much power battery has
void drawFlashlightPower(float); //displays the bar of battery life
//void menuBackground(); //menu background




#endif /* GAME_H */
