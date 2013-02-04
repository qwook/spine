/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "filesystem.h"
#include "dirent.h"

void CFileSystemModule::init( IModuleManager* modulemanager ) {
}

void CFileSystemModule::release() {
}

File *CFileSystemModule::open(const char *filename) {
    File *file = new File(filename);
    return file;
}

DirectoryList CFileSystemModule::getDirectoryList(const char *dirname) {
    DIR *dir = opendir(dirname);
    DirectoryList list;
    if (dir != NULL) {
        dirent *entity;
        while((entity = readdir(dir)) != NULL) {
            char temp[1024];
            strcpy(temp, dirname);
            char endchar = dirname[strlen(dirname)-1];
            if (endchar != '/' && endchar != '\\') {
                strcat(temp, "/");
            }
            strcat(temp, entity->d_name);
            list.push_back(std::string(temp));
        }
        closedir(dir);
    }
    return list;
}

bool CFileSystemModule::isFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    }
    return false;
}

bool CFileSystemModule::isDirectory(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (dir != NULL) {
        closedir(dir);
        return true;
    }
    return false;
}

bool CFileSystemModule::isValid(const char *filename) {
    return isFile(filename) || isDirectory(filename);
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    filesystem = new CFileSystemModule();
    return filesystem;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (CFileSystemModule *)filesystem;
}