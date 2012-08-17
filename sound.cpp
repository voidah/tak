#include "sound.h"
#include <fstream>
#include <cassert>

Sound::Sound() : m_valid(false)
{
}

Sound::~Sound()
{
}

bool Sound::Load(const std::string& key, const OptionList& options)
{
    m_valid = m_soundBuffer.loadFromFile(key);
    return m_valid;
}

bool Sound::Release()
{
    return true;
}

sf::SoundBuffer& Sound::GetInternalBuffer()
{
    assert(m_valid);

    return m_soundBuffer;
}
