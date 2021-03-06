#ifndef TILESET_H
#define TILESET_H

#include <vector>
#include <set>

#include "randutils.h"
#include "multilist.h"

struct Tile
{
    float thetaa;
    float phia;
    float thetab;
    float phib;

    Tile():
        thetaa(0.f),
        phia(0.f),
        thetab(0.f),
        phib(0.f)
    {
    }

    Tile(const Tile &other):
        thetaa(other.thetaa),
        phia(other.phia),
        thetab(other.thetab),
        phib(other.phib)
    {
    }
};

typedef MultiElement<Tile> TileElement;

class TileSet
{
    public:
        TileSet(size_t ntheta, size_t nphi);

        TileElement *GetRandomTile();
        void RemoveTile(TileElement *tile);

    private:
        MultiList<Tile> tiles;
};

#endif

