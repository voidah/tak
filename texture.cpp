#include "texture.h"
#include <cassert>

// TODO
#include <iostream>
#include <IL/ilu.h>


Texture::Texture() : m_isValid(false)
{
}

Texture::~Texture()
{
    if(IsValid())
        glDeleteTextures(1, &m_textureId);
}


bool Texture::Load(const std::string& filename, bool mipmap)
{
    // Initialize Devil only once:
    static bool alreadyInitialized = false;
    if(!alreadyInitialized)
    {
        ilInit();
        iluInit();
        alreadyInitialized = true;
    }

    // Use Devil library to load image data in memory
    ILuint texid;
    ilGenImages(1, &texid);
    ilBindImage(texid);

    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    ilEnable(IL_ORIGIN_SET);

    // Si ilLoadImage retourne false ici, c'est à cause que j'utiliser aussi un sf::Image (sfml)
    // au moins à une place dans le programme. Ca semble faire un conflit, qui entraine que Devil
    // n'est plus capable de charger d'images au format PNG
    if (!ilLoadImage((const ILstring)filename.c_str()))
    {
        ILenum Error;
        while ((Error = ilGetError()) != IL_NO_ERROR)
            printf("Error ilLoadImage #%d: %s\n", Error, iluErrorString(Error)); 
        return false;
    }

    if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
        return false;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
    }


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

    ilDeleteImages(1, &texid);

    m_isValid = true;
    return true;
}

bool Texture::LoadFromMemoryRGBA(const uint8* data, int width, int height, bool mipmap)
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(mipmap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    m_isValid = true;
    return true;
}

bool Texture::IsValid() const
{
    return m_isValid;
}

void Texture::Bind() const
{
    assert(IsValid());
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

