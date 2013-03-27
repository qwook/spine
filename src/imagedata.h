/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "imath.h"
#include <vector>
#include <algorithm>

#define uint unsigned int

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rect;

class ImageData {
private:
    void resizeBillinear( uint nwidth, uint nheight );
public:
    ImageData( uint width, uint height, char *src = 0 );
    ~ImageData();
    
    typedef std::vector<Rect> NodeList;
    NodeList nodes;
    
    void setPixel( uint x, uint y, Color col );
    Color getPixel( uint x, uint y );
    
    void clear( Color col = Color(255, 255, 255) );
    void copyRegion( ImageData *imgdat, uint x_offset = 0, uint y_offset = 0 );
    Vector2Df getRegion( int width, int height );
    int atlasFit( size_t index, int width, int height );
    
    typedef enum {
        RESIZE_MODE_BILLINEAR
    } ResizeMode;
    
    void resize( ResizeMode mode, uint nwidth, uint nheight );
    
    uint width, height;
    char *data;
};

#endif