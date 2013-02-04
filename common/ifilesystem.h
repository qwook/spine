/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include "imodule.h"
#include "iutil.h"
#include <string>
#include <vector>

class File {
private:
    FILE *file;
public:
    File() : file(NULL) {};
    File(const char *filename) : file(NULL) {
        open(filename);
    };
    void open(const char *filename) {
        // if file is open, close it.
        if (file != NULL) {
            fclose(file);
            file = NULL;
        }
        file = fopen(filename, "rb");
    }
    void close() {
        if (file != NULL) {
            fclose(file);
            file = NULL;
        }
    }
    bool isValid() {
        return (file != NULL);
    }
    long getSize() {
        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        rewind(file);
        return length;
    }
    void readBuffer( Buffer *buffer, long length = -1 ) {
        if (length == -1) {
            fseek(file, 0, SEEK_END);
            length = ftell(file);
            rewind(file);
        }
        
        buffer->allocate(length + 1);
        fread(buffer->data, 1, length, file);
        buffer->data[length] = '\0';
    };
    void release() {
        close();
        delete this;
    }
};

typedef std::vector<std::string> DirectoryList;

class IFileSystemModule : public IModule {
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
    
    virtual File *open(const char *filename) = 0;
    virtual DirectoryList getDirectoryList(const char *dirname) = 0;
    virtual bool isFile(const char *filename) = 0;
    virtual bool isDirectory(const char *dirname) = 0;
    virtual bool isValid(const char *filename) = 0;
};

extern IFileSystemModule *filesystem;

#endif