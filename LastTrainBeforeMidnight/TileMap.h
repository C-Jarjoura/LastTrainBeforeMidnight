#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Simple tile map (CSV loader) for SFML 3.0.2
// - tileset is a single image with tiles laid out horizontally (64x64 each)
// - CSV format uses commas: 0,1,2,3
// - Each tile index selects the tile at column = index
class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    explicit TileMap(const std::string& tilesetPath, unsigned tileSize);

    // Load map data from a CSV file (comma-separated integers per row)
    bool loadFromCSV(const std::string& csvPath);

    unsigned getTileSize() const { return m_tileSize; }
    unsigned getWidth()    const { return m_width; }
    unsigned getHeight()   const { return m_height; }

private:
    void buildVertices(const std::vector<int>& data);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Texture     m_tileset;     // constructed directly from file (SFML 3 style)
    sf::VertexArray m_vertices;    // sf::PrimitiveType::Quads
    unsigned        m_tileSize = 64;
    unsigned        m_width = 0;  // in tiles
    unsigned        m_height = 0;  // in tiles
};
