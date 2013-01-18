/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include <iostream>
#include "entitymanager.h"

// written by Henry and I'm very proud of it.
bool wildcard_match( const char *wildcard, const char *match ) {
    long match_len = strlen(match);
    long wildcard_len = strlen(wildcard);
    
    if ( match_len == wildcard_len && wildcard_len == 0 ) {
        return true;
    }
    
    int wild_pos = 0;
    int orig_wild = 0;
    int orig_match = 1;
    bool first_match = true;
    for( int i = 0; i < match_len; i++ ) {
        while ( wildcard[wild_pos] == '*' ) {
            wild_pos++;
            orig_wild = wild_pos;
        }
        
        if ( wild_pos < wildcard_len && match[i] == wildcard[wild_pos] ) {
            wild_pos++;
            if (first_match) {
                first_match = false;
                orig_match = i;
            }
            continue;
        }
        
        if ( wild_pos > 0 && wildcard[wild_pos-1] == '*' ) {
            first_match = true;
            continue;
        }
        
        i = orig_match;
        orig_match++;
        wild_pos = orig_wild;
    }
    
    return wild_pos == wildcard_len;
}

void CEntityManagerModule::update() {
    for ( EntityMap::iterator i = entityMap.begin(); i != entityMap.end(); i++ ) {
        (*i)->think();
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
}

void CEntityManagerModule::removeEntity( IEntity *ent ) {
    
    entityMap.erase(std::remove(entityMap.begin(), entityMap.end(), ent), entityMap.end());
    
    for ( EntityMap::iterator i = entityMap.begin(); i != entityMap.end(); i++ ) {
        //(*i)->think();
    }
    /*for ( EntityMap::iterator i = entityMap.begin(); i != entityMap.end(); ++i ) {
        IEntity *temp = *i;
        if ( (*i) == ent ) {
            entityMap.erase(i);
        }
    }*/
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
        if ( wildcard_match(classname, (*i)->getClassName()) ) {
            classmap.push_back((*i));
        }
    }
    return classmap;
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