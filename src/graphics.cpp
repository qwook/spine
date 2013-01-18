/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include <iostream>
#include <sys/time.h>
#include <sys/resource.h>

#include "os.h"

static const int NUM_FPS_SAMPLES = 64;
float fpsSamples[NUM_FPS_SAMPLES];
int currentSample = 0;

float COSModule::getCPUClock() {
    return (float)clock();
}

float COSModule::getTimeInternal() {
    return getCPUClock()/CLOCKS_PER_SEC;
}

float COSModule::getTime() {
    return storedtime;
}

float COSModule::getDelta() {
    return delta;
}

float COSModule::getFPS() {
    return fps;
}

float COSModule::getRAMUsage() {
    // still trying to figure out how to find out CPU usage
    // or at least some sort of performance measurement.
    
    // for windows: GetProcessMemoryInfo()
    
    //rusage data;
    //getrusage(RUSAGE_SELF, &data);
    
    //return data.ru_msgrcv;
    return 0.0f;
}

void COSModule::update() {
    float realtime = getTimeInternal();
    storedtime = realtime - starttime;
    delta = storedtime - lasttime;
    lasttime = storedtime;

    fps -= fpsSamples[0] / NUM_FPS_SAMPLES;

    memmove( &fpsSamples[0], &fpsSamples[1], sizeof(fpsSamples) - sizeof(*fpsSamples) );
    if ( delta == 0 ) {
        fpsSamples[NUM_FPS_SAMPLES-1] = 0;
    } else {
        fpsSamples[NUM_FPS_SAMPLES-1] = 1.0f / (delta * 1000);
        fps += (1.0f / (delta * 1000)) / NUM_FPS_SAMPLES;
    }
}

void COSModule::init( IModuleManager* modulemanager ) {
    delta = 0;
    storedtime = 0;
    lasttime = 0;
    starttime = getTimeInternal();
    fps = 0;
    for(int i=0; i<NUM_FPS_SAMPLES; i++) {
        fpsSamples[i] = 0;
    }
}

void COSModule::release() {
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    os = new COSModule();
    return os;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (COSModule *)os;
}