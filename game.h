
#ifndef GAME_H
#define GAME_H

#include <string>


//All function and class prototypes go here


/**
 * Sprite animation class
 */
class Sprite {
    GLuint glTexture;
    int frameCount;
    int rows;
    int cols;
    int width;
    int height;
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
        double delay
    );
    
    ~Sprite();
    
    void setPosition(float x, float y);
   
    void draw();
};

#endif /* GAME_H */

