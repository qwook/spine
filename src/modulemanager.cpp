
#include <dlfcn.h>
#include "modulemanager.h"

#ifdef WIN32
const char *dll_exten = ".dll";
#endif
const char *dll_exten = ".dylib";

CModuleManager::~CModuleManager()
{
    unloadAllModules();
}

IModule *CModuleManager::loadModule( const char *name )
{
    if ( modules.find( name ) != modules.end() )
        return modules[name].module;

    int size = strlen( name ) + strlen( dll_exten );
    char *modulename = new char[size+1];
    strcpy( modulename, name );
    strcpy( modulename + strlen( name ), dll_exten );
    modulename[size] = '\0';

    void* module_handle = dlopen(modulename, RTLD_LAZY);
    if ( module_handle == NULL )
    {
        printf("Unable to load module!\n");
        fprintf(stderr, "%s", dlerror());
    }

    LoadModuleFn *loadModule = (LoadModuleFn *)dlsym(module_handle, "loadModule");
    UnloadModuleFn *unloadModule = (UnloadModuleFn *)dlsym(module_handle, "unloadModule");
    IModule *module = loadModule();

    ModuleData mod_data = {unloadModule, module, module_handle};
    modules[name] = mod_data;

    module->init(this);

    printf( "Loaded library: %s\n", name );

    delete modulename;
    return module;
}

void CModuleManager::unloadAllModules()
{
    for( ModuleMap::iterator i = modules.begin(); i != modules.end(); ++i )
    {
        unloadModule(i->first);
    }
}

void CModuleManager::unloadModule( const char *name )
{
    if ( modules.find( name ) == modules.end() )
        return;

    ModuleData mod_data = modules[name];

    mod_data.module->release();
    mod_data.unloadModule();
    modules.erase( name );
    dlclose( mod_data.module_handle );
}
