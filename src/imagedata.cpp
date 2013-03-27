/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "igraphics.h"
#include "imagedata.h"
#include "imath.h"
#include <algorithm>

using namespace std;

ImageData::ImageData( uint width, uint height, char *src ) : width(width), height(height) {
    data = new char[width * height * 4];
    if (src != 0) {
        memcpy(data, src, sizeof(char) * width * height * 4);
    }
    
    Rect node = {1,1,(int)(width-2)};
    nodes.push_back(node);
}

ImageData::~ImageData() {
    delete data;
}

void ImageData::clear(Color col) {
    char *dataptr = &data[0];
    for (uint i = 0; i < width * height; i++ ) {
        *dataptr++ = col.r;
        *dataptr++ = col.g;
        *dataptr++ = col.b;
        *dataptr++ = col.a;
    }
}

void ImageData::copyRegion( ImageData *imgdat, uint x_offset, uint y_offset ) {
    uint min_height = min(imgdat->height + y_offset, height);
    uint min_width = min(imgdat->width + x_offset, width);
    for (uint y = y_offset; y < min_height; y++ ) {
        for (uint x = x_offset; x < min_width; x++ ) {
            setPixel(x, y, imgdat->getPixel(x-x_offset, y-y_offset));
        }
    }
}

int ImageData::atlasFit( size_t index, int width, int height ) {
    Rect node = nodes[index];
    int x = node.x;
    int y;
    int width_left = width;
    
    size_t i = index;
    if ((x + width) > (this->width-1)) {
        return -1;
    }
    
    y = node.y;
    while (width_left > 0) {
        node = nodes[i];
        if (node.y > y) {
            y = node.y;
        }
        if ((y + height) > (this->height-1)) {
            return -1;
        }
        width_left -= node.width;
        ++i;
    }
    return y;
}

Vector2Df ImageData::getRegion( int width, int height ) {
    int y;
    int best_height, best_width;
    Rect node, prev, next;
    Rect region = {0, 0, (int)width};
    size_t i, best_index;
    
    best_height = 2147483647;
    best_width = 2147483647;
    best_index = -1;
    bool any = false;
    
    for (i = 0; i < nodes.size(); ++i) {
        any = true;
        y = atlasFit( i, width, height );
        if ( y >= 0 ) {
            node = nodes[i];
            if (((y + height) < best_height) ||
                ( ((y + height) == best_height) &&
                 (node.width < best_width)) ) {
                    best_height = y + height;
                    best_index = i;
                    best_width = node.width;
                    region.x = node.x;
                    region.y = y;
                }
        }
    }
    
    if (best_index == -1) {
        return Vector2Df(-1, -1);
    }
    
    node = Rect();
    node.x = region.x;
    node.y = region.y + height;
    node.width = width;
    nodes.insert(nodes.begin() + best_index, node);
    
    for (i = best_index+1; i < nodes.size(); i++) {
        node = nodes[i];
        prev = nodes[i-1];
        
        if (node.x < (prev.x + prev.width)) {
            int shrink = prev.x + prev.width - node.x;
            node.x += shrink;
            node.width -= shrink;
            nodes[i] = node;
            if (node.width <= 0)
            {
                nodes.erase(nodes.begin() + i);
                --i;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    // merge atlas.
    for (i = 0; i < nodes.size()-1; i++ ){
        node = nodes[i];
        next = nodes[i+1];
        if ( node.y == next.y )
        {
            node.width += next.width;
            nodes[i] = node;
            nodes.erase(nodes.begin() + (i+1));
            --i;
        }
    }
    
    return Vector2Df(region.x, region.y);
}

void ImageData::setPixel(uint x, uint y, Color col) {
    char *dataptr = &data[(x + y*width)*4];
    *dataptr++ = col.r;
    *dataptr++ = col.g;
    *dataptr++ = col.b;
    *dataptr++ = col.a;
}

Color ImageData::getPixel(uint x, uint y) {
    char *dataptr = &data[(x + y*width)*4+3];
    return Color(*(dataptr++),*(dataptr++),*(dataptr++),*(dataptr++));
}

void ImageData::resize(ResizeMode mode, uint nwidth, uint nheight) {
    if ( mode == RESIZE_MODE_BILLINEAR ) {
        resizeBillinear(nwidth, nheight);
    }
}

void ImageData::resizeBillinear(uint nwidth, uint nheight) {
    char *a, *b, *c, *d;
    char *dst = new char[nwidth * nheight * 4];
    char *dst_ptr = &dst[0];
    uint x, y, index;
    float x_ratio = ((float)(width-1))/nwidth;
    float y_ratio = ((float)(height-1))/nheight;
    float x_diff, y_diff;
    for (uint i = 0; i < nheight; i++) {
        for (uint j = 0; j < nwidth; j++) {
            x = (int)floor(x_ratio * j);
            y = (int)floor(y_ratio * i);
            x_diff = (x_ratio * j) - (float)x;
            y_diff = (y_ratio * i) - (float)y;
            index = (y*width + x) * 4;
            a = &data[index];
            b = &data[index+4];
            c = &data[index+width*4];
            d = &data[index+width*4+4];
            
            *dst_ptr++ = ((int)*a)*(1-x_diff)*(1-y_diff) + ((int)*b)*(x_diff)*(1-y_diff) +
            ((int)*c)*(y_diff)*(1-x_diff)   + ((int)*d)*(x_diff*y_diff);
            
            a++; b++; c++; d++;
            *dst_ptr++ = ((int)*a)*(1-x_diff)*(1-y_diff) + ((int)*b)*(x_diff)*(1-y_diff) +
            ((int)*c)*(y_diff)*(1-x_diff)   + ((int)*d)*(x_diff*y_diff);
            
            a++; b++; c++; d++;
            *dst_ptr++ = ((int)*a)*(1-x_diff)*(1-y_diff) + ((int)*b)*(x_diff)*(1-y_diff) +
            ((int)*c)*(y_diff)*(1-x_diff)   + ((int)*d)*(x_diff*y_diff);
            
            a++; b++; c++; d++;
            *dst_ptr++ = ((int)*a)*(1-x_diff)*(1-y_diff) + ((int)*b)*(x_diff)*(1-y_diff) +
            ((int)*c)*(y_diff)*(1-x_diff)   + ((int)*d)*(x_diff*y_diff);
            
        }
    }
    
    delete data;
    data = dst;
    
    width = nwidth;
    height = nheight;
}


void Compare( Vector2Df *grid, int x, int y, int offsetx, int offsety, unsigned int width, unsigned int height ) {
    
    Vector2Df pos;
    if (x + offsetx < 0 || x + offsetx >= width || y + offsety < 0 || y + offsety >= height) {
        pos = Vector2Df(1000000, 1000000);
    } else {
        pos = Vector2Df(grid[(x + offsetx) + ((y + offsety) * width)]);
    }
    
    pos.x += offsetx;
    pos.y += offsety;
    
    if (pos.getLengthSqr() < grid[x + y * width].getLengthSqr()) {
        grid[x + y * width] = pos;
    }
}

void GenerateGrid( Vector2Df *grid, unsigned int width, unsigned int height ) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Compare(&grid[0], x, y, -1,  0, width, height);
            Compare(&grid[0], x, y,  0, -1, width, height);
            Compare(&grid[0], x, y, -1, -1, width, height);
            Compare(&grid[0], x, y,  1, -1, width, height);
        }
        
        for (int x = width - 1; x >= 0; --x) {
            Compare(&grid[0], x, y, 1, 0, width, height);
        }
    }
    
    for (int y = height - 1; y >= 0; --y) {
        for (int x = width - 1; x >= 0; --x) {
            Compare(&grid[0], x, y, 1,  0, width, height);
            Compare(&grid[0], x, y,  0, 1, width, height);
            Compare(&grid[0], x, y, -1, 1, width, height);
            Compare(&grid[0], x, y,  1, 1, width, height);
        }
        
        for (int x = 0; x < width; x++) {
            Compare(&grid[0], x, y, -1, 0, width, height);
        }
    }
    
}

void GenerateSDF(unsigned char *data, unsigned long width, unsigned long height) {
    Vector2Df grid1[width * height];
    Vector2Df grid2[width * height];
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float a = data[(x + y * width) * 4 + 3];
            if ( a > 127 ) {
                grid1[x + y * width] = Vector2Df (0, 0);
                grid2[x + y * width] = Vector2Df (1000000, 1000000);
            } else {
                grid2[x + y * width] = Vector2Df (0, 0);
                grid1[x + y * width] = Vector2Df (1000000, 1000000);
            }
        }
    }
    
    GenerateGrid(&grid1[0], width, height);
    GenerateGrid(&grid2[0], width, height);
    
    float spread = min(width, height) / (1 << 3);
    
    float minVal = -spread;
    float maxVal = spread;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
			float dist1 = grid1[x + y * width].getLength();
			float dist2 = grid2[x + y * width].getLength();
            
            float dist = dist1 - dist2;
            
			// Clamp and scale it, just for display purposes.
			float c = dist < 0
            ? -128 * (dist - minVal) / minVal
            : 128 + 128 * dist / maxVal;
			if ( c < 0 ) { c = 0; }
			if ( c > 255 ) { c = 255; }
            
            unsigned char d = floor(c);
            
            data[(x + y * width) * 4] = d;
            data[(x + y * width) * 4 + 1] = d;
            data[(x + y * width) * 4 + 2] = d;
            data[(x + y * width) * 4 + 3] = 255;
        }
    }
}
