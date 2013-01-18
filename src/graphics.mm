/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "graphics.h"

#include <iostream>
#include <OpenGL/gl.h>
#include <GL/glfw.h>

void CGraphicsModule::initDriver()
{
    glfwInit();
    
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 2 );
    glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    
    glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
    glfwOpenWindow( 800, 600, 0, 0, 0, 0, 0, 0, GLFW_WINDOW );
    glfwSetWindowTitle( "OpenGL" );
    
    const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
    
    printf("Version: %s\nRenderer: %s\nVendor: %s\n", version, renderer, vendor);
}

void CGraphicsModule::init( IModuleManager* modulemanager ) {
}

void CGraphicsModule::release() {
    glfwTerminate();
}

bool CGraphicsModule::update() {
    if ( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS )
        return false;
    
    if ( !glfwGetWindowParam( GLFW_OPENED ) )
        return false;
    
    glfwSwapBuffers();
    return true;
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    graphics = new CGraphicsModule();
    return graphics;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (CGraphicsModule *)graphics;
}