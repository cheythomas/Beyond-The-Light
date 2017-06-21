#ifndef GAME_H
#define GAME_H

#include <string>


//All function and class prototypes go here

//function prototypes
void renderBackground(void); //render prototype
void applyBackgroundMovement(void);

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

class Timers {
public:
    	double physicsRate;
	double oobillion;
} timers;


#endif /* GAME_H */

