// Author: Guangjin Liu
// CMPS 3350
// modified date: 2-23-18
// file for background 

#include "fonts.h"
#include <GL/glx.h>
#include <GL/glu.h>
#include <time.h>
#include <GL/gl.h>

int w1 = 0;
int h1 = 0;

void reshape (int w, int h) {
    w1 = w;
    h1 = h;
    glViewport(0, 0, w, h);
}

void orthogonalStart() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-w1/2, w1/2, -h1/2, h1/2);
    glMatrixMode(GL_MODELVIEW);
}

void orthogonalEnd() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

Gluint texture = 0;

Gluint LoadTexture(const char *filename, int width, int height) {
    Gluint texture;
    unsigned char data[] = {255,   0,   0,   0, 
                            255,   0,   0,   0, 
                            255, 255, 255, 255};
    File * file;

    file = fopen(filename, "images/background.gif");
    if (file == NULL)
        return 0;
    data = (unsigned char *)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    glGenTextures( 1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);
    return texture;
}

void FreeTexture (Gluint texture) {
    glDeleteTextures( 1, &texture);
}

void background() {
    glBindTexture(GL_TEXTURE_2D, texture);
    
    orthogonalStart();

    const int iw = 500;
    const int ih = 500;

    glPushMatrix();
    glTranslatef(-iw/2, -ih/2, 0);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i( 0,  0);
    glTexCoord2i(1, 0); glVertex2i(1w,  0);
    glTexCoord2i(1, 1); glVertex2i(iw, ih);
    glTexCoord2i(0, 1); glVertex2i( 0, ih);

    glEnd();
    glPopMatrix();

    orthogonalEnd();
}

void display() {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);

    background();
    gluLookAt(0.0, 0.0, 5.0, 
              0.0, 0.0, 0.0, 
              0.0, 1.0, 0.0);

    glutSwapBuffers();
}
/*
GLuint LoadTexture() {
    unsigned char data[] = {255,   0,   0,   0, 
                            255,   0,   0,   0, 
                            255, 255, 255, 255};

    glGenTextures( 1, &texture);
    glBindTexture( GL_TEXTURE_2D, texture);
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(Gl_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, DATA);
    return texture;
}
*/
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
