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
// tart on main menu/start up menu
// Allow for collection of points
// Allow for new game
// Settings
// Exit game

/********************************************************/
//Week 7
//Main menu: Game paused option, pauses game
//Main menu: Highlight option ready
//Main menu: add enums to handle menu options
//Add movement, keys to Mortana
//Left and right keys should allow Mortana to walk left and right
// Up allows for jump
// Movement physics calculations

/******************************************************/
//Week 8
// Render Character Enemy Sprites
// Physics for cat companion

/******************************************************/
//Week 9
// Main menu Background
// Menu item and user tutorial
// Credits button
// Collision detection
/*****************************************************/
//Week 10
// Final Phase:
//Final collision
// Final sound additions
//****************************************************/

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
posX(w / 2.0f), posY(h / 2.0f),
delay(delayValue),
time(),
visible(true),
direction(1),
repeating(true),
reverse(false),
angle(0)
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
            if (!reverse) {
                if (currentFrame + 1 < frameCount) {
                    ++currentFrame;
                } else if (repeating) {
                    currentFrame = 0;
                }
            } else {
                //previous frame
                if (currentFrame - 1 >= 0) {
                    --currentFrame;
                } else if (repeating) {
                    currentFrame = frameCount;
                }
            }
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

        if (angle != 0) {
            glTranslated(cx, cy, 0);
            glRotated(angle, 0, 0, 1);
            glTranslated(-cx, -cy, 0);
        }

        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, glTexture);
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
        glBindTexture(GL_TEXTURE_2D, 0);
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

void Sprite::setDirection(int value)
{
    direction = value;
}

int Sprite::getFrameIndex()
{
    return currentFrame;
}

void Sprite::setFrameIndex(int index)
{
    currentFrame = index;
}

bool Sprite::getRepeating()
{
    return repeating;
}

void Sprite::setRepeating(bool repeat)
{
    repeating = repeat;
}

bool Sprite::getReverse()
{
    return reverse;
}

void Sprite::setReverse(bool value)
{
    reverse = value;
}

void Sprite::setAngle(float val)
{
    angle = val;
}

float Sprite::getAngle()
{
    return angle;
}

int Sprite::getFrameCount()
{
    return frameCount;
}

void initCharacterSprites()
{
    //mortana walk sprite
    globalSprite.mortana = new Sprite
            ("girl1.gif", 11, 1, 11, 1.0f / 8.0f, 113, 128);
    globalSprite.mortana->setPos(gl.xres / 2, 100);


    //Mortana Jump Sprite
    globalSprite.mortanaJump = new Sprite
            ("jumpMortana.gif", 13, 1, 13, 1.0f / 8.0f, 113, 128);
    globalSprite.mortanaJump->setPos(gl.xres / 2, 100);



    //black cat walk
    globalSprite.blkcat = new Sprite
            ("blkscat.gif", 12, 12, 1, 1.0f / 8.0f, 50, 100);
    globalSprite.blkcat->setPos(gl.xres / 1.5 - 100, 100);
    globalSprite.blkcat->setDirection(0);
    globalSprite.blkcat->setVisible(false);

    //blk cat sit
    globalSprite.blkcatsit = new Sprite
            ("blkscats.gif", 6, 6, 1, 1.0f / 8.0f, 50, 100);
    globalSprite.blkcatsit->setPos(gl.xres / 1.5 - 100, 100);
    globalSprite.blkcatsit->setDirection(0);
    globalSprite.blkcatsit->setFrameIndex(5);
    globalSprite.blkcatsit->setRepeating(false);

}

void renderCharacterSprites()
{
    globalSprite.blkcat->draw();
    globalSprite.blkcatsit->draw();
    globalSprite.mortana->draw();
    globalSprite.mortanaJump->draw();
}

void physicsCharacterSprites()
{

    physicsMortana();

}

//physics mortana and blk cat

void physicsMortana()
{
    int catDistance = 200;
    int catDistanceSit = 170;
    if (gl.state == STATE_GAMEPLAY) {
        Sprite* m = globalSprite.mortana;
        Sprite* mj = globalSprite.mortanaJump;
        Sprite* cat = globalSprite.blkcat;
        Sprite* catsit = globalSprite.blkcatsit;
        m->setVisible(false);
        mj->setVisible(false);
        cat->setVisible(false);
        catsit->setVisible(false);
        float cx = gl.mortanaPos[0],
                cy = gl.mortanaPos[1],
                velY = gl.mortanaVelY,
                catx = gl.catPos[0],
                caty = gl.catPos[1];
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
                playJump(); //plays jump sound

                if (catsit->getFrameIndex() == 5) {
                    catsit->setVisible(true);
                    playMeow();
                } else {
                    cat->setVisible(true);
                }
                catsit->reset();
                playMeow();
            } else if (gl.keys[XK_Right]) {
                // mortana walking right
                m->physics();
                cx += 1;
                if (m->getDirection() == 0) {
                    m->setDirection(1);
                    mj->setDirection(1);
                }
                if (catx > cx - catDistance) {
                    //cat is on the right side of mortana
                    catx -= 1;
                    catsit->setVisible(true);
                    catsit->physics();
                } else {
                    cat->physics();
                    cat->setVisible(true);
                    catx = cx - catDistance;
                    cat->setDirection(0);
                    catsit->setDirection(0);
                }
                catsit->reset();
            } else if (gl.keys[XK_Left]) {
                // mortana walking left
                m->setVisible(true);
                m->physics();
                if (m->getDirection() == 1) {
                    m->setDirection(0);
                    mj->setDirection(0);
                }
                cx -= 1;
                if (catx < cx + catDistance) {
                    //cat is on the left side of mortana
                    catx += 1;
                    catsit->setVisible(true);
                    catsit->physics();
                } else {
                    cat->physics();
                    cat->setVisible(true);
                    catx = cx + catDistance;
                    cat->setDirection(1);
                    catsit->setDirection(1);
                }

            } else {
                //let cat sit down
                catsit->setVisible(true);
                catsit->physics();
                if (catx < cx - catDistanceSit) {
                    //Mortana is not moving but the cat is on the right side
                    catx += 1;
                } else if (catx > cx + catDistanceSit) {
                    //mortana is not moving but the cat is on the left side
                    catx -= 1;
                }
            }
        } else {
            //in the air
            globalSprite.mortanaJump->physics();
            velY -= 9.81;
            cy += 0.25 * velY;
            if (cy <= 100) {
                velY = 0;
                cy = 100;
            }
            mj->setVisible(true);
            if (catsit->getFrameIndex() == 5) {
                catsit->setVisible(true);
            } else {
                cat->setVisible(true);
            }
        }
        m->setPos(cx, cy);
        mj->setPos(cx, cy);
        cat->setPos(catx, caty);
        catsit->setPos(catx, caty);

        gl.mortanaPos[0] = cx;
        gl.mortanaPos[1] = cy;
        gl.mortanaVelY = velY;
        gl.catPos [0] = catx;
        gl.catPos [1] = caty;
        gl.catVelY = velY;
    }
}


//*
// ENEMY
//*

void initEnemySprites()
{
    //Pink ghost
    globalSprite.pinkghost = new Sprite
            ("ghostPink.gif", 4, 1, 4, 1.0f / 4.0f, 113, 128);
    globalSprite.pinkghost->setPos(gl.xres / 2, 200);

    //blanket ghost
    globalSprite.whiteghost = new Sprite
            ("blanketghost.gif", 9, 3, 3, 1.0f / 2.0f, 123, 138);
    globalSprite.whiteghost->setPos(gl.xres / 2, 300);

    // White pac ghost
    globalSprite.pacghost = new Sprite
            ("pacghost.gif", 9, 6, 9, 1.0f / 4.0f, 130, 150);
    globalSprite.pacghost->setPos(gl.xres / 2, 400);


}

void renderEnemySprites()
{
    for (unsigned int i = 0; i < gl.enemies.size(); i++) {
        Enemy& en = gl.enemies[i];
        Sprite* s = NULL;
        switch (en.spriteId) {
        case 0:
            s = globalSprite.pinkghost;
            break;
        case 1:
            s = globalSprite.pacghost;
            break;
        case 2:
            s = globalSprite.whiteghost;
            break;
        default:
            printf("Invalid sprite id\n");
            break;
        }
        s->setFrameIndex(en.frameIndex);
        s->setPos(en.x, en.y);
        s->draw();
    }
}

void moveGhostToMortana(Enemy& enemy)
{
    float mx = gl.mortanaPos[0],
            my = gl.mortanaPos[1];
    float cx1 = enemy.x,
            cy1 = enemy.y;

    float movePixels = rnd() * 10;
    float moveVerticalAlso = rnd() * 0.5;
    //angle between mortana and ghost
    float angle = std::atan2(my - cy1, mx - cx1);
    //Find components of vector
    float velX = movePixels * std::cos(angle) + moveVerticalAlso;
    float velY = movePixels * std::sin(angle);
    enemy.x += velX;
    enemy.y += velY;
}

void physicsPinkGhost(Enemy& enemy)
{
    Sprite* pghost = globalSprite.pinkghost;
    pghost->setFrameIndex(enemy.frameIndex);
    pghost->physics();
    enemy.frameIndex = pghost->getFrameIndex();
    moveGhostToMortana(enemy);
}

void physicsWhiteGhost(Enemy& enemy)
{

    Sprite* gs = globalSprite.whiteghost;
    gs->setFrameIndex(enemy.frameIndex);
    gs->physics();
    enemy.frameIndex = gs->getFrameIndex();
    moveGhostToMortana(enemy);
}

void physicsPacGhost(Enemy& enemy)
{
    Sprite* gs = globalSprite.pacghost;
    gs->setFrameIndex(enemy.frameIndex);
    gs->physics();
    enemy.frameIndex = gs->getFrameIndex();
    moveGhostToMortana(enemy);
}

void physicsGhosts()
{
    for (unsigned int i = 0; i < gl.enemies.size(); i++) {
        Enemy& en = gl.enemies[i];
        switch (en.spriteId) {
        case 0:
            physicsPinkGhost(en);
            break;
        case 1:
            physicsPacGhost(en);
            break;
        case 2:
            physicsWhiteGhost(en);
            break;
        default:
            printf("Invalid sprite id\n");
            break;
        }
    }
}
// **
// COLLISION
//**

void lightningCollision(Enemy& en)
{
    if (gl.lightning) {
        float x = gl.mortanaPos[0];
        float y = gl.mortanaPos[1];
        //center values from physicsLightSprite
        switch (gl.lightning) {
        case 1:
            x += 120;
            break;
        case 2:
            x += 106;
            y += 128;
            break;
        case 3:
            y += 128;
            break;
        case 4:
            x -= 106;
            y += 90;
            break;
        case 5:
            x -= 120;
            break;
        default:
            break;
        }
        bool Collision = checkCircle(
                en.x, en.y,
                x, y,
                25, 75);
        if (Collision) {
            printf("A lighting hit a ghost (%d)!\n", en.spriteId);
            en.alive = false;
            trackKills(en.spriteId);
            playGhostDeath();
        }
    }
}

void mortanaCollision()
{

    for (std::vector<Enemy>::iterator it = gl.enemies.begin(),
            end = gl.enemies.end();
            it != end; it++) {
        Enemy& en = *it;
        float radiusEnemy = 25;
        float mortanaRadius = 75;

        bool Collision = checkCircle(
                en.x, en.y,
                gl.mortanaPos[0], gl.mortanaPos[1],
                mortanaRadius, radiusEnemy);

        if (Collision) {
            // printf("There is a direct collision with Mortana and ghost:
            //Game over! %d\n", en.spriteId);
            gl.keepTrack = 10;
            gl.state = STATE_GAMEOVER;
            restart();
            return;
        }
        //check collision with lightning
        lightningCollision(en);

        if (!en.alive) {
            it = gl.enemies.erase(it);
        }

    }
}
//generate randomness for ghosts

void ghostRandom()
{
    //Increase ghost random creation linearly
    if (rand() % (60 - gl.points / 50) == 0) { //rate of spawn
        float x = (rand() % gl.xres) + gl.camera[0];
        float y = (rand() % (gl.yres - 350)) + 350;
        //if mortana is going to the left, bias to the left
        float bias = 600;
        if (gl.keys[XK_Left]) {
            x -= bias;
        } else if (gl.keys[XK_Right]) { //else if she's going to the right, bias right
            x += bias;
        }
        Enemy en(x, y, rand() % 3);
        gl.enemies.push_back(en);
    }
}

bool checkCircle(double x1, double y1, double x2, double y2,
        float r1, float r2)
{
    float distance = std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    float distanceFromRadius = (distance - r1 - r2);
    return distanceFromRadius <= 0;
}



//**
// Menu
//**
//background for menu

void initMenuBackground()
{
    globalSprite.backgroundMenu = new Sprite("boy.gif", 1, 1, 1, 1, 1280, 2220);
    globalSprite.backgroundMenu->setPos(gl.xres / 0.75, 500);


}

void renderMenuBackground()
{
    globalSprite.backgroundMenu->draw();

}

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

int MenuItem::getPosX()
{
    return posX;
}

int MenuItem::getPosY()
{
    return posY;
}

void MenuItem::setPos(int x, int y)
{
    posX = x;
    posY = y;
}

std::string MenuItem::getText()
{
    return text;
}

void MenuItem::setText(std::string value)
{
    text = value;
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
    playClick();
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
        menuItems[0].setText("Resume");
    } else {
        menuItems[0].setText("Play");
    }
    Menu::draw();
}

void MainMenu::resize(int oldw, int neww, int oldh, int newh)
{
    // position of main menu items
    int xDiff = (neww - oldw) / 2;
    int yDiff = (newh - oldh) / 2;
    for (unsigned int i = 0; i < menuItems.size(); i++) {
        int x = menuItems[i].getPosX();
        int y = menuItems[i].getPosY();
        menuItems[i].setPos(x + xDiff, y + yDiff);
    }

}

//*
//Instructions for user
//*

void renderTutorial()
{
    Rect r;
    unsigned int c = 0x00FF0000;
    r.bot = gl.yres - 20;
    r.left = -gl.camera[0] + 10;
    r.center = 0;
    ggprint8b(&r, 20, c, "INSTRUCTIONS");
    ggprint8b(&r, 20, c, "Walk Mode:");
    ggprint8b(&r, 16, c, "Right arrow -> walk right");
    ggprint8b(&r, 16, c, "Left arrow  <- walk left");
    ggprint8b(&r, 16, c, "UP arrow--Jump");
    ggprint8b(&r, 25, c, "'P/p' Game Pause/Resume ");
    ggprint8b(&r, 18, c, "Attack Mode:");
    ggprint8b(&r, 18, c, "W--up");
    ggprint8b(&r, 18, c, "A/D--Forward");
    ggprint8b(&r, 18, c, "E--upright");
    ggprint8b(&r, 18, c, "Q--upleft");


}

void monitorCTRLC(int key)
{

    if (gl.state == STATE_CREDITS || gl.state == STATE_GAMEPLAY) {
        if (key == XK_c && gl.keys[XK_Control_L]) {
            if (gl.state == STATE_GAMEPLAY) {
                gl.state = STATE_CREDITS;
            } else {
                gl.state = STATE_GAMEPLAY;
            }
        }
    }
}

void MainMenu::keyboardInput(int key)
{
    switch (key) {
        //checks key input if lower or uppercase, turns menu off
    case XK_p:
    case XK_P:
        if (gl.state == STATE_GAMEPLAY) {
            gl.state = STATE_GAMEPAUSE;
        } else if (gl.state != STATE_GAMEOVER) {
            gl.state = STATE_GAMEPLAY;
        }
        break;
    case XK_Escape:
        if (gl.state == STATE_STARTUP) {
            gl.done = 1;
        } else if (gl.state == STATE_GAMEPAUSE) {
            gl.state = STATE_GAMEPLAY;
        } else if (gl.state == STATE_GAMEOVER) {
            gl.state = STATE_STARTUP;
        } else {
            gl.state = STATE_GAMEPAUSE;
        }
    case XK_Up:
        if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP) {
            gl.mainMenu.setSelectedIndex(gl.mainMenu.getSelectedIndex()
                    - 1 % gl.mainMenu.getSize());
            playSelection();
        }
        break;
    case XK_Down:
        if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP) {
            gl.mainMenu.setSelectedIndex(gl.mainMenu.getSelectedIndex()
                    + 1 % gl.mainMenu.getSize());
            playSelection();
        }
        break;
    case XK_Return:
        if (gl.state == STATE_GAMEPAUSE || gl.state == STATE_STARTUP) {
            playClick();
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

void auroraRestart()
{
    //get rid of enemies
    gl.enemies.clear();
    gl.mortanaPos[0] = gl.xres / 2;
    gl.mortanaPos[1] = 100;
    // blkcat
    gl.catPos[0] = gl.xres / 2;
    gl.catPos[1] = 70;
}
