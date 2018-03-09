//author:Maximillian Wolfe
//3350
//date:2/23/18
//
//project file to display health in a color based off the amount
//this will also display a health bar at the upper right that will 
//correspond with the health above the character
//
//also will toggle name on and off based off of key stroke n
#include "fonts.h"
#include <GL/glx.h>
#include <time.h>
void showhealth(int health,float cx,float cy,float h,int name,int index){
	Rect r;
	r.bot = cy+h-(h/400)*16;
	r.left = cx;
	r.center = cx;
	unsigned int c;
	//color parameters----------------------
	if(health>=75)
		c=0x0000ff00;
	if(health>=50&&health<75)
		c=0x00ffff00;
	if(health>=25&&health<50)
		c=0x00ffa500;
	if(health<25)
		c=0x00ff0000;
	//--------------------------------------
	if(name==1){
		r.bot=r.bot+16;
		ggprint8b(&r, 16, c, "Player %i",index+1);
	}
	ggprint8b(&r, 16, c, "Health: %i",health);
	//print health bar-----------------------
	if(index==1){
		glColor3ub(150,150,150);
		glBegin(GL_QUADS);
		glVertex2i(400*2-5,300*2-5);
		glVertex2i(400*2-5,300*2-40);
		glVertex2i(400*2-15-300,300*2-40);
		glVertex2i(400*2-15-300,300*2-5);
		glEnd();
		
		glBegin(GL_QUADS);
		if(health>=75)
			glColor3ub(0,255,0);
		if(health>=50&&health<75)
			glColor3ub(255,255,0);
		if(health>=25&&health<50)
			glColor3ub(255,165,0);
		if(health<25)
			glColor3ub(255,0,0);
		
		glVertex2i(400*2-10,300*2-10);
		glVertex2i(400*2-10,300*2-35);
		glVertex2i(400*2-10-health*3,300*2-35);
		glVertex2i(400*2-10-health*3,300*2-10);
		glEnd();
		
		c=0x00000000;
		r.bot=300*2-30;
		r.left=400*2-10-150;
		r.center=400*2-10-150;
		ggprint8b(&r, 16, c, "HEALTH: %i%",health);
	}
	if(index==0){
		glColor3ub(150,150,150);
		glBegin(GL_QUADS);
		glVertex2i(5,300*2-5);
		glVertex2i(5,300*2-40);
		glVertex2i(15+300,300*2-40);
		glVertex2i(15+300,300*2-5);
		glEnd();
		
		glBegin(GL_QUADS);
		if(health>=75)
			glColor3ub(0,255,0);
		if(health>=50&&health<75)
			glColor3ub(255,255,0);
		if(health>=25&&health<50)
			glColor3ub(255,165,0);
		if(health<25)
			glColor3ub(255,0,0);
		
		glVertex2i(10,300*2-10);
		glVertex2i(10,300*2-35);
		glVertex2i(10+health*3,300*2-35);
		glVertex2i(10+health*3,300*2-10);
		glEnd();
		
		c=0x00000000;
		r.bot=300*2-30;
		r.left=10+150;
		r.center=10+150;
		ggprint8b(&r, 16, c, "HEALTH: %i%",health);
	}

}

double mTimer1()
{
	static double timeT = 0.0;
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);
	clock_gettime(CLOCK_REALTIME, &end);
	timeT += (double)(end.tv_sec - start.tv_sec)+
		(double)(end.tv_nsec - start.tv_nsec);
		return timeT/10000;
}

double mTimer2()
{
	static double timeT = 0.0;
	struct timespec start, end;
	int x=0;
	clock_gettime(CLOCK_REALTIME, &start);
	for(int i=0;i<10;i++){
		x = x*i/50;
		x = x*x;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	timeT += (double)(end.tv_sec - start.tv_sec)+
		(double)(end.tv_nsec - start.tv_nsec);
		return timeT/10000;
}

double mTimer3()
{
	static double timeT = 0.0;
	struct timespec start, end;
	int x=0;
	clock_gettime(CLOCK_REALTIME, &start);
	for(int i=0;i<1000;i++){
		x = x*i/50;
		x = x*x;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	timeT += (double)(end.tv_sec - start.tv_sec)+
		(double)(end.tv_nsec - start.tv_nsec);
		return timeT/10000;
}
	
	





	
	

