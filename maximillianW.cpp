#include "fonts.h"
void showhealth(int health){
	Rect r;
	r.bot = 300+200;
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
	ggprint8b(&r, 16, c, "Health: %i",health);
}

