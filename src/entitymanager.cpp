
#include <iostream>
#include <time.h>

#include "os.h"

static const int NUM_FPS_SAMPLES = 64;
float fpsSamples[NUM_FPS_SAMPLES];
int currentSample = 0;

float COSModule::getCPUClock()
{
    return (float)clock();
}

float COSModule::getTimeInternal()
{
    return getCPUClock()/CLOCKS_PER_SEC;
}

float COSModule::getTime()
{
    return storedtime;
}

float COSModule::getDelta()
{
    return delta;
}

float COSModule::getFPS()
{
    return fps;
}

void COSModule::update()
{
    float realtime = getTimeInternal();
    storedtime = realtime - starttime;
    delta = storedtime - lasttime;
    lasttime = storedtime;

    fps -= fpsSamples[0] / NUM_FPS_SAMPLES;

    memmove( &fpsSamples[0], &fpsSamples[1], sizeof(fpsSamples) - sizeof(*fpsSamples) );
    if ( delta == 0 )
        fpsSamples[NUM_FPS_SAMPLES-1] = 0;
    else
    {
        fpsSamples[NUM_FPS_SAMPLES-1] = 1.0f / (delta * 1000);
        fps += (1.0f / (delta * 1000)) / NUM_FPS_SAMPLES;
    }
}

void COSModule::init( IModuleManager* modulemanager )
{
    delta = 0;
    storedtime = 0;
    lasttime = 0;
    starttime = getTimeInternal();
    fps = 0;
    for(int i=0; i<NUM_FPS_SAMPLES; i++)
        fpsSamples[i] = 0;
}

void COSModule::release()
{
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    os = new COSModule();
    return os;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (COSModule *)os;
}