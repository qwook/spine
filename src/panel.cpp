//
//  panel.cpp
//  spine
//
//  Created by Henry Tran on 2/20/13.
//  Copyright (c) 2013 Henry Tran. All rights reserved.
//

#include "igraphics.h"
#include "panel.h"

Panel *Panel::hoveredPanel = 0;

Panel::Panel() {
    origin.x = 0;
    origin.y = 0;
    width = 0;
    height = 0;
    parent = 0;
    hovered = false;
}

void Panel::recursiveDraw() {
    graphics->pushScissor(true, origin.x, origin.y, width, height, true);
    render();
    
    for (List::iterator i = children.begin(); i != children.end(); i++) {
        (*i)->recursiveDraw();
    }
    
    graphics->popScissor();
}

bool Panel::recursiveHoverDetect(int x, int y) {
    if (x < origin.x || y < origin.y || x > origin.x + width || y > origin.y + height) {
        return false;
    }
    
    for (List::reverse_iterator i = children.rbegin(); i != children.rend(); i++) {
        if ((*i)->recursiveHoverDetect(x - origin.x, y - origin.y)) {
            return true;
        }
    }
    
    hovered = true;
    Panel::hoveredPanel = this;
    
    return true;
}

void Panel::render() {
    graphics->useProgram("default");
    graphics->useMaterial("white");
    
    graphics->setColor(255, 0, 0);
    graphics->drawQuad(0, 0, width, height);
    
    if (isHovered()) {
        graphics->setColor(40, 0, 50);
    } else {
        graphics->setColor(40, 250, 50);
    }
    
    graphics->drawQuad(1, 1, width - 2, height - 2);
}

void Panel::setOrigin( Vector2Df pos ) {
    origin = pos;
}

void Panel::setWidth( int width ) {
    this->width = width;
}

void Panel::setHeight( int height ) {
    this->height = height;
}

void Panel::setParent( Panel *parent ) {
    if ( this->parent != 0 ) {
        this->parent->children.erase(std::remove(this->parent->children.begin(), this->parent->children.end(), this), this->parent->children.end());
    }
    
    parent->children.push_back(this);
    this->parent = parent;
}

void Panel::setMouseDown(bool _mousedown) {
    mousedown = _mousedown;
}

void Panel::setHovered(bool hover) {
    hovered = hover;
}

bool Panel::isMouseDown() {
    return mousedown;
}

bool Panel::isHovered() {
    return hovered;
}
