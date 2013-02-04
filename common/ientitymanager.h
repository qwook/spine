/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef IENTITYMANAGER_H
#define IENTITYMANAGER_H

#include "imodule.h"
#include "iutil.h"
#include "imath.h"

#include <iostream>
#include <map>
#include <vector>

using namespace std;

class IEntity {
private:
    Vector2Df origin;
    float angle;
    float zIndex;
    unsigned long entIndex;
public:
    IEntity();
    ~IEntity();
    
    virtual void spawn();
    virtual void setOrigin( Vector2Df pos );
    virtual void setAngle( float angle );
    virtual void think();
    virtual void render();
    virtual void remove();
    bool operator== ( const IEntity& ent );
    const char *getClassName();
    unsigned long getEntIndex();
    
    char internalClassName;
};

typedef IEntity* EntityFactoryFn();

class IEntityManagerModule : public IModule {
public:
    typedef map<const char *, EntityFactoryFn *, cmp_str> FactoryMap;
    typedef vector<IEntity *> EntityMap;
    unsigned int index;
public:
    virtual void init( IModuleManager* modulemanager ) = 0;
    virtual void release() = 0;
    virtual void update() = 0;
    virtual void registerEntity( const char *classname, EntityFactoryFn *factory ) = 0;
    virtual IEntity *createEntity( const char *classname ) = 0;
    virtual void spawnEntity( IEntity *ent ) = 0;
    virtual void removeEntity( IEntity *ent ) = 0;
    virtual IEntity *findEntityByClass( const char *classname ) = 0;
    virtual EntityMap findEntitiesByClass( const char *classname ) = 0;
};

extern IEntityManagerModule *entitymanager;

class EntityFactoryRegister {
public:
    EntityFactoryRegister( const char *classname, EntityFactoryFn *fn ) {
        entitymanager->registerEntity(classname, fn);
    }
};



#define REGISTER_CLASS( className, class )\
IEntity* __class__EntityFactory()\
{\
class *temp = new class;\
strcpy( &temp->internalClassName, #className );\
return (IEntity *)temp;\
}\
EntityFactoryRegister __class__( #className, &__class__EntityFactory );\



#endif