#include "T3D.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

typedef DrawBuffer<unsigned char> FontBuff;
struct Character{
    FontBuff* buff;
    Vec2i Size;
    Vec2i Bearing;
    GLuint Advance;
};

std::map<GLulong, Character> Characters;
FT_Library ft;
FT_Face face;

//utf-8 to unicode
GLulong get_unicode(const char *s, size_t &idx)
{
    GLubyte b0 = s[idx];
    GLulong re;
    if(!(b0 & 0x80))
    {
        idx++;
        return b0;
    }
    if((b0&0xE0) == 0xC0)
    {
        re = ((b0&0x1F)<<6) + (s[idx+1]&0x3F); //110xxxx 10xxxxxx
        idx += 2;
        return re;
    }
    if((b0&0xF0) == 0xE0)
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        re = ((b0&0x0F)<<12) + ((s[idx+1]&0x3F)<<6) + ((s[idx+2]&0x3F));
        idx += 3;
        return re;
    }
    
    //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    re = ((b0&0x07)<<18) + ((s[idx+1]&0x3F)<<12) + ((s[idx+2]&0x3F)<<6) + ((s[idx+3]&0x3F));
    idx += 4;
    return re;
    
}

void init_font(const char* font_path)
{
    if(FT_Init_FreeType(&ft))
        std::cout<<"ERROR::FREETYPE: Could not init FreeType Libary"<<std::endl;
    
    if(FT_New_Face(ft, font_path, 0, &face))
        std::cout<<"ERROR::FREETYTE: Failed to load font file."<<std::endl;
    
    int error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if(error)
        std::cout<<"ERROR::FREETYTE: select charmap file."<<std::endl;
    
    FT_Set_Pixel_Sizes(face, 0, 90);

    
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    for(GLubyte c=0;c<128;c++)
//    {
//        add_font_texutre(c);
//    }
//    if (FT_Load_Char(face, 0x54c8, FT_LOAD_RENDER))
//    {
//        std::cout<<"ERROR::FREETYPE:Failed to load GLyph"<<std::endl;
//    }
    
    
    // glBindTexture(GL_TEXTURE_2D, 0);
    
}



void add_font_texture(GLulong c)
{

    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
        std::cout<<"ERROR::FREETYPE:Failed to load GLyph"<<std::endl;
        return;
    }
    GLuint width = face->glyph->bitmap.width;
    GLuint rows  = face->glyph->bitmap.rows;

    auto bf = new FontBuff(width,rows);
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, , , 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
    
    // 设置纹理选项
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    bf->mem_cpy(face->glyph->bitmap.buffer);
    
    Character character = {
        bf,
        Vec2i(face->glyph->bitmap.width,face->glyph->bitmap.rows),
        Vec2i(face->glyph->bitmap_left,face->glyph->bitmap_top),
        (GLuint)(face->glyph->advance.x)
    };
    
    Characters.insert(std::pair<GLulong,Character>(c,character));
    
}

Character &get_char(GLulong c)
{
    auto it = Characters.find(c);
    if(it == Characters.end())
    {
        add_font_texture(c);
    }
    return Characters[c];
}


