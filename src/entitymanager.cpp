/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "entitymanager.h"
#include "iutil.h" // this cross-dependency crap is going to bite me in the butt
// gotta find a better system.

void CEntityManagerModule::update() {
    for ( EntityMap::iterator i = entityMap.begin(); i != entityMap.end(); i++ ) {
        (*i)->think();
    }
}

void CEntityManagerModule::render() {
    for ( EntityMap::iterator i = entityMap.begin(); i != entityMap.end(); i++ ) {
        (*i)->render();
    }
}

void CEntityManagerModule::registerEntity( const char *classname, EntityFactoryFn *factory ) {
    factoryMap[classname] = factory;
}

IEntity *CEntityManagerModule::createEntity( const char *classname ) {
    return factoryMap[classname]();
}

void CEntityManagerModule::spawnEntity( IEntity *ent ) {
    entityMap.push_back(ent);
    sort();
}

void CEntityManagerModule::removeEntity( IEntity *ent ) {
    entityMap.erase(std::remove(entityMap.begin(), entityMap.end(), ent), entityMap.end());
    sort();
}

IEntity *CEntityManagerModule::findEntityByClass(const char *classname) {
    EntityMap entities = entitymanager->findEntitiesByClass(classname);
    if ( entities.size() > 0 ) {
        return *(entitymanager->findEntitiesByClass(classname).begin());
    } else {
        return NULL;
    }
}

CEntityManagerModule::EntityMap CEntityManagerModule::findEntitiesByClass(const char *classname) {
    EntityMap classmap;
    for ( EntityMap::iterator i = entityMap.begin(); i != entityMap.end(); i++ ) {
        if ( util->wildcardMatch(classname, (*i)->getClassName()) ) {
            classmap.push_back((*i));
        }
    }
    return classmap;
}

// Sort according to z-index.
bool zIndexSort (IEntity *ient, IEntity *jent) {
    long i, j;
    i = ient->getZIndex();
    j = jent->getZIndex();
    
    if (i < 0 && j >= 0) {
        return false;
    }
    
    if (i >= 0 && j < 0) {
        return true;
    }
    
    if (i < 0 && j < 0) {
        return i>j;
    }
    
    return i<j;
}

void CEntityManagerModule::sort() {
    //std::sort(entityMap.begin(), entityMap.end(), zIndexSort);
}

void CEntityManagerModule::init( IModuleManager* modulemanager ) {
}

void CEntityManagerModule::release() {
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    entitymanager = new CEntityManagerModule();
    return entitymanager;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (CEntityManagerModule *)entitymanager;
}