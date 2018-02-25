//author:Maximillian Wolfe
//3350
//date:2/23/18
//project file to display health in a color based off the amount
//also will toggle name on and off based off of key stroke n
#include "fonts.h"
void showhealth(int health,int name){
	Rect r;
	r.bot = 300+200-16;
	r.left = 400;
	r.center = 400;
	unsigned int c;
	//color parameters----------------------
	if(health>=80)
		c=0x0000ff00;
	if(health>=50&&health<80)
		c=0x00ffff00;
	if(health>=20&&health<50)
		c=0x00ffa500;
	if(health<20)
		c=0x00ff0000;
	//--------------------------------------
	if(name==1){
		r.bot=r.bot+16;
		ggprint8b(&r, 16, c, "Maximiliian Wolfe");
	}
	ggprint8b(&r, 16, c, "Health: %i",health);
}

