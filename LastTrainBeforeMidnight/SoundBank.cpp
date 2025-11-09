#include "SoundBank.h"
#include <algorithm>

void SoundBank::load(const std::string& id, const std::string& file)
{
    m_buffers.emplace(id, sf::SoundBuffer(file));
}

void SoundBank::playOneShot(const std::string& id, float volume)
{
    auto it = m_buffers.find(id);
    if (it == m_buffers.end()) return;

    // chercher un slot libre
    for (auto& s : m_oneshots)
    {
        if (s.getStatus() == sf::Sound::Status::Stopped)
        {
            // SFML3 -> impossible de changer le buffer d’un sf::Sound
            // donc on recrée
            s = sf::Sound(it->second);
            s.setVolume(volume);
            s.setLooping(false);
            s.play();
            return;
        }
    }

    // créer nouveau
    m_oneshots.emplace_back(it->second);
    auto& s = m_oneshots.back();
    s.setVolume(volume);
    s.setLooping(false);
    s.play();
}

void SoundBank::playLoop(const std::string& id, float volume)
{
    auto it = m_buffers.find(id);
    if (it == m_buffers.end()) return;

    auto lit = m_loops.find(id);
    if (lit == m_loops.end())
    {
        // créer une nouvelle boucle
        sf::Sound snd(it->second);
        snd.setVolume(volume);
        snd.setLooping(true);
        snd.play();
        m_loops.emplace(id, std::move(snd));
    }
    else
    {
        auto& snd = lit->second;
        snd.setVolume(volume);
        snd.setLooping(true);
        if (snd.getStatus() != sf::Sound::Status::Playing)
            snd.play();
    }
}

void SoundBank::stopLoop(const std::string& id)
{
    auto it = m_loops.find(id);
    if (it != m_loops.end())
        it->second.stop();
}

void SoundBank::garbageCollect()
{
    m_oneshots.erase(
        std::remove_if(m_oneshots.begin(), m_oneshots.end(),
            [](const sf::Sound& s) { return s.getStatus() == sf::Sound::Status::Stopped; }),
        m_oneshots.end()
    );
}
