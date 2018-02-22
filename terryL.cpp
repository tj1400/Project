//3350
//author: Terry langhardt
//date: 2/22/18
//
//project file for movement using the walk framework.
//#include "walk.cpp"

void walk()
{
    g.walk = 1;
    g.hold = 1;
}

void phy2()
{
//man is walking...
                //when time is up, advance the frame.
                timers.recordTime(&timers.timeCurrent);
                double timeSpan = timers.timeDiff(&timers.walkTime, &timers.timeCurrent);
                if (timeSpan > .01) {
                        //advance
                        ++g.walkFrame;
                        if (g.walkFrame >= 16)
                                g.walkFrame -= 16;
                        timers.recordTime(&timers.walkTime);
                }
}

void walkBack()
{
    g.walk_back = 1;
    g.hold = 1;
}

void jump()
{
}
