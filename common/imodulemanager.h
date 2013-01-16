
#ifndef IMODULEMANAGER_H
#define IMODULEMANAGER_H

#include "imodule.h"

class IModuleManager
{
public:
    virtual IModule *loadModule( const char *name ) = 0;
    virtual void unloadModule( const char *name ) = 0;
    virtual void unloadAllModules() = 0;
};

extern IModuleManager *modulemanager;

#endif
