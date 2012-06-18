#ifndef TAK__TEXTURE_H__
#define TAK__TEXTURE_H__

#include "define.h"
#include "nocopy.h"
#include <IL/il.h>
#include <string>

class Texture
{
    NOCOPY(Texture);

public:
    Texture();
    ~Texture();

    bool Load(const std::string& filename, bool mipmap = true);
    bool LoadFromMemoryRGBA(const std::string& name, const uint8* data, int width, int height, bool mipmap = true);
    bool IsValid() const;
    void Bind() const;

    uint GetTextureId() const;
    std::string GetTextureName() const;

    bool operator==(const Texture& texture) const;
    bool operator!=(const Texture& texture) const;

private:
    GLuint  m_textureId;
    std::string m_textureName;
    bool    m_isValid;
};

#endif // TEXTURE_H__
