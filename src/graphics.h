/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include "igraphics.h"
#include "imagedata.h"

#include <vector>

class CGraphicsModule : public IGraphicsModule {
private:
    typedef struct Scissor {
        bool enable;
        int x;
        int y;
        int width;
        int height;
        bool offset;
        int ox;
        int oy;
        Scissor (bool enable, int x, int y, int width, int height, bool offset, int ox, int oy) : enable(enable), x(x), y(y), width(width), height(height), offset(offset), ox(ox), oy(oy) {};
    } Scissor;
    
    typedef std::vector<Scissor> ScissorList;
    ScissorList scissorStack;
    
    short width;
    short height;
    
    int offsetX;
    int offsetY;
    
public:

    virtual void init( IModuleManager* modulemanager );
    virtual void release();
    virtual void initDriver( short width, short height );
    virtual void update();
    virtual bool isWindowOpen();
    
    virtual Material loadMaterial(const char *matName, ImageData *data);
    virtual Material loadMaterial(const char *matName);
    virtual Material useMaterial(const char *matName);
    virtual Material useMaterial(unsigned int mat);
    
    virtual Font *loadFont(const char *fontFile, int size);
    virtual void drawText( Font *font, const char *text, Vector2Df pos );
    
    virtual unsigned int useProgram(const char *programName);
    virtual unsigned int useProgram(unsigned int program);
    virtual void setProgramVar1f(unsigned int program, const char *var, float i);
    virtual void setProgramVar1i(unsigned int program, const char *var, int i);

    virtual void clear();
    
    virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    
    virtual void drawQuad(float x, float y, float width, float height);
    virtual void drawQuadUV(float x, float y, float width, float height, float u1, float u2, float v1, float v2);
    
    virtual void setCursorVisible( bool visible );
    virtual Vector2Df getCursorPos();
    
    virtual void pushScissor(bool enable, int x = 0, int y = 0, int width = 0, int height = 0, bool offset = false);
    virtual void popScissor();
    
    virtual short getWidth();
    virtual short getHeight();
};

#endif