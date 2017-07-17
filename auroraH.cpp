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

/*********************************************************/
//Week 7
//Main menu: Game paused option, pauses game 
//Main menu: Highlight option ready
//Main menu: add enums to handle menu options
//Add movement, keys to Mortana
//Left and right keys should allow Mortana to walk left and right
// Up allows for jump
// Movement physics calculations

/*****************************************************/
//Week 8
// Enemy Character Sprites
// Physics for enemy
// Collision detection

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
#include <algorithm>
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
        //float distance = (a-t0) + (b-t1)
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
posX(w / 2.0f), posY(h / 2.0f),
delay(delayValue),
time(),
visible(true),
direction(1)
{
    // convert to ppm automatically
    string inputFile = "./images/" + filename;
    string outputFile = "./images/converted/" + filename + ".ppm";
    printf("Converting: %s to %s...\n", inputFile.c_str(),
            outputFile.c_str());
    system(("mkdir -p ./images/converted && convert -quality 100 " + inputFile +
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
    //unlink(outputFile.c_str());
    recordTime(&time);
}

Sprite::~Sprite()
{
}

void Sprite::physics()
{
    //calculate time elapsed, update frame
    //render appropriate sprite
    if (frameCount > 1) {
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
    }
}

void Sprite::draw()
{

    if (visible) {
        //Calculate the sprite frame and size
        //and location
        float cx = posX;
        float cy = posY;
        float h = height / 2;
        float w = width / 2;
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
        // direction changes, left & right
        glBegin(GL_QUADS);
        if (direction == 1) {
            glTexCoord2f(tx, ty + th);
            glVertex2i(cx - w, cy - h);
            glTexCoord2f(tx, ty);
            glVertex2i(cx - w, cy + h);
            glTexCoord2f(tx + tw, ty);
            glVertex2i(cx + w, cy + h);
            glTexCoord2f(tx + tw, ty + th);
            glVertex2i(cx + w, cy - h);
        } else {
            glTexCoord2f(tx, ty + th);
            glVertex2i(cx + w, cy - h);
            glTexCoord2f(tx, ty);
            glVertex2i(cx + w, cy + h);
            glTexCoord2f(tx + tw, ty);
            glVertex2i(cx - w, cy + h);
            glTexCoord2f(tx + tw, ty + th);
            glVertex2i(cx - w, cy - h);
        }

        glEnd();
        glPopMatrix();
    }
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

bool Sprite::getVisible()
{
    return visible;
}

void Sprite::setVisible(bool value)
{
    visible = value;
}

void Sprite::reset()
{
    currentFrame = 0;
}

int Sprite::getDirection()
{
    return direction;
}

void Sprite::setDireciont(int value)
{
    direction = value;
}

void initCharacterSprites()
{
    globalSprite.mortana = new Sprite
            ("girl1.gif", 11, 1, 11, 1.0f / 8.0f, 113, 128);
    globalSprite.mortana->setPos(gl.xres / 2, 100);


    //Mortana Jump Sprite
    globalSprite.mortanaJump = new Sprite
            ("jumpMortana.gif", 13, 1, 13, 1.0f / 8.0f, 113, 128);
    globalSprite.mortanaJump->setPos(gl.xres / 2, 100);
}

void renderCharacterSprites()
{
    globalSprite.mortana->draw();
    globalSprite.mortanaJump->draw();

}

void physicsCharacterSprites()
{
    //    static float pos = 0;
    //    Sprite* sp = globalSprite.characterGirl;
    //    sp->setPos(pos, 113 / 2 + 25);
    //    pos += 50;
    physicsMortana();
}

void physicsMortana()
{
    if (gl.state == STATE_GAMEPLAY) {
        Sprite* m = globalSprite.mortana;
        Sprite* mj = globalSprite.mortanaJump;
        m->setVisible(false);
        mj->setVisible(false);
        float cx = gl.mortanaPos[0],
                cy = gl.mortanaPos[1],
                velY = gl.mortanaVelY;
        //printf("Pos(X: %f, Y: %f)\n", cx, cy);
        if (cy <= 100) { //If on the ground check 
            m->setVisible(true);
            if (gl.keys[XK_Up]) {
                velY += 100;
                cy += 1;
                printf("Jump!\n");
                //reset animation
                mj->reset();
                m->reset();
            } else if (gl.keys[XK_Right]) {
                // mortana walking right
                m->physics();
                cx += 3;
                if (m->getDirection() == 0) {
                    m->setDireciont(1);
                    mj->setDireciont(1);
                }
            } else if (gl.keys[XK_Left]) {
                // mortana walking left
                m->setVisible(true);
                m->physics();
                if (m->getDirection() == 1) {
                    m->setDireciont(0);
                    mj->setDireciont(0);
                }
                cx -= 3;
            }
        } else {
            //in the air            
            globalSprite.mortanaJump->physics();
            printf("Flying(VelY: %f, Y: %f)!\n", velY, cy);
            velY -= 9.81;
            cy += 0.25 * velY;
            if (cy <= 100) {
                velY = 0;
                cy = 100;
            }
            mj->setVisible(true);
        }


        m->setPos(cx, cy);
        mj->setPos(cx, cy);
        gl.mortanaPos[0] = cx;
        gl.mortanaPos[1] = cy;
        gl.mortanaVelY = velY;
    }
}


//*
// ENEMY
//*
void initEnemySprites(){
    //Pink ghost
     globalSprite.pinkghost = new Sprite
            ("ghostPink.gif", 4, 1, 4, 1.0f / 8.0f, 113, 128);
    globalSprite.pinkghost->setPos(gl.xres / 2, 200);
    
//blanket ghost
     globalSprite.blanketghost = new Sprite
            ("blanketghost.gif", 3, 3, 3, 1.0f / 8.0f, 113, 128);
    globalSprite.blanketghost->setPos(gl.xres / 2, 300);

 // White pac ghost
    globalSprite.pacghost = new Sprite
            ("pacghost.gif", 9, 6, 9, 1.0f / 8.0f, 113, 128);
    globalSprite.pacghost->setPos(gl.xres / 2, 400);
    
//    // White pac ghost part 2
//    globalSprite.pacghost01 = new Sprite
//            ("pacghost01.gif", 3, 4, 3, 1.0f / 8.0f, 113, 128);
//    globalSprite.pacghost01->setPos(gl.xres / 2, 500);
    
}
void physicsEnemySprites(){
   
    
}
void renderEnemySprites(){
    globalSprite.pinkghost->draw();
    globalSprite.blanketghost->draw();
    globalSprite.pacghost->draw();
    //globalSprite.pacghost01->draw();
    
}





//**
// Menu
//**


MenuItem::MenuItem(std::string txt, int x, int y, int w, int h)
: text(txt), posX(x), posY(y), width(w), height(h), highlight(false)
{

}
// draw out menu items

void MenuItem::draw()
{
    // rendering boxes 
    glPushMatrix();
    if (highlight) {
        glColor3ub(255, 255, 0);
    } else {
        glColor3ub(255, 255, 255);
    }
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
        MenuItem& menuitem = this->menuItems[i];
        menuitem.draw();
    }
}

Menu::Menu() : menuItems(), selectedItemIndex(0)
{
    //empty constructor
}

bool Menu::setSelectedIndex(unsigned int index)
{
    //validate input
    if (index < menuItems.size()) {
        //turn the previous highlighting off
        menuItems[selectedItemIndex].highlight = false;
        //and highlight the new index
        menuItems[index].highlight = true;
        //save new index
        selectedItemIndex = index;
        return true;
    } else {
        return false;
    }
}

unsigned int Menu::getSelectedIndex()
{
    return selectedItemIndex;
}

unsigned int Menu::getSize()
{
    return menuItems.size();
}

Menu::~Menu()
{
    //empty virtual destructor
}

void Menu::add(MenuItem item)
{
    if (menuItems.empty()) {
        item.highlight = true;
    }
    this->menuItems.push_back(item);
}

MainMenu::MainMenu()
{
    ////add all menu items    
    add(MenuItem("Play", 300, 400, 200, 60));
    add(MenuItem("High Scores", 300, 330, 200, 60));
    add(MenuItem("Credits", 300, 260, 200, 60));
    add(MenuItem("Exit", 300, 190, 200, 60));
}

void MainMenu::draw()
{
    if (gl.state == STATE_GAMEPAUSE) {
        MenuItem menuitem("Game Paused", 300, 450, 200, 60);
        menuitem.draw();
    }
    Menu::draw();
}

//*
//Instructions for user
//*

void renderTutorial()
{
    Rect r;

    unsigned int c = 0x00FF0000;
    r.bot = gl.yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 20, c, "INSTRUCTIONS");
    ggprint8b(&r, 16, c, "Right arrow -> walk right");
    ggprint8b(&r, 16, c, "Left arrow  <- walk left");
    ggprint8b(&r, 16, c, "UP arrow--Jump");
    ggprint8b(&r, 16, c, "'P/p' Game Pause/Resume ");
    ggprint8b(&r, 16, c, "'F/f' Attack ");

}

void MainMenu::keyboardInput(int key)
{
    switch (key) {
        //checks key input if lower or uppercase, turns menu off
    case XK_p:
    case XK_P:
        if (gl.state == STATE_GAMEPLAY) {
            gl.state = STATE_GAMEPAUSE;
        } else {
            gl.state = STATE_GAMEPLAY;
        }
        break;
    case XK_Escape:
        if (gl.state == STATE_GAMEPAUSE) {
            gl.state = STATE_GAMEPLAY;
        } else {
            gl.state = STATE_GAMEPAUSE;
        }
    case XK_Up:
        if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP) {
            gl.mainMenu.setSelectedIndex(gl.mainMenu.getSelectedIndex()
                    - 1 % gl.mainMenu.getSize());
        }
        break;
    case XK_Down:
        if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP) {
            gl.mainMenu.setSelectedIndex(gl.mainMenu.getSelectedIndex()
                    + 1 % gl.mainMenu.getSize());
        }
        break;
    case XK_Return:
        if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP) {
            switch (gl.mainMenu.getSelectedIndex()) {
                // This is not ready, goes back to play mode
            case 0:
                gl.state = STATE_GAMEPLAY;
                break;
            case 1:
                gl.state = STATE_HIGHSCORE;
                break;
            case 2:
                gl.state = STATE_CREDITS;

                break;
            case 3:
                gl.done = 1;

                break;

            }
        }
        break;
    }
}
