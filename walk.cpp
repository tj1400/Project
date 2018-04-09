//3350
//program: walk.cpp
//author:  Gordon Griesel
//date:    summer 2017 - 2018
//
//modified by: Terry Langhardt
//date: 2/22/18
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "fonts.h"
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <iostream>

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros 
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
#define JOY_DEV1 "/dev/input/js"
#define JOY_DEV2 "/dev/input/js"
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

extern void walk(int *walk,int *hold);
extern void walkBack(int *walk_back,int *hold);
extern void jump();
extern void showhealth(int,float,float,float,int,int);
extern void name1(Rect *r,int x, unsigned int c); 
extern int punch(Vec,float,float,Vec,int);
extern void shoot(Vec,int,int,int);
extern int moveBullets(Vec,Vec,float,float,int,int,int);
extern double timer();
extern double timer2();
extern double timer3();
extern double mTimer1();
extern double mTimer2();
extern double mTimer3();
bool setupJoystick();

//class Back{
    //public:
        //char name[200];
        //int width;
        //int height;
        //unsigned char *data;
        //~Back(){delete[] data;}
        //Back(){
            //// open image
            //FILE *fpi = fopen("images/background test.ppm", "r");
            //if(fpi){
                //char line[250];
                //fgets(line,250,fpi);
                //fgets(line,250,fpi);
                //while(line[0] == '#')
                    //fgets(line,250,fpi);
                //sscanf(line,"%i %i",&width,&height);
                //printf("dimensions: %i %i\n",width,height);
                //fgets(line,250,fpi);
                //data=new unsigned char[width * height * 3];
                //for(int i=0;i<width*height*3;i++)
                    //data[i]=fgetc(fpi);
                //fclose(fpi);
            //}
            //else{
                //printf("ERROR: opening input lab1.ppm\n");
                //exit(0);
            //}
            //unlink("images/background test.ppm");
        //}
//} back;


class Image {
public:
	int width, height;
	float v[3];
	unsigned char *data;
	~Image() {
		delete [] data;
		}
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		name[slen-4] = '\0';
		//printf("name **%s**\n", name);
		char ppmname[80];
		sprintf(ppmname,"%s.ppm", name);
		//printf("ppmname **%s**\n", ppmname);
		char ts[100];
		//system("convert eball.jpg eball.ppm");
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			while (line[0] == '#')
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		}
		else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		unlink(ppmname);
	}
public:
	Image() {
	    VecZero(v);
	}
};
Image img1[1] = {"images/walk.gif"};
Image img2[1] = {"images/walk.gif"};
Image img1b[1] = {"images/walkBack.gif"};
Image img2b[1] = {"images/walkBack.gif"};
Image imgback[1] = {"images/background.gif"};


//-----------------------------------------------------------------------------
//Setup timers
class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd;
	struct timespec timeCurrent[2];
	struct timespec walkTime[2];
	Timers() {
		physicsRate = 1.0 / 30.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
} timers;
//-----------------------------------------------------------------------------
class Player {
	public:
	int health;
	int h;
	int w;
	int walk;
	int walk_back;
	Vec position;
	//int jump;
	Vec vel;
	int name;
	int hold;
	int dir;
	int walkFrame;
	double delay;
	int jump;
	int num;
	int last[10][10];
	int joy;
	Player(){
		num=0;
		jump =0;
		joy=0;
		health=100;
		name=1;
		dir=1;
		h=50;
		w=h/2;
		jump=0;
		hold=0;
		walk=0;
		walk_back=0;
		walkFrame=0;
		delay=0.1;
		position[0]=400;
		position[1]=300;
		vel[0]=0.0;
		vel[1]=0.0;
	}
} p[2];
class Global {
public:
	int input1,input2;
	int done;
	int xres, yres;
	int name;
	int color;
	int colorc;
	int controlind[10];
	int controlsend[10];
	int playerCount;
	GLuint walkTexture;
	GLuint walkBackTexture;
	GLuint walkTexture2;
	GLuint walkBackTexture2;
	GLuint backgroundTexture;
	Vec box[20];
	Global() {
		playerCount=0;
		done=0;
		color=0;
		colorc=0;
		name=0;
		xres=800;
		yres=600;
		for (int i=0; i<20; i++) {
			box[i][0] = rnd() * xres;
			box[i][1] = rnd() * (yres-220) + 220.0;
			box[i][2] = 0.0;
		}
	}
} g;

class X11_wrapper {
public:
	Display *dpy;
	Window win;
	GC gc;
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setupScreenRes(g.xres, g.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
							StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		setTitle();
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setTitle() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Walk Cycle");
	}
	void setupScreenRes(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshapeWindow(int width, int height) {
		//window has been resized.
		setupScreenRes(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		setTitle();
	}
	void checkResize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshapeWindow(xce.width, xce.height);
		}
	}
	void setColor3i(int r, int g, int b) {
            unsigned long cref = 0L;
            cref += r;
            cref <<= 8;
            cref += g;
            cref <<= 8;
            cref += b;
            XSetForeground(dpy, gc, cref);
        }
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	void drawPixel(int x, int y){
		XDrawPoint(dpy,win,gc,x,y);
	}
	//void showImage(Back *back){
            //int offsetx=g.xres/2-back->width/2, offsety=g.yres/2-back->height/2;
            //for(int i=0;i<back->height;i++){
                //for(int j=0;j<back->width;j++){
                    //int r1 = back->data[i*back->width*3+j*3+0];
                    //int g1 = back->data[i*back->width*3+j*3+1];
                    //int b1 = back->data[i*back->width*3+j*3+2];
                    //setColor3i(r1,g1,b1);  
                    //drawPixel(j+offsetx,i+offsety);              
                    //}
                //}
            //}

} x11;

int joy_fd1[10], joy_fd2[10], *axis1={NULL}, *axis2={NULL}, num_of_axis1[10]={0}, num_of_axis2[10]={0}, num_of_buttons1[10]={0}, num_of_buttons2[10]={0}, x1, x2;
char *button1={NULL}, *button2={NULL}, name_of_joystick1[10][80], name_of_joystick2[10][80];
struct js_event js1;
struct js_event js2;

bool setupJoystick(int ind){
	if(ind == 0){
		int count = 0;
		char buffer[50];
		for(int i=0;i<10;i++){
			sprintf(buffer,"%s%d",JOY_DEV1,i);
			if( ( joy_fd1[i] = open( buffer , O_RDONLY)) == -1 )
			{
				printf( "Couldn't open joystick\n" );
				count++;
			}

			ioctl( joy_fd1[i], JSIOCGAXES, &num_of_axis1[i] );
			ioctl( joy_fd1[i], JSIOCGBUTTONS, &num_of_buttons1[i] );
			ioctl( joy_fd1[i], JSIOCGNAME(80), &name_of_joystick1[i] );
		}
		if(count == 10)
			return false;
		else{
			printf("name: 0: Keyboard \n");
			int k=0;
			int m=0;
			for(int i=0;i<10-count;i++){
				char name1[80]={"Wireless Controller"};
				char name2[80]={"Wireless Controller Motion Sensors"};
				int pass1 = 1;
				int pass2 = 1;
				for(int j=0;j<80;j++){
					if(name1[j] != name_of_joystick1[i][j]){
						pass1 = 0;
					}
					if(name2[j] != name_of_joystick1[i][j]){
						pass2 = 0;
					}
					
				}
				if(!pass2){
				printf("name %d: ",m+1);
				if(pass1){
					if(g.color==0){
						printf("(BLUE)");
					}
					if(g.color==1){
						printf("(RED)");
					}
					g.color++;
				}
				printf("%s \n",name_of_joystick1[i]);
			}
			else{
				k++;
				m--;
			}
			g.controlind[i] = i;
			g.controlsend[i] = k;
			k++;
			m++;
			}
			printf("Choose your controller player1: ");
			scanf("%d",&g.input1);
			g.input1 = g.controlsend[g.input1] -1;
			if(g.input1>10-count||g.input1<1){
				printf("Using keyboard now loser!!!\n");
				return false;
			}
		}
		axis1 = (int *) calloc( num_of_axis1[g.input1], sizeof( int ) );
		button1 = (char *) calloc( num_of_buttons1[g.input1], sizeof( char ) );

		printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
			, name_of_joystick1[g.input1]
			, num_of_axis1[g.input1]
			, num_of_buttons1[g.input1] );

		fcntl( joy_fd1[g.input1], F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
		return true;
	}	
	if(ind == 1){
		g.color=0;
		int count2 = 0;
		char buffer2[50];
		for(int i=0;i<10;i++){
			sprintf(buffer2,"%s%d",JOY_DEV2,i);
			if( ( joy_fd2[i] = open( buffer2 , O_RDONLY)) == -1 )
			{
				printf( "Couldn't open joystick\n" );
				count2++;
			}

			ioctl( joy_fd2[i], JSIOCGAXES, &num_of_axis2[i] );
			ioctl( joy_fd2[i], JSIOCGBUTTONS, &num_of_buttons2[i] );
			ioctl( joy_fd2[i], JSIOCGNAME(80), &name_of_joystick2[i] );
		}
		if(count2 == 10)
			return false;
		else{
			int k=0;
			int m=0;
			printf("name: 0: Keyboard\n");
			for(int i=1;i<10-count2;i++){
				char name1[80]={"Wireless Controller"};
				char name2[80]={"Wireless Controller Motion Sensors"};
				int pass1 = 1;
				int pass2 = 1;
				for(int j=0;j<80;j++){
					if(name1[j] != name_of_joystick2[i][j]){
						pass1 = 0;
					}
					if(name2[j] != name_of_joystick2[i][j]){
						pass2 = 0;
					}
					
				}
				if(!pass2){
				printf("name %d: ",m+1);
				if(pass1){
					if(g.color==0){
						printf("(BLUE)");
					}
					if(g.color==1){
						printf("(RED)");
					}
					g.color++;
				}
				printf("%s \n",name_of_joystick2[i]);
			}
			else{
				k++;
				m--;
			}
			g.controlind[i] = i;
			g.controlsend[i] = k;
			k++;
			m++;
		}
			printf("Choose your controller player2: ");
			scanf("%d",&g.input2);
			g.input2 = g.controlsend[g.input2] -1;
			if(g.input2>10-count2||g.input2<1||g.input1==g.input2){
				printf("Using keyboard now loser!!!\n");
				return false;
			}
		}
		axis2 = (int *) calloc( num_of_axis2[g.input2], sizeof( int ) );
		button2 = (char *) calloc( num_of_buttons2[g.input2], sizeof( char ) );

		printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
			, name_of_joystick2[g.input2]
			, num_of_axis2[g.input2]
			, num_of_buttons2[g.input2] );

		fcntl( joy_fd2[g.input2], F_SETFL, O_NONBLOCK );	/* use non-blocking mode */
		return true;
	}
	return true;
}

bool buttonDown1[2];
bool buttonDown2[2];

int checkJoystick(int ind){
	if(ind == 0){
			/* read the joystick state */
		read(joy_fd1[g.input1], &js1, sizeof(struct js_event));
	
			/* see what to do with the event */
		switch (js1.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_AXIS:
				if(axis1[js1.number] != js1.value){
					axis1[js1.number] = js1.value;
					return js1.number;
				}
				break;
			case JS_EVENT_BUTTON:
			if(button1[js1.number] != js1.value){
				button1[ js1.number ] = js1.value;
				return js1.number;
			}
			break;
		}
	}
	if(ind == 1){
			/* read the joystick state */
		read(joy_fd2[g.input2], &js2, sizeof(struct js_event));
	
			/* see what to do with the event */
		switch (js2.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_AXIS:
				if(axis2[js2.number] != js2.value){
					axis2[js2.number] = js2.value;
					return js2.number;
				}
				break;
			case JS_EVENT_BUTTON:
			if(button2[js2.number] != js2.value){
				button2[ js2.number ] = js2.value;
				return js2.number;
			}
			break;
		}
	}
	return -1;
}

void jsControl(int ind){
	if(ind == 0){
		if(js1.type == JS_EVENT_AXIS){
			if(js1.number == 6){
				if(axis1[6]<0){
					p[0].walk = 0;
					p[0].num = 1;
					p[0].vel[0] = -2.5;
					walkBack(&p[0].walk_back,&p[0].hold);
				}
				if(axis1[6]>0){
					p[0].walk_back = 0;
					p[0].num = 1;
					p[0].vel[0] = 2.5;
					walk(&p[0].walk,&p[0].hold);
				}
				if(axis1[6] == 0&&p[0].num==1&&p[0].hold==1){
					p[0].num=0;
					p[0].walk=0;
					p[0].walk_back=0;
					p[0].hold=0;
					p[0].vel[0]=0.0;
				}
			}
			if(js1.number == 0){
				if(axis1[0]<-1000){
					p[0].walk = 0;
					p[0].joy = 1;
					p[0].vel[0] = -2.5;
					walkBack(&p[0].walk_back,&p[0].hold);
				}
				if(axis1[0]>1000){
					p[0].walk_back = 0;
					p[0].joy = 1;
					p[0].vel[0] = 2.5;
					walk(&p[0].walk,&p[0].hold);
				}
				if(axis1[0] < 1000 && axis1[0] > -1000&&p[0].joy==1&&p[0].hold==1){
					p[0].joy=0;
					p[0].walk=0;
					p[0].walk_back=0;
					p[0].vel[0] = 0.0;
					p[0].hold=0;
				}
			}
		}
		if(js1.type == JS_EVENT_BUTTON){
			if(button1[0]>0.0){
				if(!buttonDown1[0]&&p[0].jump<2){
					buttonDown1[0] = true; 
					p[0].vel[1] = 8.0;
					p[0].jump++;
				}
			}
			else{
				buttonDown1[0] = false;
			}
			if(button1[3]>0.0){
				if(!buttonDown1[3]){
					buttonDown1[3] = true;
					p[1].health = punch(p[0].position,p[0].h,p[0].w,p[1].position,p[1].health);
				}
			}
			else{
				buttonDown1[3] = false;
			}
			if(button1[2]>0.0){
				if(!buttonDown1[2]){
					buttonDown1[2] = true;
					shoot(p[0].position,0,p[0].dir,p[0].w);
				}
			}
			else{
				buttonDown1[2] = false;
			}
			p[0].last[0][6]=axis1[6];
			p[0].last[0][0]=axis1[0];
		}
		printf("  \r");
		fflush(stdout);
	}
	if(ind == 1){
		if(js2.type == JS_EVENT_AXIS){
			if(js2.number == 6){
				if(axis2[6]<0){
					p[1].walk = 0;
					p[1].num = 1;
					p[1].vel[0] = -2.5;
					walkBack(&p[1].walk_back,&p[1].hold);
				}
				if(axis2[6]>0){
					p[1].walk_back = 0;
					p[1].num = 1;
					p[1].vel[0] = 2.5;
					walk(&p[1].walk,&p[1].hold);
				}
				if(axis2[6] == 0&&p[1].num==1&&p[1].hold==1){
					p[1].num=0;
					p[1].walk=0;
					p[1].walk_back=0;
					p[1].hold=0;
					p[1].vel[0]=0.0;
				}
			}
			if(js2.number == 0){
				if(axis2[0]<-1000){
					p[1].walk = 0;
					p[1].joy = 1;
					p[1].vel[0] = -2.5;
					walkBack(&p[1].walk_back,&p[1].hold);
				}
				if(axis2[0]>1000){
					p[1].walk_back = 0;
					p[1].joy = 1;
					p[1].vel[0] = 2.5;
					walk(&p[1].walk,&p[1].hold);
				}
				if(axis2[0] < 1000 && axis2[0] > -1000&&p[1].joy==1&&p[1].hold==1){
					p[1].joy=0;
					p[1].walk=0;
					p[1].walk_back=0;
					p[1].vel[0] = 0.0;
					p[1].hold=0;
				}
			}
		}
		if(js2.type == JS_EVENT_BUTTON){
			if(button2[0]>0.0){
				if(!buttonDown2[0]&&p[1].jump<2){
					buttonDown2[0] = true; 
					p[1].vel[1] = 8.0;
					p[1].jump++;
				}
			}
			else{
				buttonDown2[0] = false;
			}
			if(button2[3]>0.0){
				if(!buttonDown2[3]){
					buttonDown2[3] = true;
					p[0].health = punch(p[1].position,p[1].h,p[1].w,p[0].position,p[0].health);
				}
			}
			else{
				buttonDown2[3] = false;
			}
			if(button2[2]>0.0){
				if(!buttonDown2[2]){
					buttonDown2[2] = true;
					shoot(p[1].position,1,p[1].dir,p[1].w);
				}
			}
			else{
				buttonDown1[1] = false;
			}
			p[1].last[0][6]=axis2[6];
			p[1].last[0][0]=axis2[0];
		}
		printf("  \r");
		fflush(stdout);
	}
}

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);


int main(void)
{
	bool temp1;
	bool temp2;
	initOpengl();
	init();
	temp1=setupJoystick(0);
	temp2=setupJoystick(1);
	int done = 0;
	int js1Status,js2Status;
	while (!done){
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		if(temp1){
			while((js1Status=checkJoystick(0)) !=-1){
				jsControl(0);
			}
		}
		if(temp2){
			while((js2Status=checkJoystick(1)) !=-1){
				jsControl(1);
			}
		}
		physics();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	close(joy_fd1[g.input1]);
	close(joy_fd2[g.input2]);
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	void timer();
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	int w = img1[0].width;
	int h = img1[0].height;
	//
	//create opengl texture elements
	glGenTextures(1, &g.walkTexture);
	//-------------------------------------------------------------------------
	//silhouette
	//this is similar to a sprite graphic
	//
	glBindTexture(GL_TEXTURE_2D, g.walkTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *walkData = buildAlphaData(&img1[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData);
	//free(walkData);
	//unlink("./images/walk.ppm");
	glGenTextures(1, &g.walkBackTexture);
	//-------------------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, g.walkBackTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *walkBackData = buildAlphaData(&img1b[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkBackData);
	//-------------------------------------------------------------------------
	glGenTextures(1, &g.walkTexture2);
	glBindTexture(GL_TEXTURE_2D, g.walkTexture2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *walkData2 = buildAlphaData(&img2[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkData2);
	//-------------------------------------------------------------------------
	glGenTextures(1, &g.walkBackTexture2);
	glBindTexture(GL_TEXTURE_2D, g.walkBackTexture2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *walkBackData2 = buildAlphaData(&img2b[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, walkBackData2);
	//-------------------------------------------------------------------------
	glGenTextures(1, &g.backgroundTexture);
	glBindTexture(GL_TEXTURE_2D, g.backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *backgroundData = buildAlphaData(&imgback[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, backgroundData);
}

void init() {
	p[0].position[0]=200;
	p[1].position[0]=600;
	p[0].dir=1;
	p[1].dir=-1;
}

void checkMouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type == KeyRelease) {
		//if (key == XK_Shift_L || key == XK_Shift_R)
		//	shift = 0;
		if (key == XK_Right)
		{
		    p[0].hold = 0;
		    p[0].walk = 0;
		    p[0].vel[0]=0.0;
		}
		if (key == XK_Left)
		{
		    p[0].hold = 0;
		    p[0].walk_back = 0;
		    p[0].vel[0]=0.0;
		}
		if (key == XK_d)
		{
		    p[1].hold = 0;
		    p[1].walk = 0;
		    p[1].vel[0]=0.0;
		}
		if (key == XK_a)
		{
		    p[1].hold = 0;
		    p[1].walk_back = 0;
		    p[1].vel[0]=0.0;
		}
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_w:
			p[1].jump++;
			p[1].vel[1]=8.0;
			break;
		case XK_Left:
			walkBack(&p[0].walk_back,&p[0].hold);
			p[0].vel[0]=-1.5;
			break;
		case XK_Right:
			walk(&p[0].walk,&p[0].hold);
			p[0].vel[0]=1.5;
			break;
		case XK_a:
			walkBack(&p[1].walk_back,&p[1].hold);
			p[1].vel[0]=-1.5;
			break;
		case XK_d:
			walk(&p[1].walk,&p[1].hold);
			p[1].vel[0]=1.5;
			break;
		case XK_Up:
			p[0].jump++;
			p[0].vel[1]=8.0; 
			break;
		case XK_Down:
			break;
		case XK_h:
			p[0].health-=10;
			if(p[0].health<0)
				p[0].health=0;
			break;
		case XK_q:
			shoot(p[1].position,1,p[1].dir,p[1].w);
			break;
		case XK_equal:
			/*
			g.delay -= 0.005;
			if (g.delay < 0.005)
				g.delay = 0.005;
			*/
			break;
		case XK_minus:
			//g.delay += 0.005;
			break;
		case XK_n:
			if(g.name==0)
				g.name=1;
			else
				g.name=0;
			break;
		case XK_Escape:
			return 1;
			break;
	}
	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void physics(void)
{
	for(int i=0;i<2;i++){
	if (p[i].walk&&p[i].vel[0]!=0.0) {
		//man is walking...
		//when time is up, advance the frame.
		timers.recordTime(&timers.timeCurrent[i]);
		double timeSpan = timers.timeDiff(&timers.walkTime[i], &timers.timeCurrent[i]);
		if (timeSpan > p[i].delay) {
			//advance
			++p[i].walkFrame;
			if (p[i].walkFrame >= 16)
				p[i].walkFrame -= 16;
			timers.recordTime(&timers.walkTime[i]);
		}
		//for (int i=0; i<20; i++) {
			//g.box[i][0] -= 2.0 * (0.05 / p.delay);
			//if (g.box[i][0] < -10.0)
				//g.box[i][0] += g.xres + 10.0;
		//}
	}
	if (p[i].walk_back&&p[i].vel[0]<0.0) {
		//man is walking backwards...
		//when time is up, decrease the frame.
		timers.recordTime(&timers.timeCurrent[i]);
		double timeSpan = timers.timeDiff(&timers.walkTime[i], &timers.timeCurrent[i]);
		if (timeSpan > p[i].delay) {
			//decrease
			--p[i].walkFrame;
			if (p[i].walkFrame <= -1)
				p[i].walkFrame += 16;
			timers.recordTime(&timers.walkTime[i]);
		}
		//for (int i=0; i<20; i++) {
			//g.box[i][0] += 2.0 * (0.05 / p.delay);
			//if (g.box[i][0] < -10.0)
				//g.box[i][0] -= g.xres + 10.0;
		//}
	}
	p[i].position[1] += p[i].vel[1];
	if(p[i].vel[1] > -20)
		p[i].vel[1] -= .35;
	if(p[i].position[1]<100){
		p[i].position[1] = 100;
		p[i].jump=0;
	}
	if(p[i].position[0]<0+p[i].w/2){
		p[i].walk_back=0;
		p[i].vel[0]=0.0;
		p[i].position[0]=0.0+p[i].w/2;
	}
	if(p[i].position[0]>g.xres-p[i].w/2){
		p[i].walk=0;
		p[i].vel[0]=0.0;
		p[i].position[0]=g.xres-p[i].w/2;
	}
}
float distx=p[0].position[0]-p[1].position[0];
float disty=p[0].position[1]-p[1].position[1];
if(disty<0)
	disty=-disty;
if(distx<0)
	distx=-distx;
if(p[0].health>0&&p[1].health>0){
if(distx<=p[0].w&&disty<=p[0].h*2-15){
	if(distx<=p[0].w&&distx>p[0].w-5){
	if(p[0].position[0]>p[1].position[0]){
		p[0].position[0]+=.01;
		p[1].position[0]-=.01;
		if(p[0].vel[0]<0.0)
			p[0].vel[0]=0.0;
		if(p[1].vel[0]>0.0)
			p[1].vel[0]=0.0;
		}
	else{
		p[1].position[0]+=.01;
		p[0].position[0]-=.01;
		if(p[1].vel[0]<0.0)
			p[1].vel[0]=0.0;
		if(p[0].vel[0]>0.0)
			p[0].vel[0]=0.0;
		}
	}
	if(disty<=p[0].h*2-15&&disty>p[0].h*2-30){
	if(p[0].position[1]>p[1].position[1]){
		p[0].position[1]=p[1].position[1]+p[1].h*2-15;
		p[0].vel[0]=p[1].vel[0]+p[0].walk*2.5-p[0].walk_back*2.5;
		p[0].jump=0;
		if(p[0].vel[1]<0.0)
			p[0].vel[1]=0.0;
		if(p[1].vel[1]>0.0)
			p[1].vel[1]=0.0;
		}
	else{
		p[1].position[1]=p[0].position[1]+p[0].h*2-15;
		p[1].vel[0]=p[0].vel[0]+p[1].walk*2.5-p[1].walk_back*2.5;
		p[1].jump=0;
		if(p[1].vel[1]<0.0)
			p[1].vel[1]=0.0;
		if(p[0].vel[1]>0.0)
			p[0].vel[1]=0.0;
		}
	}
}
}
}

void render(void)
{
	if(p[0].position[0]-p[1].position[0]<0){
		p[0].dir=1;
	}
	else{
		p[0].dir=-1;
	}
	if(p[0].dir==1){
		p[1].dir=-1;
	}
	else{
		p[1].dir=1;
	}
	Rect r;
	//Clear the screen
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
//	x11.showImage(&back);
	glPushMatrix();
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, g.backgroundTexture);
	glEnable(GL_ALPHA_TEST);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(1,0);glVertex2i(0,600);
		glTexCoord2f(1,1);glVertex2i(0,0);
		glTexCoord2f(0,1);glVertex2i(800,0);
		glTexCoord2f(0,0);glVertex2i(800,600);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);

	for(int i=0;i<2;i++){
		if(p[i].health>0){
			p[i].position[0]+=p[i].vel[0];
			glPushMatrix();
			glColor3f(1.0, 1.0, 1.0);
			if(p[i].dir==-1)
				glBindTexture(GL_TEXTURE_2D, g.walkBackTexture);
			if(p[i].dir==1)
				glBindTexture(GL_TEXTURE_2D, g.walkTexture);
			//
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
			int ix = p[i].walkFrame % 8;
			int iy = 0;
			if (p[i].walkFrame >= 8)
				iy = 1;
			float tx = (float)ix / 8.0;
			float ty = (float)iy / 2.0;
			glBegin(GL_QUADS);
				glTexCoord2f(tx,      ty+.5); glVertex2i(p[i].position[0]-p[i].w, p[i].position[1]-p[i].h);
				glTexCoord2f(tx,      ty);    glVertex2i(p[i].position[0]-p[i].w, p[i].position[1]+p[i].h);
				glTexCoord2f(tx+.125, ty);    glVertex2i(p[i].position[0]+p[i].w, p[i].position[1]+p[i].h);
				glTexCoord2f(tx+.125, ty+.5); glVertex2i(p[i].position[0]+p[i].w, p[i].position[1]-p[i].h);
			glEnd();
			glPopMatrix();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			showhealth(p[i].health,p[i].position[0],p[i].position[1],p[i].h,g.name,i);
		}
	}
	int hit = moveBullets(p[0].position,p[1].position,p[1].w,p[1].h,g.xres,p[0].health,p[1].health);
	if(hit>0)
		printf("%d\n",hit);
	if (hit == 1||hit == 3)
		p[0].health -= 5;
	if (hit == 2||hit == 3)
		p[1].health -= 5;

	//
	unsigned int c = 0x00ffff44;
	r.bot = g.yres - 20;
	r.left = 10;
	r.center = 0;
	
	ggprint8b(&r, 16, c, "hold right arrow to walk right");
	ggprint8b(&r, 16, c, "hold left arrow to walk left");
	ggprint8b(&r, 16, c, "press n to toggle name");
	ggprint8b(&r, 16, c, "frame: %i", p[1].walkFrame);
	name1(&r,16, c);
	//ggprint8b(&r, 16, c, "time: %lf", timer());
	//ggprint8b(&r, 16, c, "time2: %lf", timer2());
	//ggprint8b(&r, 32, c, "time3: %lf", timer3());
	//ggprint8b(&r, 16, c, "maxTime1(no loop):             %lf", mTimer1());
	//ggprint8b(&r, 16, c, "maxTime2(for loop of 10):    %lf", mTimer2());
	//ggprint8b(&r, 16, c, "maxTime3(for loop of 1000): %lf", mTimer3());
}







