
#include "imath.h"
#include <cmath>


Vector2Df Vector2Df::operator+(Vector2Df rhs) {
    return Vector2Df( x + rhs.x, y + rhs.y );
}

Vector2Df Vector2Df::operator-(Vector2Df rhs) {
    return Vector2Df( x - rhs.x, y - rhs.y );
}

Vector2Df Vector2Df::operator*(float scalar) {
    return Vector2Df( x * scalar, y * scalar );
}

Vector2Df Vector2Df::operator/(float scalar) {
    return Vector2Df( x / scalar, y / scalar );
}

Vector2Df Vector2Df::getRotated( float angle ) {
    float rad = degtorad(angle);
    
    float c = cos(rad);
    float s = sin(rad);
    
    return Vector2Df( x*c - y*s, x*s + y*c );
}

Vector2Df Vector2Df::getNormal() {
    int len = getLength();
    if ( len == 0 ) {
        return 0;
    }
    
    return *this / len;
}

float Vector2Df::getDistance(Vector2Df rhs) const {
    return ( rhs - *this ).getLength();
}

float Vector2Df::getDot(Vector2Df rhs) const {
    return x * rhs.x + y * rhs.y;
}

float Vector2Df::getLength() const {
    return sqrt( getDot(*this) );
}

float Vector2Df::getLengthSqr() const {
    return x*x + y*y;
}

float Vector2Df::getAngle() const {
    return radtodeg(acos(x));
}

