
#ifndef CMODULE_H
#define CMODULE_H

#include "imodule.h"

class CModule
{
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
};

#endif