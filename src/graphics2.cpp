/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

/* Welcome to Suburbia. */

#include "imath.h"
#include "ifilesystem.h"
#include "graphics.h"

#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glfw.h>

#include "imagedata.h"
#include "fonts.h"

std::map<std::string, GLuint> shaders;
std::map<std::string, GLuint> programs;
std::map<std::string, Material> materials;

Font *test;

void MousePressedCallback(int i, int b)
{
    printf("%d %d\n", i, b);
}

void CGraphicsModule::initDriver( short width, short height )
{
    offsetX = 0;
    offsetY = 0;
    
    this->width = width;
    this->height = height;
    
    if (!glfwInit()) {
        printf("Failed to start GLFW.");
        exit(1);
    }
    
#ifdef USE_OPENGL_3
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 2 );
    glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#endif
    
    glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
    
    glfwOpenWindow( width, height, 0, 0, 0, 0, 8, 0, GLFW_WINDOW );
    glfwSetWindowTitle( "Spine" );
    
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* glslvers = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    printf("Version: %s\nRenderer: %s\nVendor: %s\nGLSLVersion: %s\n", version, renderer, vendor, glslvers);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf( "Error loading glew!" );
        exit(1);
    } else {
        printf( "Using GLEW: %s %d\n", glewGetString(GLEW_VERSION), glewIsSupported("GL_ARB_fragment_program") );
    }
    
    // Compile all the shaders.
    DirectoryList list = filesystem->getDirectoryList("./shaders/");
    for (DirectoryList::iterator i = list.begin(); i != list.end(); ++i) {
        char name[50];
        char exten[10];
        util->getFileName(name, (*i).c_str());
        util->getFileExtension(exten, (*i).c_str());
        
        Buffer buffer;
        File *file = filesystem->open((*i).c_str());
        file->readBuffer(&buffer);
        
        if (strcmp(exten, ".fsh") == 0) {
            const char *c_str = buffer.data;
            GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
            glShaderSource( fragmentShader, 1, &c_str, NULL );
            glCompileShader( fragmentShader );
            shaders[name] = fragmentShader;
            
            GLint status;
            glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &status );
            
            if (status == GL_TRUE) {
                printf("Loaded fragment shader: %s (%d)\n", name, fragmentShader);
            } else {
                char buffer[512];
                glGetShaderInfoLog( fragmentShader, 512, NULL, buffer );
                
                printf("Error loading fragment shader: %s (%d)\n", name, fragmentShader);
                printf("%s\n", buffer);
            }
        } else if (strcmp(exten, ".vsh") == 0) {
            const char *c_str = buffer.data;
            GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
            glShaderSource( vertexShader, 1, &c_str, NULL );
            glCompileShader( vertexShader );
            shaders[name] = vertexShader;
            
            GLint status;
            glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &status );
            
            if (status == GL_TRUE) {
                printf("Loaded vertex shader: %s (%d)\n", name, vertexShader);
            } else {
                char buffer[512];
                glGetShaderInfoLog( vertexShader, 512, NULL, buffer );
                
                printf("Error loading vertex shader: %s (%d)\n", name, vertexShader);
                printf("%s\n", buffer);
            }
        }
        
        delete file;
    }
    
    // Compile all the programs.
    for (DirectoryList::iterator i = list.begin(); i != list.end(); ++i) {
        char exten[10];
        util->getFileExtension(exten, (*i).c_str());
        if (strcmp(exten, ".kv") == 0) {
            char name[50];
            util->getFileName(name, (*i).c_str());
            util->getFileNameNoExtension(name, name);
            
            Buffer buffer;
            File *file = filesystem->open((*i).c_str());
            file->readBuffer(&buffer);
            
            KeyValue *kv = util->parseKeyValue(&buffer);
            
            GLuint shaderProgram = glCreateProgram();
            
            KeyValue *shadermembers = kv->getMember("shaders");
            
            if (shadermembers != NULL) {
                unsigned int b = 0;
                KeyValue *member = shadermembers->getMember(b);
                while ((member = shadermembers->getMember(b)) != NULL) {
                    glAttachShader( shaderProgram, shaders[member->getString()] );
                    b++;
                }
            }
            
            KeyValue *fragdatamembers = kv->getMember("fragdata");
            
            if (fragdatamembers != NULL) {
                unsigned int b = 0;
                KeyValue *member = fragdatamembers->getMember(b);
                while ((member = fragdatamembers->getMember(b)) != NULL) {
                    glBindFragDataLocation( shaderProgram, b, member->getString().c_str());
                    b++;
                }
            }
            
            glLinkProgram( shaderProgram );
            glUseProgram( shaderProgram );
            
            GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
            glEnableVertexAttribArray( posAttrib );
            glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0 );
            
            programs[name] = shaderProgram;
            
            printf("Loaded program: %s (%d)\n", name, shaderProgram);
            
            kv->release();
            file->release();
        }
        
    }
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBlendEquation( GL_FUNC_ADD );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    loadMaterial("disttex.tga");
    loadMaterial("8.tga");
    
    // load default textures
    ImageData white(1, 1);
    white.setPixel(0, 0, Color(255, 255, 255));
    loadMaterial("white", &white);
    
    ImageData black(1, 1);
    black.setPixel(0, 0, Color(0, 0, 0));
    loadMaterial("black", &black);
    
    glfwSetMouseButtonCallback(&MousePressedCallback);
}

Font *CGraphicsModule::loadFont(const char *fontFile, int size)
{
    return new Font(fontFile, size, 128, 128);
}

void CGraphicsModule::drawText( Font *font, const char *text, Vector2Df pos ) {
    font->drawString(text, pos);
}

Material CGraphicsModule::loadMaterial(const char *matName, ImageData *data) {
    Material material = materials[matName];
    
    if (material.mat) {
        return useMaterial(matName);
    }
    
    GLuint textureID;
    glGenTextures( 1, &textureID );
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, textureID );
    
    unsigned int format = GL_RGBA;
    unsigned int level = 0;
    glTexImage2D( GL_TEXTURE_2D, level, format,
                 data->width, data->height, 0, format,
                 GL_UNSIGNED_BYTE, (void*) data->data );
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    Material ret = { (unsigned int)textureID, data->width, data->height };
    materials[matName] = ret;
    
    return ret;
}

Material CGraphicsModule::loadMaterial(const char *matName) {
    Material material = materials[matName];
    
    if (material.mat) {
        return useMaterial(matName);
    }
    
    GLuint textureID;
    glGenTextures( 1, &textureID );
    
    GLFWimage teximg; // Use intermediate GLFWimage to get width and height
    
    if(!glfwReadImage(matName, &teximg, GLFW_NO_RESCALE_BIT))
        printf("I/O error %s", "Failed to load distance texture from TGA file.");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, textureID );
    
    glfwLoadTextureImage2D( &teximg, 0 );
    
    unsigned int width = teximg.Width, height = teximg.Height;
    
    // The special shader used to render this texture performs its own minification
    // and magnification. Specify nearest neighbor sampling to avoid trampling
    // over the distance values split over two channels as 8.8 fixed-point data.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glfwFreeImage(&teximg); // Clean up the malloc()'ed data pointer
    
    Material ret = { (unsigned int)textureID, width, height };
    materials[matName] = ret;
    
    return ret;
}

Material CGraphicsModule::useMaterial(const char *matName) {
    Material material = materials[matName];
    if (material.mat) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture( GL_TEXTURE_2D, material.mat );
        
        return material;
    }
    
    Material ret = {0, 0, 0};
    return ret;
}

Material CGraphicsModule::useMaterial(unsigned int mat) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, mat );
    
    int width, height;
    
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    
    Material ret = { (unsigned int)mat, (unsigned int)width, (unsigned int)height };
    
    return ret;
}

unsigned int CGraphicsModule::useProgram(const char *programName) {
    GLuint program = programs[programName];
    if (program) {
        glUseProgram(program);
        return program;
    }
    return 0;
}

unsigned int CGraphicsModule::useProgram(unsigned int program) {
    glUseProgram(program);
    return program;
}


void CGraphicsModule::setProgramVar1f(unsigned int program, const char *var, float i) {
    GLint location = glGetUniformLocation( program, var );
    glUniform1f( location, i );
}

void CGraphicsModule::setProgramVar1i(unsigned int program, const char *var, int i) {
    GLint location = glGetUniformLocation( program, var );
    glUniform1i( location, i );
}

void CGraphicsModule::drawQuad(float x, float y, float width, float height) {
    glBegin( GL_QUADS );
    glTexCoord2f( 0.0f, 1.0f );
    glVertex2f( x+offsetX, y+height+offsetY );
    glTexCoord2f( 1.0f, 1.0f );
    glVertex2f( x+width+offsetX, y+height+offsetY );
    glTexCoord2f( 1.0f, 0.0f );
    glVertex2f( x+width+offsetX, y+offsetY );
    glTexCoord2f( 0.0f, 0.0f );
    glVertex2f( x+offsetX, y+offsetY );
    glEnd();
}

void CGraphicsModule::drawQuadUV(float x, float y, float width, float height, float u1, float u2, float v1, float v2) {
    glBegin( GL_QUADS );
    glTexCoord2f( u1, v2 );
    glVertex2f( x+offsetX, y+height+offsetY );
    glTexCoord2f( v1, v2 );
    glVertex2f( x+width+offsetX, y+height+offsetY );
    glTexCoord2f( v1, u2 );
    glVertex2f( x+width+offsetX, y+offsetY );
    glTexCoord2f( u1, u2 );
    glVertex2f( x+offsetX, y+offsetY );
    glEnd();
}

void CGraphicsModule::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    glColor4f((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f);
}

void CGraphicsModule::init( IModuleManager* modulemanager ) {
}

void CGraphicsModule::release() {
    glfwTerminate();
}

bool CGraphicsModule::isWindowOpen() {
    if ( glfwGetKey( GLFW_KEY_ESC ) == GLFW_PRESS ) {
        return false;
    }
    
    if ( !glfwGetWindowParam( GLFW_OPENED ) ) {
        return false;
    }
    
    return true;
}

void CGraphicsModule::clear() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glDepthMask(true);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
    glOrtho(0.0f, 1.0f*getWidth(), 1.0f*getHeight(), 0.0f, 0.0f, 1.0f);
    //glOrtho(0.0f, 1.0f*800, 0.0f, 1.0f*600, -1.0f, 100.0f);
    //gluPerspective( 45.0f, (GLfloat)800/(GLfloat)600, 1.0f, 100.0f );
    //glViewport( 0, 0, 640, 480 );
    
    glDepthMask(false);
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    offsetX = 0;
    offsetY = 0;
    
    while ( scissorStack.size() > 0 ) {
        popScissor();
    }
}

void CGraphicsModule::setCursorVisible( bool visible ) {
    if ( visible ) {
        glfwEnable( GLFW_MOUSE_CURSOR );
    } else {
        glfwDisable( GLFW_MOUSE_CURSOR );
    }
}

Vector2Df CGraphicsModule::getCursorPos() {
    int x, y;
    glfwGetMousePos(&x, &y);
    return Vector2Df((float)x, (float)y);
}

void CGraphicsModule::update() {
    glfwSwapBuffers();
}

void CGraphicsModule::pushScissor(bool enable, int x, int y, int width, int height, bool offset) {
    
    int cutx = 0;
    int cuty = 0;
    int cutwidth = 0;
    int cutheight = 0;
    
    int nX = x;
    int nY = y;
    int nWidth = width;
    int nHeight = height;
    
    if ( enable ) {
        glEnable(GL_SCISSOR_TEST);
        if ( scissorStack.size() > 0 ) {
            Scissor s = *scissorStack.end()--;
            if ( x < s.x ) {
                cutx = s.x - x;
            }
            if ( y < s.y ) {
                cuty = s.y - y;
            }
            if ( x + width > s.x + s.width ) {
                cutwidth = (x + width) - (s.x + s.width);
            }
            if ( y + height > s.y + s.height ) {
                cutheight = (y + height) - (s.y + s.height);
            }
        }
        nX = x - cutx;
        nY = getHeight() - y - cuty - height;
        nWidth = width - cutx - cutwidth;
        nHeight = height - cuty - cutheight;
        glScissor(nX, nY, nWidth, nHeight);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
    
    if (offset == true) {
        offsetX += x;
        offsetY += y;
    }
    
    scissorStack.push_back(Scissor(enable, nX, nY, nWidth, nHeight, offset, x, y));
}

void CGraphicsModule::popScissor(){
    if ( scissorStack.size() > 0 ) {
        Scissor s = *scissorStack.rbegin();
        if ( s.offset == true ) {
            offsetX -= s.ox;
            offsetY -= s.oy;
        }
    }
    
    scissorStack.pop_back();
    if ( scissorStack.size() > 0 ) {
        Scissor s = *scissorStack.end()--;
        if ( s.enable ) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(s.x, s.y, s.width, s.height);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

short CGraphicsModule::getWidth() {
    return width;
}

short CGraphicsModule::getHeight() {
    return height;
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    graphics = new CGraphicsModule();
    return graphics;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (CGraphicsModule *)graphics;
}