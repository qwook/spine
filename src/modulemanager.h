
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

#include <map>
#include "imodulemanager.h"

using namespace std;

class CModuleManager : public IModuleManager
{
private:
    typedef IModule* LoadModuleFn();
    typedef void UnloadModuleFn();

    typedef struct
    {
        UnloadModuleFn *unloadModule;
        IModule *module;
        void *module_handle;
    } ModuleData;

    typedef map<const char*, ModuleData> ModuleMap;
    ModuleMap modules;
public:
    virtual ~CModuleManager();

    virtual IModule *loadModule( const char *name );
    virtual void unloadModule( const char *name );
    virtual void unloadAllModules();
};

#endif
