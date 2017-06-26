// Author: Aurora Hernandez
// Date: 06/17/17
// Purpose: sprite animations

//Week 4:
//Added initialization, physics and render funcs for Sprite
//Added first sprite
//characterGirl credit: 
//http://hordana.deviantart.com/art/Anime-girl-Walk-486317319
//Tested main character sprite
/**/
//Week 5
// Start on main menu/start up menu
// Allow for collection of points
// Allow for new game
// Settings
// Exit game


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

#include <string>
#include <vector>
#include "game.h"

// Facilitating sprite animations for team members
//For simplifying sprite animations
// Extracted from walk.cpp

using namespace std;

unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int i;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *) img->data;
    newdata = (unsigned char *) malloc(img->width * img->height * 4);
    ptr = newdata;
    unsigned char a, b, c;
    //use the first pixel in the image as the transparent color.
    unsigned char t0 = *(data + 0);
    unsigned char t1 = *(data + 1);
    unsigned char t2 = *(data + 2);
    for (i = 0; i < img->width * img->height * 3; i += 3) {
        a = *(data + 0);
        b = *(data + 1);
        c = *(data + 2);
        *(ptr + 0) = a;
        *(ptr + 1) = b;
        *(ptr + 2) = c;
        *(ptr + 3) = 1;
        if (a == t0 && b == t1 && c == t2)
            *(ptr + 3) = 0;
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

Sprite::Sprite(
        const std::string & filename,
        int frameCountValue,
        int rowsValue,
        int colsValue,
        double delayValue,
        float h,
        float w
        ) : glTexture(),
frameCount(frameCountValue),
rows(rowsValue), cols(colsValue),
origHeight(), origWidth(),
height(h), width(w),
currentFrame(0),
posX(), posY(),
delay(delayValue),
time()
{
    // convert to ppm automatically
    string inputFile = "./images/" + filename;
    string outputFile = "./images/converted/" + filename + ".ppm";
    printf("Converting: %s to %s...\n", inputFile.c_str(),
            outputFile.c_str());
    system(("mkdir -p ./images/converted && convert  " + inputFile +
            " " + outputFile).c_str());

    //create openGL IMAGE
    //use build AlphaData to convert transparent
    //save result in class variables


    Ppmimage *ppmImage = ppm6GetImage(outputFile.c_str());
    origHeight = ppmImage->height;
    origWidth = ppmImage->width;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //must build a new set of data...
    unsigned char *imageData = buildAlphaData(ppmImage);
    // unsigned buildAlphaData(gl.explosiveImage);//***********
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, origWidth, origHeight, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, imageData
            );

    free(imageData);
    ppm6CleanupImage(ppmImage);
    unlink(outputFile.c_str());
    recordTime(&time);
}

Sprite::~Sprite()
{
}

void Sprite::draw()
{
    //calculate time elapsed, update frame
    //render appropriate sprite

    struct timespec current;
    recordTime(&current);

    double timeSpan = timeDiff(&time, &current);
    if (timeSpan > delay) {
        //next frame
        ++currentFrame;
        if (currentFrame >= frameCount)
            currentFrame = 0;
        recordTime(&time);
    }

    //Calculate the sprite frame and size
    //and location
    float cx = posX;
    float cy = posY;
    float h = height;
    float w = width;
    int ix = currentFrame % frameCount;
    int iy = currentFrame / cols;
    float tx = (float) ix / cols;
    float ty = (float) iy / rows;
    float tw = 1.0f / cols;
    float th = 1.0f / rows;

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    //
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);


    glBegin(GL_QUADS);
    glTexCoord2f(tx, ty + th);
    glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx, ty);
    glVertex2i(cx - w, cy + w);
    glTexCoord2f(tx + tw, ty);
    glVertex2i(cx + w, cy + h);
    glTexCoord2f(tx + tw, ty + th);
    glVertex2i(cx + w, cy - h);

    glEnd();
    glPopMatrix();
}

void Sprite::setPos(float x, float y)
{
    posX = x;
    posY = y;
}

void Sprite::setSize(float height, float width)
{
    this->height = height;
    this->width = width;
}

float Sprite::getWidth()
{
    return width;
}

float Sprite::getHeight()
{
    return height;
}

double Sprite::getDelay()
{
    return delay;
}

float Sprite::getPosX()
{
    return posX;
}

float Sprite::getPosY()
{
    return posY;
}

void initCharacterSprites()
{
    globalSprite.characterGirl = new Sprite
            ("girl1.gif", 11, 1, 11, 1.0f / 8.0f, 113, 128);
    globalSprite.characterGirl->setPos(gl.xres / 2, gl.yres / 2);
}

void renderCharacterSprites()
{
    globalSprite.characterGirl->draw();

}

void physicsCharacterSprites()
{
    static float pos = 0;
    Sprite* sp = globalSprite.characterGirl;
    if (pos > gl.xres + sp->getWidth()) {
        pos = -sp->getWidth();
    }
    sp->setPos(pos, gl.yres / 2);
    pos += 10;
}
//**
// Menu
//**

MenuItem::MenuItem(std::string txt, int x, int y, int w, int h)
: text(txt), posX(x), posY(y), width(w), height(h)
{

}
// draw out menu items

void MenuItem::draw()
{
    // rendering boxes 
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glRectf(
            posX,
            posY,
            posX + width,
            posY + height
            );
    // rendering text
    Rect r;
    r.center = 0;
    r.bot = posY + 15;
    r.left = posX + 50;
    // hex for color white
    ggprint16(&r, 0, 0xFF0000, this->text.c_str());


    glPopMatrix();
}

void Menu::draw()
{
    //draw any background

    //loop through menuitems...
    for (size_t i = 0; i < this->menuItems.size(); i++) {
        MenuItem& mi = this->menuItems[i];
        mi.draw();
    }
}

Menu::Menu() : menuItems()
{
    //empty constructor
}

Menu::~Menu()
{
    //empty virtual destructor
}

void Menu::add(MenuItem item)
{
    this->menuItems.push_back(item);
}

MainMenu::MainMenu()
{
    ////add all menu items    
    add(MenuItem("[P] lay", 300, 400, 200, 60));
    add(MenuItem("[H] igh Scores", 300, 330, 200, 60));
    add(MenuItem("[C] redits", 300, 260, 200, 60));
    add(MenuItem("[E] xit", 300, 190, 200, 60));
}

void MainMenu::keyboardInput(int key)
{

    switch (key) {
        //checks key input if lower or uppercase, turns menu off
    case XK_p:
    case XK_P:
        gl.mainMenuOpen = false;
        break;
    }

}
