#ifndef FONT_H__
#define FONT_H__

#include "define.h"
#include "texture.h"
#include "nocopy.h"
#include <string>

class Font
{
    NOCOPY(Font);

    public:
        struct CharInfo
        {
            // Texture coordinates [0..1]
            float texX; // lower left corner
            float texY; // lower right
            float texW; // width
            float texH; // height

            // Character dimensions in pixel
            float charW;
            float charH;
            float charB; // offset under the baseline

            void Set(float tx, float ty, float tw, float th, float cw, float ch, float cb)
            {
                texX = tx;
                texY = ty;
                texW = tw;
                texH = th;
                charW = cw;
                charH = ch;
                charB = cb;
            }
        };

    public:
        Font();
        ~Font();
        bool Load(const std::string& f, int textureSize = 512, const std::string& ansiCodePage = "ISO−8859−1");
        const CharInfo& GetCharInfo(char asciiChar) const;

        const Texture& GetTexture() const;
        std::string GetAnsiCodePage() const;

    private:
        Texture m_texture;
        CharInfo m_charInfo[256];
        std::string m_ansiCodePage;
};

#endif // FONT_H__
