/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

/*

 Don't over-comment! // this is a comment
 Don't waste vertical space! if (true) do(everything); here(); yep();
 Wrap everything in brackets! {{{{}}}}
 
*/

#include <iostream>
#include <map>

#include "modulemanager.h"
#include "imath.h"
#include "ios.h"
#include "ientitymanager.h"
#include "iclient.h"
#include "igraphics.h"
#include "iutil.h"
#include "ifilesystem.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

using namespace std;

IModuleManager *modulemanager;
IOSModule *os;
IEntityManagerModule *entitymanager;
IClientModule *client;
IGraphicsModule *graphics;
IUtilModule *util;
IFileSystemModule *filesystem;

#include <vector>

// let there be light
int main( int argc, const char **argv ) {
    modulemanager = new CModuleManager;
    
#ifdef __APPLE__2
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
#endif
    
    util =
        (IUtilModule *)modulemanager->
        loadModule("util");
    filesystem =
        (IFileSystemModule *)modulemanager->
        loadModule("filesystem");
    graphics =
        (IGraphicsModule *)modulemanager->
        loadModule("graphics");
    os =
        (IOSModule *)modulemanager->
        loadModule("os");
    entitymanager =
        (IEntityManagerModule *)modulemanager->
        loadModule("entitymanager");
    client =
        (IClientModule *)modulemanager->
        loadModule("client");
    
    graphics->initDriver();
    
    float lastFrameTime = os->getTimeInternal();
    const float FPSLimit = 90; // TODO: Make this an option.
    while (graphics->isWindowOpen()) {
        
        os->update();
        entitymanager->update();
        client->update();
        
        //printf("%f\n", os->getFPS());
        
        // If we have an FPS limit, then don't render on certain frames.
        float currentTime = os->getTimeInternal();
        if ( (currentTime - lastFrameTime) < ( 0.001 / FPSLimit ) ) {
            continue;
        }
        lastFrameTime = currentTime;
        
        graphics->update();
        
    }

    delete (CModuleManager *)modulemanager;
    return 0;
}