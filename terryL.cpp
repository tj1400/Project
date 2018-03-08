//3350
//author: Terry langhardt
//date: 2/22/18
//
//project file for movement using the walk framework.

#include "fonts.h"
#include <time.h>

Rect r;

void name1(Rect *r,int x, unsigned int c)
{
    ggprint8b(r, x, c, "Terry Langhardt");
}

void walk(int *walk,int *hold)
{
    *walk=1;
    *hold=1;
}

//void phy2(int &walkFrame,Timers timers)
//{
////man is walking...
                ////when time is up, advance the frame.
                //timers.recordTime(&timers.timeCurrent);
                //double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
                //if (timeSpan > .01) {
                        ////advance
                        //++walkFrame;
                        //if (walkFrame >= 16)
                                //g.walkFrame -= 16;
                        //timers.recordTime(&timers.walkTime);
                //}
//}

void walkBack(int *walk_back,int *hold)
{
    *walk_back=1;
    *hold=1;
}

void jump()
{
}

double timer()
{
    int x = 543;
    static double t = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i<25; i++)
    {
	if(x < 75)
	{
	    int z = 56 * 34 / 10;
	    int y = 45 * 88888 / 2;
	    x = 2 + 24 - 23 * 4;
	}
	x = x - 40;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t += (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec -
	    start.tv_nsec);
    
    return t/10000;
}

double timer2()
{
    int x = 543;
    static double t2 = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i<25; i++)
    {
	if(x < 75)
	{
	    int z = 56 * 34;
	    int y = 45 * 88888;
	    x = 2 + 24 - 23 * 4;
	}
	x = x - 40;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t2 += (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec -
	    start.tv_nsec);
    
    return t2/10000;
}

double timer3()
{
    int x = 543;
    int z = 0, y = 0;
    static double t3 = 0.0;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0; i < 25; i++)
    {
	for(int j = 0; j < 34; j++)
	{
	    z += 56;
	}
	for(int k = 0; k < 45; k++)
	{
	    y += 88888;
	}
	x = 2 + 24 - 23 * 4;
	
	x = x - 40;
    }
    clock_gettime(CLOCK_REALTIME, &end);
    t3 += (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec -
	    start.tv_nsec);
    
    return t3/10000;
}

