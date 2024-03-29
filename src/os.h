/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef COS_H
#define COS_H

#include "ios.h"

class COSModule : public IOSModule {
private:
    float delta;
    float lasttime;
    float storedtime;
    float starttime;
    float fps;
public:
    /*struct Date
    {
        int second, minute, hour, day, month, year;
        char month_string[11];
        char weekday[11];
    };*/

    virtual void init( IModuleManager* modulemanager );
    virtual void release();
    //virtual Date getDate();
    virtual float getCPUClock();
    virtual float getTime();
    virtual float getTimeInternal();
    virtual float getDelta();
    virtual float getFPS();
    virtual float getRAMUsage();
    virtual void update();
};

#endif