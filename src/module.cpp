
#include <iostream>

#include "module.h"

class IGraphicsModule : public IModule
{
public:
    virtual void init( IModuleManager* modulemanager );
    virtual void release();
};
IGraphicsModule *graphics;

void IGraphicsModule::init( IModuleManager* modulemanager )
{
    printf("yo\n");
}

void IGraphicsModule::release()
{
    printf("bye\n");
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    graphics = new IGraphicsModule();
    return graphics;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete graphics;
}