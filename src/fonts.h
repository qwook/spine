/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef SPINE_FONTS_H
#define SPINE_FONTS_H

#include <iostream>
#include "igraphics.h"
#include "imath.h"

struct Glyph {
    int x;
    int y;
    int width;
    int height;
    int advance;
    int offset_x;
    int offset_y;
};

class Font {
    Material material;
    Glyph glyphlist[255];
public:
    Font( const char *name, unsigned int size = 18, unsigned int width = 512, unsigned int height = 512 );
    void print() { printf("hello!\n"); };
    void drawGlyph( char charcode, Vector2Df pos );
    void drawString( const char *string, Vector2Df pos );
    Vector2Df getStringSize( char *string );
};

#endif