/*
 * WMPlayState.cpp
 *
 *  Created on: Feb 16, 2014
 *      Author: jasper
 */

#include "../jaztec/ErrHandling.hpp"
#include "WMMainState.hpp"

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
	this->camPosition = NULL;
}

void WMPlayState::init(JEngine* engine) {
	PRINT("WMPlayState::init");
	// Load the font.
	this->font = TTF_OpenFont("../font/sans.ttf", 30);
	if (this->font == NULL) {
		throw ERR::Out_Of_Memory();
	}
	// Set some internal variables.
	this->frame = 0;
	this->screenWidth = engine->getScreenWidth();
	this->screenHeight = engine->getScreenHeight();
	// Start the timer so we can keep track.
	this->fps.start();
	// Setup the camera position.
	this->camPosition = new SDL_Rect();
	this->camPosition->h = engine->getScreenHeight();
	this->camPosition->w = engine->getScreenWidth();
	this->camPosition->x = 0;
	this->camPosition->y = 0;
	// Setup the map
	SDL_Surface* mapClips = loadImageFromFile("../gfx/Background.png");
	this->map = new TileManager();
	if (this->map == NULL) {
		throw ERR::Out_Of_Memory();
	}
	this->map->init(mapClips, 12, 12, engine->getScreenWidth(),
			engine->getScreenHeight());
	this->map->makeRandomMap();
	std::cout << "Warmonger play state initialization complete" << std::endl;
	PRINT("WMPlayState::init finish");
}

void WMPlayState::cleanUp() {
	PRINT('WMPlayState::cleanUp');
	if (this->font != NULL) {
		TTF_CloseFont(this->font);
	}
	if (this->map) {
		delete this->map;
		this->map = NULL;
	}
	if (this->camPosition) {
		delete this->camPosition;
		this->camPosition = NULL;
	}
}

void WMPlayState::pause() {

}

void WMPlayState::resume() {

}

void WMPlayState::handleEvent(JEngine* engine, SDL_Event* event) {
	if (event->type == SDL_KEYUP) {
		switch (event->key.keysym.sym) {
		case (SDLK_UP):
			this->camPosition->y += 5;
			break;
		case (SDLK_DOWN):
			this->camPosition->y -= 5;
			break;
		case (SDLK_LEFT):
			this->camPosition->x -= 5;
			break;
		case (SDLK_RIGHT):
			this->camPosition->x += 5;
			break;
		}
	}
}

void WMPlayState::calculate(JEngine * engine) {

}

void WMPlayState::update(JEngine * engine) {
	// Set the camera position.
	this->map->camera.setCamera(this->camPosition->x, this->camPosition->y);
	// Fill the background color.
	this->frame++;
}

void WMPlayState::show(JEngine* engine) {
	SDL_Surface* background = this->map->drawMap();

	SDL_Rect* camView = &this->map->camera.getView();

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

