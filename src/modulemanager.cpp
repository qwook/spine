/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/

#include <dlfcn.h>
#include "modulemanager.h"

#ifdef WIN32
const char *dll_exten = ".dll";
#endif
const char *dll_exten = ".dylib";

CModuleManager::~CModuleManager() {
    unloadAllModules();
}

IModule *CModuleManager::loadModule( const char *name ) {
    if ( modules.find( name ) != modules.end() ) {
        return modules[name].module;
    }

    long size = strlen( name ) + strlen( dll_exten );
    char *modulename = new char[size+1];
    strcpy( modulename, name );
    strcpy( modulename + strlen( name ), dll_exten );
    modulename[size] = '\0';

    void* module_handle = dlopen(modulename, RTLD_LAZY);
    if ( module_handle == NULL ) {
        printf("Unable to load module!\n");
        fprintf(stderr, "%s", dlerror());
        exit(1);
    }

    LoadModuleFn *loadModule =
    (LoadModuleFn *)dlsym(module_handle, "loadModule");
    UnloadModuleFn *unloadModule =
    (UnloadModuleFn *)dlsym(module_handle, "unloadModule");
    
    IModule *module = loadModule();

    ModuleData mod_data = {unloadModule, module, module_handle};
    modules[name] = mod_data;

    module->init(this);

    printf( "Loaded library: %s\n", name );

    delete modulename;
    return module;
}

void CModuleManager::unloadAllModules() {
    ModuleMap::iterator i = modules.begin();
    while( i != modules.end() )
    {
        printf("UNLOAD: %s\n",i->first);
        modules.erase(i);
        i = modules.begin();
    }
}

void CModuleManager::unloadModule( const char *name ) {
    if ( modules.find( name ) == modules.end() ) {
        return;
    }

    ModuleData mod_data = modules[name];

    mod_data.module->release();
    mod_data.unloadModule();
    modules.erase( name );
    dlclose( mod_data.module_handle );
}
