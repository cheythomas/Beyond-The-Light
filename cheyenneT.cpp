// modified by: cheyenne thomas
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "ppm.h"
#include "fonts.h"
typedef float Vec[5];

class Light {

	public:
		Vec lightPosition;
		Vec lightDiffuse;
		Vec lightSpecular;
		Vec lightAmbient;
		Vec lightDirection;
	
		Light();
		void drawSpotlight();
		
};

void drawSpotlight() {

}
