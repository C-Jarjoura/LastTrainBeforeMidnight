#include "TileMap.h"

#include <fstream>
#include <sstream>

TileMap::TileMap(const std::string& tilesetPath, unsigned tileSize)
    : m_tileset(tilesetPath),
    m_vertices(sf::PrimitiveType::Triangles),
    m_tileSize(tileSize)
{
}

bool TileMap::loadFromCSV(const std::string& csvPath)
{
    std::ifstream in(csvPath);
    if (!in.is_open())
        return false;

    std::vector<int> data;
    m_width = 0;
    m_height = 0;

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string cell;
        unsigned rowCount = 0;
        while (std::getline(ss, cell, ','))
        {
            if (!cell.empty())
            {
                data.push_back(std::stoi(cell));
                ++rowCount;
            }
        }
        if (rowCount > 0)
        {
            if (m_width == 0) m_width = rowCount;
            ++m_height;
        }
    }

    if (m_width == 0 || m_height == 0)
        return false;

    buildVertices(data);
    return true;
}

void TileMap::buildVertices(const std::vector<int>& data)
{
    const unsigned tilesPerRow = m_tileset.getSize().x / m_tileSize;

    // *** 6 vertices par tile (2 triangles)
    m_vertices.clear();
    m_vertices.resize(m_width * m_height * 6);

    for (unsigned y = 0; y < m_height; ++y)
    {
        for (unsigned x = 0; x < m_width; ++x)
        {
            const unsigned i = x + y * m_width;
            const int tileIndex = data[i];

            const unsigned tu = static_cast<unsigned>(tileIndex) % tilesPerRow;
            const unsigned tv = static_cast<unsigned>(tileIndex) / tilesPerRow;

            sf::Vertex* tri = &m_vertices[i * 6];

            float px = (float)(x * m_tileSize);
            float py = (float)(y * m_tileSize);

            float tx = (float)(tu * m_tileSize);
            float ty = (float)(tv * m_tileSize);

            // triangle 1
            tri[0].position = { px,                  py };
            tri[1].position = { px + m_tileSize,     py };
            tri[2].position = { px + m_tileSize,     py + m_tileSize };

            // triangle 2
            tri[3].position = { px,                  py };
            tri[4].position = { px + m_tileSize,     py + m_tileSize };
            tri[5].position = { px,                  py + m_tileSize };

            // UV coords
            tri[0].texCoords = { tx,                 ty };
            tri[1].texCoords = { tx + m_tileSize,    ty };
            tri[2].texCoords = { tx + m_tileSize,    ty + m_tileSize };
            tri[3].texCoords = { tx,                 ty };
            tri[4].texCoords = { tx + m_tileSize,    ty + m_tileSize };
            tri[5].texCoords = { tx,                 ty + m_tileSize };
        }
    }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}
