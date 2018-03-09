// Author: Guangjin Liu
// CMPS 3350
// modified date: 2-23-18
// file for background 

#include "fonts.h"
#include <GL/glx.h>
#include <GL/glu.h>
#include <time.h>

void background(int x, int y) {

	static float angle = 0.0;

	glPushMatrix();

	glTranslatef(x, y, 0);
	glBegin(GL_QUADS);
	glVertex2i( 0, 0);
	glVertex2i( 0, 100);
	glVertex2i(100,100);
	glVertex2i(200, 0);
	
	
	glEnd();
	Rect r;
	r.bot = 100;
	r.left = 100;
	r.center = 0;
	
	 "images/background.gif";
	glPopMatrix();

}

double gtimer() {
    int x = 0;
    static double t = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 10; i++) {
	x = x*i/50;
	x = x*x;
    }
    clock_gettime(CLOCK_REALTIME, &end);

    t += (double)(end.tv_sec - start.tv_sec) + 
	(double)(end.tv_nsec - start.tv_nsec);
    return t/10000;
}

double gtimer2() {
    int x = 0;
    static double t = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 100; i++) {
	x = x*i/50;
	x = x*x;
    }
    clock_gettime(CLOCK_REALTIME, &end);

    t += (double)(end.tv_sec - start.tv_sec) + 
	(double)(end.tv_nsec - start.tv_nsec);
    return t/10000;
}

double gtimer3() {
    int x = 0;
    static double t = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 1000; i++) {
	x = x*i/50;
	x = x*x;
    }
    clock_gettime(CLOCK_REALTIME, &end);

    t += (double)(end.tv_sec - start.tv_sec) + 
	(double)(end.tv_nsec - start.tv_nsec);
    return t/10000;
}
