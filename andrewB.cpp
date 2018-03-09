//Andrew Bainbridge
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"


double    obillion = 1.0 / 1e9;

double time_diff(struct timespec *start, struct timespec *end) {
    return (double)(end->tv_sec - start->tv_sec ) +
	(double)(end->tv_nsec - start->tv_nsec) * obillion;
}

double timelab1()
{

    static double t = 0.0;
    struct timespec ftimeStart, ftimeEnd;
    clock_gettime(CLOCK_REALTIME, &ftimeStart);
    int a = 100, b = 200, c = 400;
    for (int i=0; i<10; i++) {
	b = (a&15) - (b&7) - (c&3);
	c = (b&31) - (a&7) - (c&3);	    
    }
    clock_gettime(CLOCK_REALTIME, &ftimeEnd);
    t += time_diff(&ftimeStart, &ftimeEnd);
    return t;
}



/*
   bool OnGround = true;

   void JumpUp(int cy)

   {
   if (OnGround) {
   cy += 11.0f;
   OnGround = false;
   }
   else {
   return; //char in air
   }
   }

   void UpdatePos()
   {
   velY += gravity;
   posY += velY;
   posX += velX;
   if (posY > ) {
   posY =  ;
   velY = 0.0;
   OnGround = true;
   }

   }
   */


