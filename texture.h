#ifndef TAK__TEXTURE_H__
#define TAK__TEXTURE_H__

#include "define.h"
#include <IL/il.h>
#include <string>

class Texture
{
public:
    Texture(const std::string& filename = "");
    ~Texture();

    bool Load(const std::string& filename);
    bool IsValid() const;
    void Bind() const;

private:
    // Disallow texture object to be copied
    Texture(const Texture&);
    Texture& operator=(const Texture&);

private:
    GLuint  m_textureId;
    bool    m_isValid;
};

#endif // TEXTURE_H__
