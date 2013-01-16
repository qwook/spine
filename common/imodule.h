
#ifndef IMODULE_H
#define IMODULE_H

#if defined( WIN32 )
#ifdef MINGW32
#define SPINE_DLLEXPORT __declspec( dllexport )
#define SPINE_DLLIMPORT __declspec( dllimport )
#else
#define SPINE_DLLEXPORT __stdcall
#define SPINE_DLLIMPORT __stdcall
#endif
#else
#define SPINE_DLLEXPORT
#define SPINE_DLLIMPORT
#endif

class IModuleManager;
class IModule
{
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
};

#endif