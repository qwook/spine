/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef CONCOMMAND
#define CONCOMMAND

#include "iutil.h"

typedef void(*concommand_fn)(const char *cmd, char *args[]);
typedef std::map<const char *, concommand_fn, cmp_str> ConCommandList;

extern ConCommandList concommands;

class CreateConsoleCommand {
public:
    CreateConsoleCommand(const char *name, concommand_fn callback) {
        printf("CMD! %s\n", name);
        char *args[5] = {"hello", "world!"};
        callback("yo", args);
    };
};

#define CONCOMMAND(name) \
void name##_concommand(const char *cmd, char* args[]); \
CreateConsoleCommand name##_ccmd( #name, name##_concommand ); \
void name##_concommand(const char *cmd, char* args[])

#endif
