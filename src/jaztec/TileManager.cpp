/**
 * @author Jasper van Herpt
 * @date Jan-2010
 */

unsigned short MAP_SIZE = 0;

#include "TileManager.h"

/**
 * Class Tile
 */

Tile::Tile() {
    this->inner = new SDL_Rect;
    this->typeTile = 0;
    if (this->inner == 0) {
        return;
    }
}

Tile::~Tile() {
    delete this->inner;
}

Uint16 Tile::getType()const {
    return this->typeTile;
}

void Tile::setType(Uint16 type) {
	this->typeTile = type;
}

SDL_Rect& Tile::getInnerRect() {
    return *this->inner;
}

SDL_Rect* Tile::getInnerRect(Uint32 flag) {
    return this->inner;
}

void Tile::setInner(SDL_Rect* newbox) {
    if (this->inner) {
        delete this->inner;
    }
    this->inner = new SDL_Rect;
    if (this->inner == 0) {
        return;
    }
    *this->inner = *newbox;
}

void Tile::show(SDL_Surface* screen, SDL_Surface* clipSheet, Tile* tile) {
    SDL_BlitSurface(clipSheet, tile->getInnerRect(1), screen, this->inner);
}


Camera::Camera() {
    itsView.x = 0;
    itsView.y = 0;
    itsView.w = 0;
    itsView.h = 0;
}

void Camera::setCamera(Sint32 xPos, Sint32 yPos) {
    if (itsView.w < this->getMapWidth()) {
        if ((xPos > 0) && ((xPos + this->itsView.w) <= this->getMapWidth())) {
        	this->itsView.x = xPos;
        } else if (xPos < 0) {
        	this->itsView.x = 0;
        } else if ((xPos + itsView.w) > this->getMapWidth()) {
        	this->itsView.x = this->getMapWidth() - this->itsView.w;
        }
    } else
        itsView.x = 0;

    if (this->itsView.h < this->getMapHeight()) {
        if ((yPos > 0) && ((yPos + this->itsView.h) <= this->getMapHeight())) {
        	this->itsView.y = yPos;
        } else if (yPos < 0) {
        	this->itsView.y = 0;
        } else if ((yPos + this->itsView.h) > this->getMapHeight()) {
        	this->itsView.y = this->getMapHeight() - this->itsView.h;
        }
    } else {
    	this->itsView.y = 0;
    }
}

void Camera::setDimensions(Uint16 camW, Uint16 camH) {
	this->itsView.w = camW;
	this->itsView.h = camH;
}


void TileManager::init(SDL_Surface* itsgraphs, Uint32 xTiles, Uint32 yTiles, Uint32 screenWidth, Uint32 screenHeigth) {

    this->clipSheet = itsgraphs;

    this->itsTiles = new Tile[ MAX_TILES ];
    if (this->itsTiles == 0) {
        return;
    }

    MAP_SIZE = xTiles * yTiles;
    this->mapWidth = xTiles * TILE_WIDTH;
    this->mapHeight = yTiles * TILE_HEIGHT;

    this->camera.setDimensions(screenWidth, screenHeigth);

    this->initGraphics();
    std::cout << "TileManager initiated" << std::endl;
}

void TileManager::cleanUp() {
    std::cout << "Destroying TileManager" << std::endl;
    if (this->mapLoaded) {
        delete [] this->loadedMap;
    }
    if (this->tilesLoaded) {
        delete [] this->itsTiles;
    }
    if (this->clipSheet) {
    	SDL_FreeSurface(this->clipSheet);
    }
}

void TileManager::initGraphics() {
    std::cout << "Initiating graphics\n";
    int depth = 0;
    int height = 0;

    for (int i = 0; i < MAX_TILES; i++) {
        if (depth == 3 * TILE_WIDTH) {
            depth = 0;
            height += TILE_HEIGHT;
        }

        this->itsTiles[ i ].getInnerRect().x = depth;
        this->itsTiles[ i ].getInnerRect().y = height;
        this->itsTiles[ i ].getInnerRect().w = TILE_WIDTH;
        this->itsTiles[ i ].getInnerRect().h = TILE_HEIGHT;

        depth += TILE_WIDTH;
    }

    this->itsTiles[ GRASS_TILE ].setType(GRASS_TILE);
    this->itsTiles[ DIRT_TILE ].setType(DIRT_TILE);
    this->itsTiles[ FLOWER_TILE ].setType(FLOWER_TILE);
    this->itsTiles[ FINISH_TILE ].setType(FINISH_TILE);

    this->tilesLoaded = true;
    std::cout << "Graphics initiated\n";
}

void TileManager::readMap() {
    std::cout << "Reading map...\n" << std::endl << std::endl;
    std::cout << "Map size is " << MAP_SIZE << std::endl;

    for (int i = 0; i < MAP_SIZE; i++) {
        std::cout << "**Reading tile " << i << "**\n";
        std::cout << "\ttype:\t" << loadedMap[ i ].getType() << std::endl;
        std::cout << "\twidth:\t" << this->loadedMap[ i ].getInnerRect().w << std::endl;
        std::cout << "\theight:\t" << this->loadedMap[ i ].getInnerRect().h << std::endl;
        std::cout << "\tx:\t" << this->loadedMap[ i ].getInnerRect().x << std::endl;
        std::cout << "\ty:\t" << this->loadedMap[ i ].getInnerRect().y << std::endl;
    }

    std::cout << "Map reading completed...\n";
}

void TileManager::makeRandomMap() {
    std::cout << "Random map making started\n";
    if (this->mapLoaded) {
        delete [] this->loadedMap;
    }

    this->loadedMap = new Tile[ MAP_SIZE ];
    if (this->loadedMap == 0) {
        std::cout << "Throwing in TileManager::make_random_map after making new tile-array\n";
        throw ( ERR::Out_Of_Memory());
    }

    bool finishPlaced = false;
    unsigned int finish = rand() % MAP_SIZE;
    std::cout << "Finish tile is set on " << finish << std::endl;

    unsigned int numberExtraTiles = MAP_SIZE / 5;

    unsigned int dirtTiles[ numberExtraTiles ];
    unsigned int flowerTiles[ numberExtraTiles ];

    for (unsigned int j = 0; j < numberExtraTiles; j++) {
        dirtTiles[ j ] = rand() % MAP_SIZE;
        flowerTiles[ j ] = rand() % MAP_SIZE;
    }

    unsigned int depth = 0;
    unsigned int height = 0;

    for (unsigned int i = 0; i < MAP_SIZE; i++) {
        bool tile_placed = false;

        if (depth == this->getMapWidth()) {
            height += TILE_HEIGHT;
            depth = 0;
        }
        for (unsigned int k = 0; k < numberExtraTiles; k++) {
            if (i == dirtTiles[ k ]) {
            	this->loadedMap[ i ].setType(DIRT_TILE);
                tile_placed = true;
            }
            if (i == flowerTiles[ k ]) {
            	this->loadedMap[ i ].setType(FLOWER_TILE);
                tile_placed = true;
            }
        }
        if (i == finish) {
        	this->loadedMap[ i ].setType(FINISH_TILE);
            finishPlaced = true;
            tile_placed = true;
        }
        if (tile_placed == false)
        	this->loadedMap[ i ].setType(GRASS_TILE);

        this->loadedMap[ i ].getInnerRect().x = depth;
        this->loadedMap[ i ].getInnerRect().y = height;
        this->loadedMap[ i ].getInnerRect().w = TILE_WIDTH;
        this->loadedMap[ i ].getInnerRect().h = TILE_HEIGHT;

        depth += TILE_WIDTH;
    }

    //read_map(); // Remove comment to write the map configuration to the output buffer.

    this->mapLoaded = true;
    std::cout << "Made random map\n";
}

void TileManager::loadMap(std::string) {
}

bool TileManager::isInView(Tile* tile) {
    SDL_Rect cam = camera.getView();
    SDL_Rect& tileB = tile->getInnerRect();

    if ((tileB.y + tileB.h) < cam.y ||
            tileB.y > (cam.y + cam.h) ||
            (tileB.x + tileB.w) < cam.x ||
            tileB.x > (cam.x + cam.w)) {
        return false;
    }

    return true;
}

SDL_Surface* TileManager::drawMap() {
    if (mapLoaded) {
        SDL_Surface* screen = SDL_CreateRGBSurface(SDL_HWSURFACE, this->getMapWidth(), this->getMapHeight(), 32, 0, 0, 0, 0);
        if (screen == 0) {
            return NULL;
        }
        for (int i = 0; i < MAP_SIZE; i++) {
            if (this->isInView(&loadedMap[ i ])) {
                loadedMap[ i ].show(screen, clipSheet, &itsTiles[ loadedMap[ i ].getType() - 1 ]);
            }
        }
        return screen;
    }
    return NULL;
}

Tile* TileManager::getFinishTile() {
    if (this->mapLoaded == false) {
        return NULL;
    }

    for (int i = 0; i < MAP_SIZE; i++) {
        if (this->loadedMap[ i ].getType() == FINISH_TILE)
            return &this->loadedMap[ i ];
    }

    return NULL;
}

bool TileManager::checkTilesLoaded() const {
    return this->tilesLoaded;
}

bool TileManager::checkMapLoaded() const {
    return this->mapLoaded;
}

Uint32 TileManager::getMapWidth() const {
    return this->mapWidth;
}

Uint32 TileManager::getMapHeight() const {
    return this->mapHeight;
}

void Camera::setMapWidth(Uint32 width) {
	this->mapWidth = width;
}

void Camera::setMapHeight(Uint32 height) {
	this->mapHeight = height;
}

Uint32 Camera::getMapWidth() const {
	return this->mapWidth;
}

Uint32 Camera::getMapHeight() const {
	return this->mapHeight;
}
