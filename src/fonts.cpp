/*
 
 Copyright (C) 2013 Henry Tran
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#include "fonts.h"
#include "graphics.h"
#include "imagedata.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H

Font::Font(const char *name, unsigned int size, unsigned int width, unsigned int height) {
    
    const char *cachedglyphs = " !\"#$%&'()*+,-./0123456789:;<=>?"
                                "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                                "`abcdefghijklmnopqrstuvwxyz{|}~";
    
    const char *dataptr = &cachedglyphs[0];
    
    ImageData imgdat(width, height);
    imgdat.clear(Color(255, 255, 255, 0));
    
    while (*dataptr != '\0') {
        
        char c = *dataptr++;
        
        FT_Library library;
        if (FT_Init_FreeType(&library) != 0)
        {
            printf("Failed to load freetype!\n");
        }
        
        FT_Face face;
        if (FT_New_Face(library, "fonts/DroidSans.ttf", 0, &face) != 0) {
            printf("Failed to create new face!\n");
        }
        
        if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0) {
            printf("Failed to load charmap!\n");
        }
        
        FT_Set_Pixel_Sizes(face, 0, size);
        
        if (FT_Load_Char(face, c, FT_LOAD_TARGET_NORMAL) != 0) {
            printf("Failed to load glyph!\n");
        }
        
        FT_Glyph glyphDesc;
        if (FT_Get_Glyph(face->glyph, &glyphDesc) != 0) {
            printf("Failed to load glyph!\n");
        }
        
        FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;
        FT_Bitmap& bitmap = bitmapGlyph->bitmap;
        
        unsigned int width = bitmap.width;
        unsigned int height = bitmap.rows;
        
        char *data = new char[width * height * 4];
        char *dataptr = data;
        
        const unsigned char* pixels = bitmap.buffer;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                *dataptr++ = 255;
                *dataptr++ = 255;
                *dataptr++ = 255;
                if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
                    *dataptr++ = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
                } else {
                    *dataptr++ = pixels[x];
                }
            }
            pixels += bitmap.pitch;
        }
        
        Vector2Df region = imgdat.getRegion((int)width, (int)height);
        
        ImageData glyph_data(width, height, data);
        imgdat.copyRegion(&glyph_data, (int)region.x, (int)region.y);
        
        Glyph glyph;
        glyph.x = (int)region.x;
        glyph.y = (int)region.y;
        glyph.width = width;
        glyph.height = height;
        glyph.advance = glyphDesc->advance.x >> 16;
        glyph.offset_x = bitmapGlyph->left;
        glyph.offset_y = -bitmapGlyph->top;
        
        FT_Done_Face(face);
        
        glyphlist[c] = glyph;
        
        delete data;
    }

    material = ((CGraphicsModule *)graphics)->loadMaterial(name, &imgdat);
    
}

void Font::drawGlyph( char charcode, Vector2Df pos ) {
    Glyph glyph = glyphlist[charcode];
    graphics->drawQuadUV(pos.x, pos.y + glyph.offset_y, glyph.width, glyph.height, (float)glyph.x/material.width, (float)glyph.y/material.height, (float)(glyph.x+glyph.width)/material.width, (float)(glyph.y+glyph.height)/material.height);
}

void Font::drawString( const char *string, Vector2Df pos ) {
    const char *ptr = &string[0];
    int offset_x = 0;
    
    graphics->useMaterial(material.mat);
    
    while (*ptr != '\0') {
        char charcode = *ptr++;
        Glyph glyph = glyphlist[charcode];
        graphics->drawQuadUV(pos.x + offset_x + glyph.offset_x, pos.y + glyph.offset_y, glyph.width, glyph.height, (float)glyph.x/material.width, (float)glyph.y/material.height, (float)(glyph.x+glyph.width)/material.width, (float)(glyph.y+glyph.height)/material.height);
        
        switch (charcode)
        {
            case ' ': offset_x += glyph.advance; continue;
            case '\t': offset_x += glyph.advance*4; continue;
            default: offset_x += glyph.advance;
        }
    }
}

Vector2Df Font::getStringSize( char *string ) {
    /*char *ptr = &string[0];
    int offset_x = 0;
    int height = 0;
    int width = 0;
    
    while (*ptr != '\0') {
        char charcode = *ptr++;
        Glyph glyph = glyphlist[charcode];
        graphics->drawQuadUV(pos.x + offset_x + glyph.offset_x, pos.y + glyph.offset_y, glyph.width, glyph.height, (float)glyph.x/material.width, (float)glyph.y/material.height, (float)(glyph.x+glyph.width)/material.width, (float)(glyph.y+glyph.height)/material.height);
        
        switch (charcode)
        {
            case ' ': offset_x += glyph.advance; continue;
            case '\t': offset_x += glyph.advance*4; continue;
            default: offset_x += glyph.advance;
        }
    }*/
}
