#include <iostream>
#include <cmath>

#include "tileset.h"

TileSet::TileSet(size_t ntheta, size_t nphi)
{
    const float dtheta = M_PI / ntheta;
    const float dphi = 2.f * M_PI / nphi;

    std::vector<TileElement*> _tiles((ntheta-2) * nphi);

    // First, create all elements
    for(size_t itheta = 1; itheta+1 < ntheta; ++itheta)
    {
        Tile tile;
        tile.thetaa = itheta * dtheta;
        tile.thetab = (itheta+1) * dtheta;

        for(size_t iphi = 0; iphi < nphi; ++iphi)
        {
            tile.phia = iphi * dphi;
            tile.phib = (iphi+1) * dphi;

            size_t offset = (itheta-1) * nphi;
            size_t index = offset + iphi;
            _tiles[index] = tiles.AddElement(tile);
        }
    }

    // Then, link them together
    for(size_t itheta = 1; itheta+1 < ntheta; ++itheta)
    {
        for(size_t iphi = 0; iphi < nphi; ++iphi)
        {
            size_t offset = (itheta-1) * nphi;
            size_t index = offset + iphi;
            size_t index_right = offset + (iphi == nphi-1 ? 0 : iphi+1);

            TileElement *tile = _tiles[index];
            tiles.LinkElements(tile, _tiles[index_right]);
            if(itheta != 1)
            {
                size_t index_up = index - nphi;
                tiles.LinkElements(tile, _tiles[index_up]);
            }
        }
    }
}

TileElement *TileSet::GetRandomTile()
{
    return tiles.GetRandomElement();
}

void TileSet::RemoveTile(TileElement *tile)
{
    tiles.RemoveElement(tile);
}

