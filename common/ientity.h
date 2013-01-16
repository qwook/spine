
#ifndef IOS_H
#define IOS_H

#include "imodule.h"

class IOSModule : public IModule
{
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
    virtual float getCPUClock() = 0;
    virtual float getTime() = 0;
    virtual float getTimeInternal() = 0;
    virtual float getDelta() = 0;
    virtual float getFPS() = 0;
    virtual void update() = 0;
};

extern IOSModule *os;

#endif