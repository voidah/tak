#ifndef FONT_H__
#define FONT_H__

#include "define.h"
#include "texture.h"
#include "resource.h"
#include <string>

class Font : public Resource<Font>
{
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

        virtual bool Load(const std::string& key, const OptionList& options);
        virtual bool Release();

        const CharInfo& GetCharInfo(char asciiChar) const;

        Texture& GetTexture();
        std::string GetAnsiCodePage() const;

    private:
        bool Load(const std::string& f, int textureSize, const std::string& ansiCodePage);

    private:
        Texture m_texture;
        CharInfo m_charInfo[256];
        std::string m_ansiCodePage;
};

#endif // FONT_H__
