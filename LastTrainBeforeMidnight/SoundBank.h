#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>

class SoundBank
{
public:
    // charge un buffer (WAV/OGG)
    void load(const std::string& id, const std::string& file);

    // joue un one-shot (pas loop)
    void playOneShot(const std::string& id, float volume = 100.f);

    // lance/relance une boucle (ex: ambiance menu)
    void playLoop(const std::string& id, float volume = 100.f);

    // stoppe une boucle
    void stopLoop(const std::string& id);

    // optionnel : nettoyage
    void garbageCollect();

private:
    std::map<std::string, sf::SoundBuffer> m_buffers;

    // SFML3: un sf::Sound DOIT être construit avec un buffer
    std::map<std::string, sf::Sound>       m_loops;
    std::vector<sf::Sound>                 m_oneshots;
};
