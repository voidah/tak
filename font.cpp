#include "font.h"
#include "unicode.h"

Font::Font()
{
}

Font::~Font()
{
}

bool Font::Load(const std::string& key, const OptionList& options)
{
    return Load(key, options.GetNumber<int>("texture_size", 512), options.GetString("ansi_codepage", "ISO−8859−1"));
}

bool Font::Release()
{
	return true;
}

bool Font::Load(const std::string& f, int textureSize, const std::string& ansiCodePage)
{
    m_ansiCodePage = ansiCodePage;

    sf::Font font;
    if(!font.loadFromFile(f))
        return false;

    for(int i = 0; i < 256; ++i)
        m_charInfo[i].Set(1, 1, 1, 1, 0, 0, 0);

    sf::RenderTexture texture;
    if(!texture.create(textureSize, textureSize))
        return false;

    texture.clear(sf::Color::Transparent);

    //const std::string cs = Unicode::FromUtf8ToAnsi(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~abcé", ansiCodePage);
    std::string cs(256 - 32, 0);
    for(int i = 32; i < 256; ++i)
        cs[i - 32] = i;

    int charPerSide = sqrt((double)cs.length()) + 1;
    int charSize = textureSize / charPerSide;

    for(int i = 0; i < cs.length(); ++i)
    {
        unsigned char charCode = cs[i];

        int x = i % charPerSide;
        int y = i / charPerSide;

        sf::IntRect bound = font.getGlyph(charCode, charSize, false).bounds;
        if(charCode == ' ')
            bound.width = charSize / 2; // TODO hack

        if(bound.width > charSize * 2) // TODO hack
            continue;

        char ss[2];
        ss[0] = charCode;
        ss[1] = 0;
        sf::String sfStr;
        sfStr = ss;

        sf::Text letter(sf::String(Unicode::FromAnsiToWstring(ss, ansiCodePage)));
        letter.setFont(font);
        letter.setCharacterSize(charSize);
        letter.setColor(sf::Color::White);
        letter.setPosition(x * charSize, y * charSize);
        texture.draw(letter);

        //std::cout << "BOUND: " << (int)(charCode & 0xff) << ", '" << (char)charCode << "', \t" << charSize << ", \t" << bound.left << ", \t" << bound.top << ", \t" << bound.width << ", \t" << bound.height << std::endl;

        float offset = bound.height + bound.top;
        float tx = (float)(x * charSize + bound.left) / (float)textureSize;
        float ty = ((float)((charPerSide - y - 1) * charSize - offset + 1) / (float)textureSize);
        float tw = (float)bound.width / (float)textureSize;
        float th = (float)bound.height / (float)textureSize;


#if 0
        // Overflow under baseline
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(bound.width, bound.height + bound.top));
        rect.setPosition(sf::Vector2f(x * charSize + bound.left, (y + 1) * charSize));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(1);
        texture.draw(rect);

        // Char square
        rect.setSize(sf::Vector2f(bound.width, bound.height));
        rect.setPosition(sf::Vector2f(x * charSize + bound.left, (y + 1) * charSize + bound.top));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(1);
        texture.draw(rect);
#endif

        // x and y position is bottom-left of each letter
        m_charInfo[(unsigned char)charCode].Set(tx, ty, tw, th, bound.width, bound.height, offset);
    }

    // Needed so that the texture is correct, it does'nt actually "display" anything
    texture.display();

    sf::Texture tex = texture.getTexture();
    sf::Image img = tex.copyToImage();
    //img.saveToFile("fontmap.bmp");

    img.flipVertically();
    m_texture.LoadFromMemoryRGBA(f, img.getPixelsPtr(), textureSize, textureSize, true);

    return true;
}

const Font::CharInfo& Font::GetCharInfo(char asciiChar) const
{
    return m_charInfo[(unsigned char)asciiChar];
}

Texture& Font::GetTexture()
{
    return m_texture;
}

std::string Font::GetAnsiCodePage() const
{
    return m_ansiCodePage;
}
