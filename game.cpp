
#include "game.h"

int main(void)
{
    srand(time(NULL));
    initXWindows();
    initOpengl();
    setupAudio();
    setupAudio();
    initMenuBackground(); //main menu background
    initCharacterSprites(); // function call inside initOpenGl
    initBackgroundSprites();
    initLightSprite();
    initCreditBackground();
    initLifeBarSprite();
    initHighScores();
    initRavenSprite();
    initEnemySprites();
    initGameOverSprite();
    initSpells();
    init();
    struct timespec now, last;
    recordTime(&last);

    const double physicsRate = 1.0f / 60.0f;
    while (!gl.done) {
        while (XPending(gl.dpy)) {
            XEvent e;
            XNextEvent(gl.dpy, &e);
            checkResize(&e);
            checkMouse(&e);
            checkKeys(&e);
        }

        //Run physics at a fixed interval (60 times per second)
        recordTime(&now);
        double physicsCountDown = timeDiff(&last, &now);
        timeCopy(&last, &now);
        while (physicsCountDown >= physicsRate) {
            physics();
            physicsAudio();
            physicsCountDown -= physicsRate;
        }
        render();
        glXSwapBuffers(gl.dpy, gl.win);
    }
    cleanupXWindows();
    cleanup_fonts();
    cleanupAudio();
    return 0;
}

Global gl;
GlobalSprite globalSprite;

void cleanupXWindows(void)
{
    XDestroyWindow(gl.dpy, gl.win);
    XCloseDisplay(gl.dpy);
}
//hello

void setTitle(void)
{
    //Set the window title bar.
    XMapWindow(gl.dpy, gl.win);
    XStoreName(gl.dpy, gl.win, "CMPS 3350 - Project: Beyond The Light");
}

void setupScreenRes(const int w, const int h)
{
    gl.xres = w;
    gl.yres = h;
}

void initXWindows(void)
{
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
    XSetWindowAttributes swa;
    setupScreenRes(gl.xres, gl.yres);
    gl.dpy = XOpenDisplay(NULL);
    if (gl.dpy == NULL) {
        printf("\n\tcannot connect to X server\n\n");
        exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(gl.dpy);
    XVisualInfo *vi = glXChooseVisual(gl.dpy, 0, att);
    if (vi == NULL) {
        printf("\n\tno appropriate visual found\n\n");
        exit(EXIT_FAILURE);
    }
    Colormap cmap = XCreateColormap(gl.dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
            StructureNotifyMask | SubstructureNotifyMask;
    gl.win = XCreateWindow(gl.dpy, root, 0, 0, gl.xres, gl.yres, 0,
            vi->depth, InputOutput, vi->visual,
            CWColormap | CWEventMask, &swa);
    GLXContext glc = glXCreateContext(gl.dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(gl.dpy, gl.win, glc);
    setTitle();
}

void reshapeWindow(int width, int height)
{
    //window has been resized.
    setupScreenRes(width, height);
    //
    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    setTitle();
}

void initOpengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
        gl.mainMenu.resize(gl.xres, xce.width, gl.yres, xce.height);
        reshapeWindow(xce.width, xce.height);
        gl.xres = xce.width;
        gl.yres = xce.height;
    }
}

void init()
{

}

void restart()
{
    //add your own function if you have to reset anything
    //when the game is over
    //example:
    //restartBattery();
    cheyRestart();
    auroraRestart();
    karenRestart();
    printf("Restarted the game!\n");
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
        if (e->xbutton.button == 1) {
            //Left button is down
        }
        if (e->xbutton.button == 3) {
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
    static int shift = 0;
    int key = XLookupKeysym(&e->xkey, 0);
    key = key & 0x0000ffff; // fixes segfault
    gl.keys[key] = 1;
    if (e->type == KeyRelease) {
        gl.keys[key] = 0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift = 0;
        return;
    }
    if (e->type == KeyPress) {
        gl.keys[key] = 1;
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift = 1;
            return;
        }
    } else {
        return;
    }

    if (e->type == KeyPress) {
        checkUserNameInput(key);
        //Always checked because it opens the menu
        //it checks if main menu is open
        gl.mainMenu.keyboardInput(key);
        //renderCreditSprite.keyboardInput(key);

        monitorCTRLC(key);
        for (int i = 0; i < 3; i++) {
            gl.spells[i]->input(key);
        }
    }

    if (shift) {
    }
    switch (key) {
        break;
    case XK_space:
        if (gl.keepTrack == 10) {
            gl.spacebar = 1;
        }
        if (gl.state == STATE_HIGHSCORE) {
            gl.spacebar = 2;
        }
        break;
    case XK_h:
        if (gl.state == STATE_GAMEPLAY) {
            if (gl.keepTrack >= 0 && gl.hardSelection == 0) {
                gl.hardSelection = 1;
            } else if (gl.hardSelection == 1) {
                gl.hardSelection = 0;
            }
        }
        break;
    case XK_Escape:
        break;
    case XK_j:
        if (gl.state == STATE_GAMEPLAY) {
            if (gl.keepTrack >= 0 && gl.hardSelection == 0) {
                gl.danceParty = 1;
            } else if (gl.hardSelection == 1) {
                gl.danceParty = 0;
            }
        }
        break;
    case XK_f:
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
        break;
    case XK_minus:
        break;

    }
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen * xlen + ylen * ylen + zlen*zlen;
    if (len == 0.0) {
        MakeVector(0.0, 0.0, 1.0, vec);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;

    return (len);
}

void physics(void)
{
    //Only run physics when not in main menu
    //if(!gl.mainMenuOpen) {
    //   physicsCharacterSprites();
    if (gl.state == STATE_GAMEPLAY) {
        physicsCharacterSprites();
        physicsGhosts();
        gl.camera[0] = -globalSprite.mortana->getPosX() + gl.xres / 2;
        physicsLightSprite();
        //When game is not paused, sprite physics must be updated
        for (int i = 0; i < 5; i++) {
            // globalSprite.background[i]->physics();
        }
        globalSprite.gameover->physics();
        for (int i = 0; i < 10; i++) {

            globalSprite.life[i]->physics();
        }
        globalSprite.light[4]->physics();
        ghostRandom();
        mortanaCollision();
        for (int i = 0; i < 3; i++) {
            gl.spells[i]->physics();
        }
        physicsRaven();
    }
}

void render(void)
{
    //Clear the screen
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    // Menu and menu item must open first then the rest will be rendered
    if (gl.state == STATE_GAMEPLAY) {
        glPushMatrix();
        // renderTutorial();
        glTranslatef(gl.camera[0], 0, 0);
        renderBackgroundSprites();
        renderTutorial();
        gl.batt.energybarAppears();
        renderLifeBarSprite();
        renderCharacterSprites();
        renderRavenSprites();
        renderEnemySprites();
        hardMode();
        disco();
        renderText();
        renderLightSprite();
        redScreenFlash();
        for (int i = 0; i < 3; i++) {
            gl.spells[i]->render();
        }
        glPopMatrix();
    } else if (gl.state == STATE_STARTUP || gl.state == STATE_GAMEPAUSE) {
        renderMenuBackground();
        gl.mainMenu.draw();
    } else if (gl.state == STATE_CREDITS) {
        renderCreditBackground();
    } else if (gl.state == STATE_HIGHSCORE) {
        renderHighScores();
    } else if (gl.state == STATE_GAMEOVER) {
        gl.batt.gameOver();
        renderGameOverSprite();
        renderUsernameInput();
    }

    if (gl.keys['y'] && gl.keys['b']) {
        glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 0.0, 0.3);
        glBegin(GL_QUAD_STRIP);
        glVertex2i(0, 0);
        glVertex2i(100, 100);
        glVertex2i(gl.xres, 0);
        glVertex2i(gl.xres - 100, 100);
        glVertex2i(gl.xres, gl.yres);
        glVertex2i(gl.xres - 100, gl.yres - 100);
        glVertex2i(0, gl.yres);
        glVertex2i(100, gl.yres - 100);
        glVertex2i(0, 0);
        glVertex2i(100, 100);
        glEnd();
        glDisable(GL_BLEND);
        glPopMatrix();


    }

}

