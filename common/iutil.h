/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef IUTIL_H
#define IUTIL_H

#include "imodule.h"
#include "keyval.h"

class Buffer {
public:
    char *data;
    Buffer() { data = NULL; };
    ~Buffer() {
        if (data != NULL) {
            delete data;
        }
    };
    
    void allocate( long size ) {
        if (data != NULL) {
            delete data;
        }
        data = new char[size];
    };
    operator const char*() { return data; };
    operator char*() { return data; };
    
};

class IUtilModule : public IModule {
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
    
    virtual void getFileExtension( char *c, const char *filename ) = 0;
    virtual void getFileName( char *c, const char *filename ) = 0;
    virtual void getFileNameNoExtension( char *c, const char *filename ) = 0; // name without extension.
    virtual bool wildcardMatch( const char *wildcard, const char *match ) = 0;
    virtual KeyValue *parseKeyValue( char *data ) = 0;
    virtual KeyValue *parseKeyValue( Buffer *buffer ) = 0;
};

extern IUtilModule *util;

#endif