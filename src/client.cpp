/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "imodulemanager.h"
#include "ientitymanager.h"
#include "client.h"

class CBaseEntity : public IEntity
{
public:
    virtual void think();
    virtual void setAngle( float angle );
};

REGISTER_CLASS( base_entity, CBaseEntity );

void CBaseEntity::setAngle(float angle) {
}

void CBaseEntity::think()
{
}

void CClientModule::update() {
}

void CClientModule::init( IModuleManager* modulemanager ) {
    IEntity* ent = entitymanager->createEntity("base_entity");
    ent->spawn();
}

void CClientModule::release() {
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    client = new CClientModule();
    return client;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (CClientModule *)client;
}