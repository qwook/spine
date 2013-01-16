
#ifndef COS_H
#define COS_H

#include "ios.h"

class COSModule : public IOSModule
{
private:
    float delta;
    float lasttime;
    float storedtime;
    float starttime;
    float fps;
public:
    /*struct Date
    {
        int second, minute, hour, day, month, year;
        char month_string[11];
        char weekday[11];
    };*/

    virtual void init( IModuleManager* modulemanager );
    virtual void release();
    //virtual Date getDate();
    virtual float getCPUClock();
    virtual float getTime();
    virtual float getTimeInternal();
    virtual float getDelta();
    virtual float getFPS();
    virtual void update();
};

#endif