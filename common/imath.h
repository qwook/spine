
#ifndef IMATH_H
#define IMATH_H

#include <cmath>

static const double MATH_PI = 3.14159265358979323846;
#define radtodeg(rad) ( rad * (180/MATH_PI) )
#define degtorad(deg) ( deg * (MATH_PI/180) )

template<class T> class Vector2D
{
public:
    T x, y;

    Vector2D( T x = 0, T y = 0 ) : x(x), y(y) {};
    Vector2D<T> operator+ (Vector2D<T> rhs);
    Vector2D<T> operator- (Vector2D<T> rhs);
    Vector2D<T> operator* (float scalar);
    Vector2D<T> operator/ (float scalar);

    Vector2D<T> getRotated( float angle );
    Vector2D<T> getNormal();
    T getDistance(Vector2D<T> rhs);
    T getDotProduct(Vector2D<T> rhs);
    T getLength();
    T getAngle();
};

typedef Vector2D<float> Vector2Df;


template<class T>
Vector2D<T> Vector2D<T>::operator+(Vector2D<T> rhs)
{
    return Vector2D<T>( x + rhs.x, y + rhs.y );
}

template<class T>
Vector2D<T> Vector2D<T>::operator-(Vector2D<T> rhs)
{
    return Vector2D<T>( x - rhs.x, y - rhs.y );
}

template<class T>
Vector2D<T> Vector2D<T>::operator*(float scalar)
{
    return Vector2D<T>( x * scalar, y * scalar );
}

template<class T>
Vector2D<T> Vector2D<T>::operator/(float scalar)
{
    return Vector2D<T>( x / scalar, y / scalar );
}

template<class T>
Vector2D<T> Vector2D<T>::getRotated( float angle )
{
    float rad = degtorad(angle);
    
    float c = cos(rad);
    float s = sin(rad);
    
    return Vector2D<T>( x*c - y*s, x*s + y*c );
}

template<class T>
Vector2D<T> Vector2D<T>::getNormal()
{
    int len = getLength();
    if ( len == 0 )
        return 0;
    
    return *this / len;
}

template<class T>
T Vector2D<T>::getDistance(Vector2D<T> rhs)
{
    return ( rhs - *this ).getLength();
}

template<class T>
T Vector2D<T>::getDotProduct(Vector2D<T> rhs)
{
    Vector2D<T> norma = getNormal();
    Vector2D<T> normb = rhs.getNormal();
    
    return norma.x * normb.x + norma.y * normb.y;
}

template<class T>
T Vector2D<T>::getLength()
{
    return sqrt( x*x + y*y );
}

template<class T>
T Vector2D<T>::getAngle()
{
    return radtodeg(acos(x));
}


#endif