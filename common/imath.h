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

template<class T> class Vector2D {
public:
    T x, y;

    Vector2D( T x = 0, T y = 0 ) : x(x), y(y) {};
    Vector2D<T> operator+ (Vector2D<T> rhs);
    Vector2D<T> operator- (Vector2D<T> rhs);
    Vector2D<T> operator* (float scalar);
    Vector2D<T> operator/ (float scalar);

    Vector2D<T> getRotated( float angle );
    Vector2D<T> getNormal();
    T getDistance(Vector2D<T> rhs) const;
    T getDot(Vector2D<T> rhs) const;
    T getLength() const;
    T getAngle() const;
};

typedef Vector2D<float> Vector2Df;


template<class T>
Vector2D<T> Vector2D<T>::operator+(Vector2D<T> rhs) {
    return Vector2D<T>( x + rhs.x, y + rhs.y );
}

template<class T>
Vector2D<T> Vector2D<T>::operator-(Vector2D<T> rhs) {
    return Vector2D<T>( x - rhs.x, y - rhs.y );
}

template<class T>
Vector2D<T> Vector2D<T>::operator*(float scalar) {
    return Vector2D<T>( x * scalar, y * scalar );
}

template<class T>
Vector2D<T> Vector2D<T>::operator/(float scalar) {
    return Vector2D<T>( x / scalar, y / scalar );
}

template<class T>
Vector2D<T> Vector2D<T>::getRotated( float angle ) {
    float rad = degtorad(angle);
    
    float c = cos(rad);
    float s = sin(rad);
    
    return Vector2D<T>( x*c - y*s, x*s + y*c );
}

template<class T>
Vector2D<T> Vector2D<T>::getNormal() {
    int len = getLength();
    if ( len == 0 ) {
        return 0;
    }
    
    return *this / len;
}

template<class T>
T Vector2D<T>::getDistance(Vector2D<T> rhs) const {
    return ( rhs - *this ).getLength();
}

template<class T>
T Vector2D<T>::getDot(Vector2D<T> rhs) const {
    return x * rhs.x + y * rhs.y;
}

template<class T>
T Vector2D<T>::getLength() const {
    return sqrt( getDot(*this) );
}

template<class T>
T Vector2D<T>::getAngle() const {
    return radtodeg(acos(x));
}


#endif