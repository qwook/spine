/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

/* Welcome to Suburbia. */

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

std::map<std::string, GLuint> shaders;
std::map<std::string, GLuint> programs;
std::map<std::string, Material> materials;

void CGraphicsModule::initDriver()
{
    //glfwSwapInterval(1);
    
    GLuint err;
    
    if (!glfwInit()) {
        printf("Failed to start GLFW.");
        exit(1);
    }
    
    /*glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
    glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 2 );
    glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );*/
    
    glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );
    
    glfwOpenWindow( 800, 600, 0, 0, 0, 0, 8, 0, GLFW_WINDOW );
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
    
    // Draw triangles. Commented out for future reference.
    /*float vertices[] = {
        0.0f,  0.5f, // Vertex 1 (X, Y)
        0.5f, -0.5f, // Vertex 2 (X, Y)
        -0.5f, -0.5f  // Vertex 3 (X, Y)
    };
    
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    
    GLuint vbo;
    glGenBuffers( 1, &vbo ); // Generate 1 buffer
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );*/
    
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
    glVertex2f( x, y+height );
    glTexCoord2f( 1.0f, 1.0f );
    glVertex2f( x+width, y+height );
    glTexCoord2f( 1.0f, 0.0f );
    glVertex2f( x+width, y );
    glTexCoord2f( 0.0f, 0.0f );
    glVertex2f( x, y );
    glEnd();
}

void CGraphicsModule::drawQuadUV(float x, float y, float width, float height, float u1, float u2, float v1, float v2) {
    glBegin( GL_QUADS );
    glTexCoord2f( u1, v2 );
    glVertex2f( x, y+height );
    glTexCoord2f( v1, v2 );
    glVertex2f( x+width, y+height );
    glTexCoord2f( v1, u2 );
    glVertex2f( x+width, y );
    glTexCoord2f( u1, u2 );
    glVertex2f( x, y );
    glEnd();
}

void CGraphicsModule::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    glColor4f(r/255, g/255, b/255, a/255);
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
    
    gluPerspective( 45.0f, (GLfloat)800/(GLfloat)600, 1.0f, 100.0f );
    glViewport( 0, 0, 800, 600 );
    
    glDepthMask(false);
}

void CGraphicsModule::update() {
    /*for( int i = 1; i < 8; i++ ) {
        
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, (sin(glfwGetTime()*i/4)*2)-3.0f);
        
        setColor(255, 5, 50, 255);
        
        Material img = useMaterial("8.tga");
        
        unsigned int sdfield = useProgram("sdfield");
        setProgramVar1i(sdfield, "disttext", 0);
        setProgramVar1f(sdfield, "texw", img.width);
        setProgramVar1f(sdfield, "texh", img.height);
        
        // Draw one texture mapped quad in the (x,y) plane
        drawQuad(-0.5,-0.5,1,1);
    }*/
    
    glDepthMask(true);
    
    glfwSwapBuffers();
}

extern "C" IModule * SPINE_DLLEXPORT loadModule() {
    graphics = new CGraphicsModule();
    return graphics;
}

extern "C" void SPINE_DLLEXPORT unloadModule() {
    delete (CGraphicsModule *)graphics;
}