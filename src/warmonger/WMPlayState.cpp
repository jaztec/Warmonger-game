/*
 * WMPlayState.cpp
 *
 *  Created on: Feb 16, 2014
 *      Author: jasper
 */

#include "../jaztec/ErrHandling.hpp"
#include "../jaztec/JEngine.h"
#include "WMMainState.hpp"
#include "WMCamera.hpp"

#include <fstream>
#include <sstream>

using namespace jasdlib;

/**
 * Initiate the static state instance.
 */
WMPlayState WMPlayState::WarmongerPlayState;

WMPlayState::WMPlayState() {
	this->font = NULL;
	this->frame = 0;
	this->screenHeight = 0;
	this->screenWidth = 0;
	this->map = NULL;
	this->camera = NULL;
}

void WMPlayState::init(JEngine* engine) {
	PRINT("WMPlayState::init");
	// Load the font.
    std::string fontPath = getAbsolutePath(engine->getBasePath() + "/../font/sans.ttf");
    std::cout << fontPath.c_str();
	this->font = TTF_OpenFont(fontPath.c_str(), 30);
	if (this->font == NULL) {
        std::cout << "Not able to open font" << std::endl;
		throw ERR::Out_Of_Memory();
	}
	// Set some internal variables.
	this->frame = 0;
	this->screenWidth = engine->getScreenWidth();
	this->screenHeight = engine->getScreenHeight();
	// Start the timer so we can keep track.
	this->fps.start();
	// Setup the camera position.
    Camera* camera = new WMCamera();
    this->camera = camera;
	// Setup the map
    std::string clipsPath = getAbsolutePath(engine->getBasePath() + "/../gfx/Background.png");
	SDL_Surface* mapClips = loadImageFromFile(clipsPath.c_str());
	this->map = new TileManager();
	if (this->map == NULL) {
		throw ERR::Out_Of_Memory();
	}
    this->map->setCamera(camera);
	this->map->init(mapClips, 12, 12, engine->getScreenWidth(),
			engine->getScreenHeight());
	this->map->makeRandomMap();
}

void WMPlayState::cleanUp() {
	PRINT("WMPlayState::cleanUp");
	if (this->font != NULL) {
		TTF_CloseFont(this->font);
	}
	if (this->map) {
		delete this->map;
		this->map = NULL;
	}
}

void WMPlayState::pause() {

}

void WMPlayState::resume() {

}

void WMPlayState::handleEvent(JEngine* engine, SDL_Event* event) {
	if (event->type == SDL_KEYDOWN) {
		switch (event->key.keysym.sym) {
		case (SDLK_UP):
			this->camera->setYSpeed(-5.0f);
			break;
		case (SDLK_DOWN):
			this->camera->setYSpeed(5.0f);
			break;
		case (SDLK_LEFT):
			this->camera->setXSpeed(-5.0f);
			break;
		case (SDLK_RIGHT):
			this->camera->setXSpeed(5.0f);
			break;
		}
	} else if (event->type == SDL_KEYUP) {
		switch (event->key.keysym.sym) {
		case (SDLK_UP):
		case (SDLK_DOWN):
			this->camera->setYSpeed(0.0f);
			break;
		case (SDLK_LEFT):
		case (SDLK_RIGHT):
			this->camera->setXSpeed(0.0f);
			break;
		}
    }
}

void WMPlayState::calculate(JEngine * engine) {

}

void WMPlayState::update(JEngine * engine) {
	this->camera->update();
    // Keep track of the frames.
	this->frame++;
}

void WMPlayState::show(JEngine* engine) {
	SDL_Surface* background = this->map->drawMap();

	SDL_Rect* camView = &this->map->getCamera()->getView();

	SDL_BlitSurface(background, camView, engine->screen,
			&engine->screen->clip_rect);

	if (SDL_Flip(engine->screen) == -1) {
		throw(ERR::SDL_Err());
	}

	if (this->fps.getTicks() < 1000 / engine->getFps()) {
		SDL_Delay((1000 / engine->getFps()) - this->fps.getTicks());
	}

	SDL_FreeSurface(background);
}

