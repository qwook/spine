/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "ientitymanager.h"

void IEntity::spawn() {
    entitymanager->spawnEntity(this);
}

IEntity::IEntity() : zIndex(0), angle(0), origin(Vector2Df(0,0)) {
    entitymanager->index++;
    entIndex = entitymanager->index;
}

IEntity::~IEntity() {
}

void IEntity::think() {
    
}

void IEntity::render() {
    
}

void IEntity::setOrigin(Vector2Df pos) {
    this->origin = pos;
}

void IEntity::setAngle(float angle) {
    this->angle = angle;
}

void IEntity::remove() {
    entitymanager->removeEntity(this);
    delete this;
}

const char *IEntity::getClassName() {
    return &internalClassName;
}

unsigned long IEntity::getEntIndex()
{
    return entIndex;
}

bool IEntity::operator== ( const IEntity& ent )
{
    return entIndex == ent.entIndex;
}

void IEntity::setZIndex( long z ) {
    zIndex = z;
    entitymanager->sort();
};
