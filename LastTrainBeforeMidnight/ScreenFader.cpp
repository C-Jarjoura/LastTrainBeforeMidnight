#include "ScreenFader.h"

ScreenFader::ScreenFader()
{
    m_alpha = 255.f;
    m_speed = 400.f;    // pixels alpha/sec
    m_active = false;
    m_isOut = false;

    m_rect.setSize({ 1920.f,1080.f });
    m_rect.setFillColor(sf::Color(0, 0, 0, 255));
}

void ScreenFader::startFadeOut()
{
    m_isOut = true;
    m_active = true;
}

void ScreenFader::startFadeIn()
{
    m_isOut = false;
    m_active = true;
}

void ScreenFader::update(float dt)
{
    if (!m_active) return;

    float dir = m_isOut ? 1.f : -1.f;
    m_alpha += dir * m_speed * dt;

    if (m_alpha > 255.f) { m_alpha = 255.f; m_active = false; }
    if (m_alpha < 0.f) { m_alpha = 0.f; m_active = false; }

    m_rect.setFillColor(sf::Color(0, 0, 0, (uint8_t)m_alpha));
}

void ScreenFader::draw(sf::RenderWindow& window)
{
    window.draw(m_rect);
}
