// Author: Aurora Hernandez
// Date: 06/17/17
// Purpose: sprite animations



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

double timeDiff(struct timespec *start, struct timespec *end)
{
    return (double) (end->tv_sec - start->tv_sec) +
            (double) (end->tv_nsec - start->tv_nsec) * (1e-9);
}


Sprite::Sprite(const std::string & filename,
        int frameCountValue,
        int rowsValue,
        int colsValue,
        double delayValue
        ) : frameCount(frameCountValue), rows(rowsValue), cols(colsValue),
delay(delayValue)
{
    // convert to ppm automatically
    string inputFile = "./images/" + filename;
    string outputFile = "./images/converted/" + filename + ".ppm";
    system(("convert  " + inputFile + " " + outputFile).c_str());

    //create openGL IMAGE
    //use build AlphaData to convert transparent
    //save result in class variables


    Ppmimage *ppmImage = ppm6GetImage(outputFile.c_str());
    height = ppmImage->height;
    width = ppmImage->width;
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //must build a new set of data...
    unsigned char *imageData = buildAlphaData(ppmImage);
    // unsigned buildAlphaData(gl.explosiveImage);//***********
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, imageData
            );

    free(imageData);
    ppm6CleanupImage(ppmImage);
}

Sprite::~Sprite()
{

}

void Sprite::draw()
{
    //calculate time elapsed, update frame
    //render appropriate sprite

    struct timespec current;
    clock_gettime(CLOCK_REALTIME, &current);

    double timeSpan = timeDiff(&time, &current);
    if (timeSpan > delay) {
        //next frame
        ++currentFrame;
        if (delay >= frameCount)
            frameCount = 0;
        clock_gettime(CLOCK_REALTIME, &time);
    }


    //Calculate the sprite frame and size
    //and location
    float cx = posX;
    float cy = posY;
    float h = height / rows;
    float w = width / cols;
    int ix = currentFrame % frameCount;
    int iy = currentFrame / cols;
    float tx = (float) ix / cols;
    float ty = (float) iy / rows;
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, glTexture);

    //
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255, 255, 255, 255);


    glBegin(GL_QUADS);
    glTexCoord2f(tx, ty + .5);
    glVertex2i(cx - w, cy - h);
    glTexCoord2f(tx, ty);
    glVertex2i(cx - w, cy + h);
    glTexCoord2f(tx + .125, ty);
    glVertex2i(cx + w, cy + h);
    glTexCoord2f(tx + .125, ty + .5);
    glVertex2i(cx + w, cy - h);

    glEnd();
    glPopMatrix();
}

void Sprite::setPosition(float x, float y)
{
    posX = x;
    posY = y;
}
