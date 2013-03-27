/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef IMATH_H
#define IMATH_H

#include <cmath>

static const double MATH_PI = 3.14159265358979323846;
#define radtodeg(rad) ( rad * (180/MATH_PI) )
#define degtorad(deg) ( deg * (MATH_PI/180) )

/*class Rect {
public:
    float x, y, width, height;
    Rect( float x = 0, float y = 0, float width = 0, float height = 0 ) : x(x), y(y), width(width), height(height) {};
};*/

class Vector2Df {
public:
    float x, y;
    
    Vector2Df( float x = 0, float y = 0 ) : x(x), y(y) {};
    Vector2Df operator+ (Vector2Df rhs);
    Vector2Df operator- (Vector2Df rhs);
    Vector2Df operator* (float scalar);
    Vector2Df operator/ (float scalar);
    
    Vector2Df getRotated( float angle );
    Vector2Df getNormal();
    float getDistance(Vector2Df rhs) const;
    float getDot(Vector2Df rhs) const;
    float getLength() const;
    float getLengthSqr() const;
    float getAngle() const;
};

#endif