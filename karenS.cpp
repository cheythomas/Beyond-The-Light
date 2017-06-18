//Modified by: Karen Salinas
//Date: 6/18/2017

//This program is from rainforest frameworks, but this is a head 
//to creating the framework for BeyondTheLight
//most of the prototypes have been changed to the names
//we will be using for the project.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
//
//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt Matrix[4][4];

//macros
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
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double)(end->tv_sec - start->tv_sec ) +
            (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source)
{
    memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------

class Global {
public:
    int done;
    int xres, yres;
    Global() {
        logOpen();
        done=0;
        xres=800;
        yres=600;
    }
    ~Global() {
        logClose();
    }
} gl;

Ppmimage *personImage=NULL;
Ppmimage *hauntedhouseImage=NULL;
Ppmimage *hauntedhouseTransImage=NULL;
Ppmimage *flashlightImage=NULL;
GLuint personTexture;
GLuint silhouetteTexture;
GLuint hauntedhouseTexture;
GLuint hauntedhouseTransTexture;
GLuint flashlightTexture;
int showPerson=0;
int hauntedhouse=1;
int silhouette=1;
int trees=1;
int showLight=0;
//
class Person {
public:
    Vec pos;
    Vec vel;
} person;

class Lightdrop {
public:
    int type;
    int linewidth;
    int sound;
    Vec pos;
    Vec lastpos;
    Vec vel;
    Vec maxvel;
    Vec force;
    float length;
    float color[4];
    Lightdrop *prev;
    Lightdrop *next;
} *lighthead = NULL;
int nldrops=1;
int totlight=0;
int maxlight=0;
void deleteLight(Lightdrop *node);
void cleanupLightdrops(void);
//
#define LIGHT_FLAT  0
#define LIGHT_ROUND 1
class Flashlight {
public:
    int shape;
    Vec pos;
    Vec lastpos;
    float width;
    float width2;
    float radius;
} flashlight;
int showFlashlight=0;
int deflection=0;


int main(void)
{
    initXWindows();
    initOpengl();
    init();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    while (!gl.done) {
        while (XPending(dpy)) {
            XEvent e;
            XNextEvent(dpy, &e);
            checkResize(&e);
            checkMouse(&e);
            checkKeys(&e);
        }
        //
        //Below is a process to apply physics at a consistent rate.
        //1. Get the current time.
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        //2. How long since we were here last?
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        //3. Save the current time as our new starting time.
        timeCopy(&timeStart, &timeCurrent);
        //4. Add time-span to our countdown amount.
        physicsCountdown += timeSpan;
        //5. Has countdown gone beyond our physics rate? 
        //       if yes,
        //           In a loop...
        //              Apply physics
        //              Reducing countdown by physics-rate.
        //              Break when countdown < physics-rate.
        //       if no,
        //           Apply no physics this frame.
        while (physicsCountdown >= physicsRate) {
            //6. Apply physics
            physics();
            //7. Reduce the countdown by our physics-rate
            physicsCountdown -= physicsRate;
        }
        //Always render every frame.
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
    XStoreName(dpy, win, "3350 - Animation Template");
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
    setupScreenRes(640, 480);
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
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //-----------------------------------------------
        //get largest color component...
        //*(ptr+3) = (unsigned char)((
        //      (int)*(ptr+0) +
        //      (int)*(ptr+1) +
        //      (int)*(ptr+2)) / 3);
        //d = a;
        //if (b >= a && b >= c) d = b;
        //if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //-----------------------------------------------
        //this code optimizes the commented code above.
        *(ptr+3) = (a|b|c);
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
    personImage     = ppm6GetImage("./images/person.ppm");
    hauntedhouseImage      = ppm6GetImage("./images/hauntedhouse.ppm");
    hauntedhouseTransImage = ppm6GetImage("./images/hauntedhouseTrans.ppm");
    flashlightImage    = ppm6GetImage("./images/flashlight.ppm");
    //
    //create opengl texture elements
    glGenTextures(1, &personTexture);
    glGenTextures(1, &silhouetteTexture);
    glGenTextures(1, &hauntedhouseTexture);
    glGenTextures(1, &flashlightTexture);
    //-------------------------------------------------------------------------
    //person
    //
    int w = personImage->width;
    int h = personImage->height;
    //
    glBindTexture(GL_TEXTURE_2D, personTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
                            GL_RGB, GL_UNSIGNED_BYTE, personImage->data);
    //-------------------------------------------------------------------------
    //
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *silhouetteData = buildAlphaData(personImage);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    free(silhouetteData);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
    //  GL_RGB, GL_UNSIGNED_BYTE, personImage->data);
    //-------------------------------------------------------------------------
    //
    //flashlight
    //
    glBindTexture(GL_TEXTURE_2D, flashlightTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    silhouetteData = buildAlphaData(flashlightImage); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    free(silhouetteData);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
    //  GL_RGB, GL_UNSIGNED_BYTE, personImage->data);
    //-------------------------------------------------------------------------
    //
    //hauntedhouse
    glBindTexture(GL_TEXTURE_2D, hauntedhouseTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                            hauntedhouseImage->width, hauntedhouseImage->height,
                            0, GL_RGB, GL_UNSIGNED_BYTE, hauntedhouseImage->data);
    //-------------------------------------------------------------------------
    //
    //hauntedhouse transparent part
    //
    glBindTexture(GL_TEXTURE_2D, hauntedhouseTransTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    w = hauntedhouseTransImage->width;
    h = hauntedhouseTransImage->height;
    unsigned char *ftData = buildAlphaData(hauntedhouseTransImage);   
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, ftData);
    free(ftData);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
    //GL_RGB, GL_UNSIGNED_BYTE, personImage->data);
    //-------------------------------------------------------------------------
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

void initSounds(void)
{

}

void init() {
    flashlight.pos[0] = 220.0;
    flashlight.pos[1] = (double)(gl.yres-200);
    VecCopy(flashlight.pos, flashlight.lastpos);
    flashlight.width = 200.0;
    flashlight.width2 = flashlight.width * 0.5;
    flashlight.radius = (float)flashlight.width2;
    flashlight.shape = LIGHT_FLAT;
    MakeVector(-150.0,180.0,0.0, person.pos);
    MakeVector(6.0,0.0,0.0, person.vel);
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

void checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyRelease) {
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return;
    }
    if (e->type == KeyPress) {
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift=1;
            return;
        }
    } else {
        return;
    }
    switch (key) {
        case XK_b:
            showPerson ^= 1;
            if (showPerson) {
                person.pos[0] = -250.0;
            }
            break;
        case XK_d:
            deflection ^= 1;
            break;
        case XK_f:
            hauntedhouse ^= 1;
            break;
        case XK_s:
            silhouette ^= 1;
            printf("silhouette: %i\n",silhouette);
            break;
        case XK_t:
            trees ^= 1;
            break;
        case XK_u:
            showFlashlight ^= 1;
            break;
        case XK_p:
            flashlight.shape ^= 1;
            break;
        case XK_r:
            showLight ^= 1;
            break;
        case XK_Left:
            VecCopy(flashlight.pos, flashlight.lastpos);
            flashlight.pos[0] -= 10.0;
            break;
        case XK_Right:
            VecCopy(flashlight.pos, flashlight.lastpos);
            flashlight.pos[0] += 10.0;
            break;
        case XK_Up:
            VecCopy(flashlight.pos, flashlight.lastpos);
            flashlight.pos[1] += 10.0;
            break;
        case XK_Down:
            VecCopy(flashlight.pos, flashlight.lastpos);
            flashlight.pos[1] -= 10.0;
            break;
        case XK_equal:
            if (++nldrops > 40)
                nldrops=40;
            break;
        case XK_minus:
            if (--nldrops < 0)
                nldrops = 0;
            break;
        case XK_n:
            break;
        case XK_w:
            if (shift) {
                //shrink the flashlight
                flashlight.width *= (1.0 / 1.05);
            } else {
                //enlarge the flashlight
                flashlight.width *= 1.05;
            }
            //half the width
            flashlight.width2 = flashlight.width * 0.5;
            flashlight.radius = (float)flashlight.width2;
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

void cleanupLightdrops(void)
{
    Lightdrop *s;
    while (lighthead) {
        s = lighthead->next;
        free(lighthead);
        lighthead = s;
    }
    lighthead=NULL;
}

void deleteLight(Lightdrop *node)
{
    //remove a node from linked list
    //Log("deleteLight()...\n");
    if (node->prev == NULL) {
        if (node->next == NULL) {
            //Log("only 1 item in list.\n");
            lighthead = NULL;
        } else {
            //Log("at beginning of list.\n");
            node->next->prev = NULL;
            lighthead = node->next;
        }
    } else {
        if (node->next == NULL) {
            //Log("at end of list.\n");
            node->prev->next = NULL;
        } else {
            //Log("in middle of list.\n");
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }
    free(node);
    node = NULL;
}

void movePerson()
{
    //move person...
    int addgrav = 1;
    //Update position
    person.pos[0] += person.vel[0];
    person.pos[1] += person.vel[1];
    //Check for collision with window edges
    if ((person.pos[0] < -140.0 && person.vel[0] < 0.0) ||
        (person.pos[0] >= (float)gl.xres+140.0 &&
        person.vel[0] > 0.0))
    {
        person.vel[0] = -person.vel[0];
        addgrav = 0;
    }
    if ((person.pos[1] < 150.0 && person.vel[1] < 0.0) ||
        (person.pos[1] >= (float)gl.yres && person.vel[1] > 0.0)) {
        person.vel[1] = -person.vel[1];
        addgrav = 0;
    }
    //Gravity?
    if (addgrav)
        person.vel[1] -= 0.75;
}


void createLightdrop(const int n)
{
    //create new light drops...
    int i;
    for (i=0; i<n; i++) {
        Lightdrop *node = (Lightdrop *)malloc(sizeof(Lightdrop));
        if (node == NULL) {
            Log("error allocating node.\n");
            exit(EXIT_FAILURE);
        }
        node->prev = NULL;
        node->next = NULL;
        node->sound=0;
        node->pos[0] = rnd() * (float)gl.xres;
        node->pos[1] = rnd() * 100.0f + (float)gl.yres;
        VecCopy(node->pos, node->lastpos);
        node->vel[0] = 
        node->vel[1] = 0.0f;
        node->color[0] = rnd() * 0.2f + 0.8f;
        node->color[1] = rnd() * 0.2f + 0.8f;
        node->color[2] = rnd() * 0.2f + 0.8f;
        node->color[3] = rnd() * 0.5f + 0.3f; //alpha
        node->linewidth = random(8)+1;
        //larger linewidth = faster speed
        node->maxvel[1] = (float)(node->linewidth*16);
        node->length = node->maxvel[1] * 0.2f + rnd();
        //put light into linked list
        node->next = lighthead;
        if (lighthead != NULL)
            lighthead->prev = node;
        lighthead = node;
        ++totlight;
    }
}

void checkLightdrops()
{
    if (random(100) < 50) {
        createLightdrop(nldrops);
    }
    //
    //move light droplets
    Lightdrop *node = lighthead;
    while (node) {
        //force is toward the ground
        node->vel[1] += gravity;
        VecCopy(node->pos, node->lastpos);
        node->pos[0] += node->vel[0] * timeslice;
        node->pos[1] += node->vel[1] * timeslice;
        if (fabs(node->vel[1]) > node->maxvel[1])
            node->vel[1] *= 0.96;
        node->vel[0] *= 0.999;
        node = node->next;
    }
    //}
    //
    //check light droplets
    int n=0;
    node = lighthead;
    while (node) {
        n++;
        #ifdef USE_SOUND
        if (node->pos[1] < 0.0f) {
            //raindrop hit ground
            if (!node->sound && play_sounds) {
                //small chance that a sound will play
                int r = random(50);
                if (r==1) {
                    //play sound here...


                }
                //sound plays once per lightdrop
                node->sound=1;
            }
        }
        #endif //USE_SOUND
        //collision detection for lightdrop on flashlight
        if (showFlashlight) {
            if (flashlight.shape == LIGHT_FLAT) {
                if (node->pos[0] >= (flashlight.pos[0] - flashlight.width2) &&
                    node->pos[0] <= (flashlight.pos[0] + flashlight.width2)) {
                    if (node->lastpos[1] > flashlight.lastpos[1] ||
                        node->lastpos[1] > flashlight.pos[1]) {
                        if (node->pos[1] <= flashlight.pos[1] ||
                            node->pos[1] <= flashlight.lastpos[1]) {
                            if (node->linewidth > 1) {
                                Lightdrop *savenode = node->next;
                                deleteLight(node);
                                node = savenode;
                                continue;
                            }
                        }
                    }
                }
            }
            if (flashlight.shape == LIGHT_ROUND) {
                float d0 = node->pos[0] - flashlight.pos[0];
                float d1 = node->pos[1] - flashlight.pos[1];
                float distance = sqrt((d0*d0)+(d1*d1));
                //Log("distance: %f  flashlight.radius: %f\n",
                //                          distance,flashlight.radius);
                if (distance <= flashlight.radius &&
                                        node->pos[1] > flashlight.pos[1]) {
                    if (node->linewidth > 1) {
                        if (deflection) {
                            //deflect lightdrop
                            double dot;
                            Vec v, up = {0,1,0};
                            VecSub(node->pos, flashlight.pos, v);
                            VecNormalize(v);
                            node->pos[0] =
                                flashlight.pos[0] + v[0] * flashlight.radius;
                            node->pos[1] =
                                flashlight.pos[1] + v[1] * flashlight.radius;
                            dot = VecDot(v,up);
                            dot += 1.0;
                            node->vel[0] += v[0] * dot * 1.0;
                            node->vel[1] += v[1] * dot * 1.0;
                        } else {
                            Lightdrop *savenode = node->next;
                            deleteLight(node);
                            node = savenode;
                            continue;
                        }
                    }
                }
            }
        }
        if (node->pos[1] < -20.0f) {
            //light drop is below the visible area
            Lightdrop *savenode = node->next;
            deleteLight(node);
            node = savenode;
            continue;
        }
        node = node->next;
    }
    if (maxlight < n)
        maxlight = n;
}

void physics(void)
{
    if (showPerson)
        movePerson();
    if (showLight)
        checkLightdrops();
}

void drawFlashlight(void)
{
    //Log("drawFlashlight()...\n");
    if (flashlight.shape == LIGHT_FLAT) {
        glColor4f(1.0f, 0.2f, 0.2f, 0.5f);
        glLineWidth(8);
        glBegin(GL_LINES);
            glVertex2f(flashlight.pos[0]-flashlight.width2, flashlight.pos[1]);
            glVertex2f(flashlight.pos[0]+flashlight.width2, flashlight.pos[1]);
        glEnd();
        glLineWidth(1);
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
        glPushMatrix();
        glTranslatef(flashlight.pos[0],flashlight.pos[1],flashlight.pos[2]);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBindTexture(GL_TEXTURE_2D, flashlightTexture);
        glBegin(GL_QUADS);
            float w = flashlight.width2;
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
            glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
            glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_ALPHA_TEST);
        glPopMatrix();
    }
}

void drawLightdrops(void)
{
    Lightdrop *node = lighthead;
    while (node) {
        glPushMatrix();
        glTranslated(node->pos[0],node->pos[1],node->pos[2]);
        glColor4fv(node->color);
        glLineWidth(node->linewidth);
        glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(0.0f, node->length);
        glEnd();
        glPopMatrix();
        node = node->next;
    }
    glLineWidth(1);
}

void render(void)
{
    Rect r;

    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //
    //draw a quad with texture
    float wid = 120.0f;
    glColor3f(1.0, 1.0, 1.0);
    if (hauntedhouse) {
        glBindTexture(GL_TEXTURE_2D, hauntedhouseTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
            glTexCoord2f(0.0f, 0.0f); glVertex2i(0, gl.yres);
            glTexCoord2f(1.0f, 0.0f); glVertex2i(gl.xres, gl.yres);
            glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres, 0);
        glEnd();
    }
    if (showPerson) {
        glPushMatrix();
        glTranslatef(person.pos[0], person.pos[1], person.pos[2]);
        if (!silhouette) {
            glBindTexture(GL_TEXTURE_2D, personTexture);
        } else {
            glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
        }
        glBegin(GL_QUADS);
            if (person.vel[0] > 0.0) {
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
            } else {
                glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
                glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
            }
        glEnd();
        glPopMatrix();
        //
        if (trees && silhouette) {
            glBindTexture(GL_TEXTURE_2D, hauntedhouseTransTexture);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(0, gl.yres);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(gl.xres, gl.yres);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(gl.xres, 0);
            glEnd();
        }
        glDisable(GL_ALPHA_TEST);
    }

    glDisable(GL_TEXTURE_2D);
    //glColor3f(1.0f, 0.0f, 0.0f);
    //glBegin(GL_QUADS);
    //  glVertex2i(10,10);
    //  glVertex2i(10,60);
    //  glVertex2i(60,60);
    //  glVertex2i(60,10);
    //glEnd();
    //return;
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    if (showLight)
        drawLightdrops();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    //
    if (showFlashlight)
        drawFlashlight();
    glBindTexture(GL_TEXTURE_2D, 0);
    //
    //
    unsigned int c = 0x00ffff44;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, c, "B - Person");
    ggprint8b(&r, 16, c, "F - Forest");
    ggprint8b(&r, 16, c, "S - Silhouette");
    ggprint8b(&r, 16, c, "T - Trees");
    ggprint8b(&r, 16, c, "U - Flashlight");
    ggprint8b(&r, 16, c, "R - Light");
    ggprint8b(&r, 16, c, "D - Deflection");
    ggprint8b(&r, 16, c, "N - Sounds");
}
