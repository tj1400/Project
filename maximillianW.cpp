//author:Maximillian Wolfe
//3350
//date:2/23/18
//
//Maximillian Wolfe

//project file to display health in a color based off the amount
//this will also display a health bar at the upper right that will 
//correspond with the health above the character
//
//also will toggle name on and off based off of key stroke n
#include "fonts.h"
#include <GL/glx.h>
#include <time.h>
typedef double Flt;

typedef double Vec[3];

class Bullet {
	public:
	int ind;
	Vec pos;
	int vel;
}b[2][20];
int bullets1 = 0;
int bullets2 = 0;

void showhealth(int health,float cx,float cy,float h,int name,int index,int power,Flt xmod,Flt ymod){
	if(health>0){
		Rect r;
		r.bot = (cy+h-(h/400)*16)*ymod;
		r.left = (cx)*xmod;
		r.center = (cx)*xmod;
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
			glPushMatrix();
			glScalef(xmod,ymod,1.0);
			glColor3ub(150,150,150);
			glBegin(GL_QUADS);
			glVertex2i(400*2-5,300*2-5);
			glVertex2i(400*2-5,300*2-50);
			glVertex2i(400*2-15-300,300*2-50);
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
			
			glColor3ub(100,100,255);
			glBegin(GL_QUADS);
			glVertex2i(400*2-10,300*2-40);
			glVertex2i(400*2-10,300*2-45);
			glVertex2i(400*2-10-power*3,300*2-45);
			glVertex2i(400*2-10-power*3,300*2-40);
			glEnd();
			glPopMatrix();
			
			
			c=0x00000000;
			r.bot=(300*2-30)*ymod;
			r.left=(400*2-10-150)*xmod;
			r.center=(400*2-10-150)*xmod;
			ggprint8b(&r, 16, c, "HEALTH: %i%",health);
		}
		if(index==0){
			glPushMatrix();
			glScalef(xmod,ymod,1.0);
			glColor3ub(150,150,150);
			glBegin(GL_QUADS);
			glVertex2i(5,300*2-5);
			glVertex2i(5,300*2-50);
			glVertex2i(15+300,300*2-50);
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
			
			glColor3ub(100,100,255);
			glBegin(GL_QUADS);
			glVertex2i(10,300*2-40);
			glVertex2i(10,300*2-45);
			glVertex2i(10+power*3,300*2-45);
			glVertex2i(10+power*3,300*2-40);
			glEnd();
			glPopMatrix();
			
			c=0x00000000;
			r.bot=(300*2-30)*ymod;
			r.left=(10+150)*xmod;
			r.center=(10+150)*xmod;
			ggprint8b(&r, 16, c, "HEALTH: %i%",health);
		}
	}
}

void shoot(Vec pos,int ind, int dir, int w,Flt xmod,Flt ymod){
	if(ind == 0){
		b[0][bullets1].pos[0] = pos[0] + (w/2*dir);
		b[0][bullets1].pos[1] = pos[1];
		b[0][bullets1].vel = 4 * dir;
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+4.0/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/11.0)); glVertex2i(pos[0]-5, pos[1]-5);
				glTexCoord2f(tx,      ty+(1.0/50.0));    glVertex2i(pos[0]-5, pos[1]+5);
				glTexCoord2f(tx+(1.0/27.0), ty+(1.0/50.0));    glVertex2i(pos[0]+5, pos[1]+5);
				glTexCoord2f(tx+(1.0/27.0), ty+(1.0/11.0)); glVertex2i(pos[0]+5, pos[1]-5);
		glEnd();
		bullets1++;
	}
	if(ind == 1){
		b[1][bullets2].pos[0] = pos[0] + (w/2*dir);
		b[1][bullets2].pos[1] = pos[1];
		b[1][bullets2].vel = 4 * dir;
		bullets2++;
	}
}

int moveBullets(Vec p1,Vec p2, float w, float h,int xres,int health1,int health2,GLuint walkTexture,GLuint walkTexture2,Flt xmod,Flt ymod){
	int hit = 0;
	for(int i=0;i<bullets1;i++){
		b[0][i].pos[0]+= b[0][i].vel;
		float distx = b[0][i].pos[0]-p2[0];
		float disty = b[0][i].pos[1]-p2[1];
		float pdistx = b[0][i].pos[0]-p1[0];
		if(distx<0)
			distx=-distx;
		if(pdistx<0)
			pdistx=-pdistx;
		if(disty<0)
			disty=-disty;
		if(distx<w/2&&disty<h&&health1>0&&health2>0){
			bullets1--;
			for(int j=i;j<bullets1;j++){
				b[0][j] = b[0][j+1];
			}
			hit += 2;;
		}
		if(b[0][i].pos[0]>xres||b[0][i].pos[0]<0){
			bullets1--;
			for(int j=i;j<bullets1;j++){
				b[0][j] = b[0][j+1];
			}
		}
		//printf("in here");
		glPushMatrix();
		glScalef(xmod,ymod,1.0);
		glColor3f(1.0, 1.0, 1.0);
		//if(p[i].dir==-1)
			//glBindTexture(GL_TEXTURE_2D, g.walkBackTexture);
		//if(p[i].dir==1)
		glBindTexture(GL_TEXTURE_2D, walkTexture);
		//
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255);
		
		if(pdistx<20){
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+4.2/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
			}
		else if(pdistx<35){
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+4.8/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
	}
		else if(pdistx<50){
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+5.6/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
		}
		else if(pdistx<65){
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+6.2/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
		}
		else if(pdistx<80){
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+6.8/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
		}
		else if(pdistx<95){
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+7.6/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
		}
		else{
			int ix = 25.0;
			int iy = 4;
		//	if(p[i].walkFrame>6){
		//		iy = 2;
		//	}
			float tx = (float)ix / 25.0+8.3/25.0;
			float ty = (float)iy / 10.0+1.4/20.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]-10);
				glTexCoord2f(tx,      ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]-15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/37.0));    glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]+10);
				glTexCoord2f(tx+(1.0/40.0), ty+(1.0/20.0)); glVertex2i(b[0][i].pos[0]+15, b[0][i].pos[1]-10);
		}
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBegin(GL_QUADS);
		//glColor3ub(100,100,255);
		//glVertex2i(b[0][i].pos[0]+2,b[0][i].pos[1]+2);
		//glVertex2i(b[0][i].pos[0]+2,b[0][i].pos[1]-2);
		//glVertex2i(b[0][i].pos[0]-2,b[0][i].pos[1]-2);
		//glVertex2i(b[0][i].pos[0]-2,b[0][i].pos[1]+2);
		//glEnd();
	}
	for(int i=0;i<bullets2;i++){
		b[1][i].pos[0]+= b[1][i].vel;
		float distx = b[1][i].pos[0]-p1[0];
		float disty = b[1][i].pos[1]-p1[1];
		float pdistx = b[0][i].pos[0]-p2[0];
		if(distx<0)
			distx=-distx;
		if(pdistx<0)
			pdistx=-pdistx;
		if(disty<0)
			disty=-disty;
		if(distx<w/2&&disty<h&&health2>0&&health1>0){
			bullets2--;
			for(int j=i;j<bullets2;j++){
				b[1][j] = b[1][j+1];
			}
			hit += 1;
		}
		if(b[1][i].pos[0]>xres||b[1][i].pos[0]<0){
			bullets2--;
			for(int j=i;j<bullets2;j++){
				b[1][j] = b[1][j+1];
			}
		}
		glEnd();
	}
	return hit;
}
	
		
			
			
			
	
	
double oobillion = 1.0/1e9;
double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}

double mTimer1()
{
	static double timeT = 0.0;
	struct timespec start, end;
	int x=0;
	clock_gettime(CLOCK_REALTIME, &start);
		x = x/50;
		x = x*x;
	clock_gettime(CLOCK_REALTIME, &end);
	timeT += timeDiff(&start, &end);
	return timeT;
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
	timeT += timeDiff(&start, &end);
		return timeT;
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
	timeT += timeDiff(&start, &end);
		return timeT;
}
	
int punch(Vec att,float ah,float aw, Vec vic, int health){
	float distx = att[0]-vic[0];
	if(distx<0)
		distx=-distx;
	float disty = (att[1]+ah/6)-vic[1];
	if(disty<0)
		disty=-disty;
	if(distx<aw*2&&disty<ah){
		return health-5;
	}
	return health;
}
	





	
	

