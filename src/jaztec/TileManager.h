
#ifndef _JAS_TILE_MANAGER_H_
#define _JAS_TILE_MANAGER_H_

/**
 * @author Jasper van Herpt
 * @date 2014-2-16
 */

#ifdef __WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include "ErrHandling.hpp"

#include <iostream>
#include <fstream>
#include <string>

#define		MAX_TILES		0x00000004
#define		TILE_WIDTH		0x00000096
#define		TILE_HEIGHT		0x00000096

#define		GRASS_TILE		0x0001
#define		DIRT_TILE		0x0002
#define		FLOWER_TILE		0x0003
#define		FINISH_TILE		0x0004

class Tile {
private:
    SDL_Rect* inner;
    Uint16 typeTile;

public:
    Tile();
    ~Tile();

    Uint16 getType()const;
    void setType(Uint16);
    SDL_Rect& getInnerRect();
    SDL_Rect* getInnerRect(Uint32);
    void setInner(SDL_Rect*);
    void show(SDL_Surface*, SDL_Surface*, Tile*);

};

class Camera {
private:
    SDL_Rect itsView;
    Uint32 mapWidth;
    Uint32 mapHeight;

protected:

public:
    Camera();

    ~Camera() {
    }

    void setCamera(Sint32 xPos, Sint32 yPos);
    void setDimensions(Uint16 camW, Uint16 camH);

    SDL_Rect getView() const {
        return this->itsView;
    }

    void setMapWidth(Uint32 width);
    void setMapHeight(Uint32 height);

    Uint32 getMapWidth() const;
    Uint32 getMapHeight() const;
};

class TileManager {
private:
    Tile* itsTiles;
    Tile* loadedMap;

    bool tilesLoaded;
    bool mapLoaded;

    Uint32 mapWidth;
    Uint32 mapHeight;

    SDL_Surface* clipSheet;

    void initGraphics();
    void readMap();
    bool isInView(Tile* tile);

public:

    TileManager() {
    	this->clipSheet = NULL;
    	this->itsTiles = NULL;
    	this->loadedMap = NULL;
    	this->mapLoaded = false;
    	this->tilesLoaded = false;
    }

    ~TileManager() {
    }

    void init(SDL_Surface* itsgraphs, Uint32 xTiles, Uint32 yTiles, Uint32 screenWidth, Uint32 screenHeigth);
    void cleanUp();

    void makeRandomMap();
    void loadMap(std::string);

    SDL_Surface* drawMap();
    Tile* getFinishTile();

    bool checkTilesLoaded()const;
    bool checkMapLoaded()const;

    Uint32 getMapWidth() const;
    Uint32 getMapHeight() const;

    Camera camera;
};

#endif
