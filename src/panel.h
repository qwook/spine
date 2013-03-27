//
//  panel.h
//  spine
//
//  Created by Henry Tran on 2/20/13.
//  Copyright (c) 2013 Henry Tran. All rights reserved.
//

#ifndef __spine__panel__
#define __spine__panel__

#include <iostream>
#include <vector>
#include "imath.h"

class Panel {
private:
    Vector2Df origin;
    int width;
    int height;
    Panel *parent;
    bool hovered;
    bool mousedown;
    
    typedef std::vector<Panel*> List;
public:
    Panel();
    
    void recursiveDraw();
    bool recursiveHoverDetect(int x, int y);
    void render();
    
    void setOrigin( Vector2Df origin );
    void setWidth( int width );
    void setHeight( int height );
    
    void setParent( Panel *parent );
    
    void setMouseDown(bool _mousedown);
    void setHovered(bool hover);
    bool isMouseDown();
    bool isHovered();
    
    List children;
    
    static Panel *hoveredPanel;
    static Panel *pressedPanel;
};

#endif /* defined(__spine__panel__) */
