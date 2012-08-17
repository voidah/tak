#ifndef TAK_SOUND_H__
#define TAK_SOUND_H__

#include "define.h"
#include <SFML/Audio.hpp>
#include "resource.h"
#include "vector3.h"

class Sound : public Resource<Sound>
{
    public:
        Sound();
        virtual ~Sound();

        virtual bool Load(const std::string& key, const OptionList& options);
        virtual bool Release();

        sf::SoundBuffer& GetInternalBuffer();

    private:
        bool m_valid;
        sf::SoundBuffer m_soundBuffer;
};

#endif // TAK_SOUND_H__
