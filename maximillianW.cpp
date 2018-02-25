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
void showhealth(int health,float cx,float cy,float h,int name){
	Rect r;
	r.bot = cy+h-16;
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
		ggprint8b(&r, 16, c, "Maximiliian Wolfe");
	}
	ggprint8b(&r, 16, c, "Health: %i",health);
	//print health bar-----------------------
	glBegin(GL_QUADS);
	glColor3ub(150,150,150);
	glVertex2i(cx*2-5,cy*2-5);
	glVertex2i(cx*2-5,cy*2-40);
	glVertex2i(cx*2-15-300,cy*2-40);
	glVertex2i(cx*2-15-300,cy*2-5);
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
	
	glVertex2i(cx*2-10,cy*2-10);
	glVertex2i(cx*2-10,cy*2-35);
	glVertex2i(cx*2-10-health*3,cy*2-35);
	glVertex2i(cx*2-10-health*3,cy*2-10);
	glEnd();
	
	c=0x00000000;
	r.bot=cy*2-30;
	r.left=cx*2-10-150;
	r.center=cx*2-10-150;
	ggprint8b(&r, 16, c, "HEALTH: %i%",health);
}

