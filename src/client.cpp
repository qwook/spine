/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "imodulemanager.h"
#include "ientitymanager.h"
#include "igraphics.h"
#include "ios.h"
#include "client.h"
#include "panel.h"

class CBaseEntity : public IEntity
{
public:
    virtual void think();
    virtual void render();
    virtual void setAngle( float angle );
};

REGISTER_CLASS( base_entity, CBaseEntity );

void CBaseEntity::setAngle(float angle) {
}

void CBaseEntity::think()
{
    setOrigin(graphics->getCursorPos());
}

Font *test;

void CBaseEntity::render()
{
    
    //printf("%d\n", (getZIndex()+10));
    //Material img = graphics->useMaterial("test.ttf");
    // Draw one texture mapped quad in the (x,y) plane
    
    
    /*graphics->pushScissor(true, origin.x, origin.y-5, 80, 100, true);
    graphics->setColor(255, 0, 0, 255);
    graphics->useMaterial("white");
    graphics->drawQuad(origin.x,origin.y,100, 500);
    
    graphics->pushScissor(true, origin.x, origin.y-5, 200, 50, true);
    graphics->setColor(0, 255, 0, 255);
    graphics->useMaterial("white");
    graphics->drawQuad(origin.x,origin.y+10,500, 500);
    graphics->popScissor();
    
    graphics->popScissor();
    
    graphics->setColor(0, 0, 0, 255);
    graphics->useProgram("default");
    graphics->pushScissor(true, origin.x, origin.y-5, 80, 100, true);
    graphics->drawText(test, "gui1234", origin);
    graphics->popScissor();*/
    
    
}

Panel *p;

void CClientModule::update() {
    Vector2Df mxy = graphics->getCursorPos();
    
    if (Panel::hoveredPanel) {
        Panel::hoveredPanel->setHovered(false);
    }
    p->recursiveHoverDetect(mxy.x, mxy.y);
    
    entitymanager->update();
}

void CClientModule::render() {
    entitymanager->render();
    
    graphics->useProgram("default");
    graphics->setColor(0, 0, 0, 255);
    graphics->drawText(test, "hello! 10 bitches suck dick </>", Vector2Df(0,150));
    graphics->drawQuad(0,200,128, 128);
    
    p->recursiveDraw();
}

void CClientModule::mouseDown(int mouseKey) {
    if (Panel::hoveredPanel) {
        Panel::hoveredPanel->setMouseDown(true);
        Panel::pressedPanel = Panel::hoveredPanel;
    }
}

void CClientModule::mouseUp(int mouseKey) {
    if (Panel::pressedPanel) {
        Panel::pressedPanel->setMouseDown(false);
    }
}

void CClientModule::init( IModuleManager* modulemanager ) {
    //graphics->setCursorVisible(false);
    test = graphics->loadFont("DroidSans.ttf", 12);
    
    /*IEntity* entD = entitymanager->createEntity("base_entity");
    entD->spawn();
    entD->setOrigin(Vector2Df(0,0));
    entD->setZIndex(-2);*/
    
    p = new Panel();
    p->setOrigin( Vector2Df(5, 5) );
    p->setWidth( 100 );
    p->setHeight( 100 );
    
    Panel *z = new Panel();
    z->setParent( p );
    z->setOrigin(Vector2Df(60, 10));
    z->setWidth( 50 );
    z->setHeight( 50 );
    
    Panel *a = new Panel();
    a->setParent( p );
    a->setOrigin(Vector2Df(3, 3));
    a->setWidth( 10 );
    a->setHeight( 10 );
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