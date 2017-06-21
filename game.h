#ifndef GAME_H
#define GAME_H

#include <string>


//All function and class prototypes go here

//function prototypes
void renderBackground(void); //render prototype
void applyBackgroundMovement(void);
void initCharacterSprites(); // for Sprite characters
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

//added class Timers

// Double defined /kS
/*class Timers {
public:
    double physicsRate;
    double oobillion;
} timers;
*/
/*This creates a class called GlobalSprite, has one variable of type Sprite*,
 *named characterGirl
It also at the same time makes it accessible globally
by the name of globalSprite
 */
struct GlobalSprite {
    Sprite* characterGirl;
} globalSprite;


#endif /* GAME_H */

