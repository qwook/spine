/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef IGRAPHICS_H
#define IGRAPHICS_H

#include "imodule.h"
#include "imath.h"

class Font;

typedef struct {
    unsigned int mat;
    unsigned int width;
    unsigned int height;
} Material;

class Color {
public:
    Color(char r = 255, char g = 255, char b = 255, char a = 255)
    : r(r), g(g), b(b), a(a) {};
    char r, g, b, a;
};

class IGraphicsModule : public IModule {
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
    virtual void initDriver( short width, short height ) = 0;
    virtual void update() = 0;
    virtual bool isWindowOpen() = 0;
    
    virtual Material loadMaterial(const char *matName) = 0;
    virtual Material useMaterial(const char *matName) = 0;
    virtual Material useMaterial(unsigned int mat) = 0;
    
    virtual Font *loadFont(const char *fontFile, int size) = 0;
    virtual void drawText( Font *font, const char *text, Vector2Df pos ) = 0;
    
    virtual unsigned int useProgram(const char *programName) = 0;
    virtual unsigned int useProgram(unsigned int program) = 0;
    virtual void setProgramVar1f(unsigned int program, const char *var, float i) = 0;
    virtual void setProgramVar1i(unsigned int program, const char *var, int i) = 0;
    
    virtual void clear() = 0;
    
    virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) = 0;
    
    virtual void drawQuad(float x, float y, float width, float height) = 0;
    virtual void drawQuadUV(float x, float y, float width, float height, float u1, float u2, float v1, float v2) = 0;
    
    virtual void setCursorVisible( bool visible ) = 0;
    virtual Vector2Df getCursorPos() = 0;
    
    virtual void pushScissor(bool enable, int x = 0, int y = 0, int width = 0, int height = 0, bool offset = false) = 0;
    virtual void popScissor() = 0;
    
    virtual short getWidth() = 0;
    virtual short getHeight() = 0;
};

extern IGraphicsModule *graphics;

#endif