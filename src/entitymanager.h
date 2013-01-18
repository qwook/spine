/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "ientitymanager.h"

class CEntityManagerModule : public IEntityManagerModule
{
private:
    FactoryMap factoryMap;
    EntityMap entityMap;
public:
    virtual void init( IModuleManager* modulemanager );
    virtual void release();
    virtual void update();
    virtual void registerEntity( const char *classname, EntityFactoryFn *factory );
    virtual IEntity *createEntity( const char *classname );
    virtual void spawnEntity( IEntity *ent );
    virtual void removeEntity( IEntity *ent );
    virtual IEntity *findEntityByClass( const char *classname );
    virtual EntityMap findEntitiesByClass( const char *classname );
};

#endif