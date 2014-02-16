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

WMPlayState::WMPlayState()
{
	this->font = NULL;
	this->frame = 0;
	this->screenHeight = 0;
	this->screenWidth = 0;
}

void WMPlayState::init(JEngine* engine)
{
	// Load the font.
	this->font = TTF_OpenFont("obj/sans.ttf", 30);
	if (this->font == NULL) {
		throw ERR::Out_Of_Memory();
	}
	// Set some internal variables.
	this->frame = 0;
	this->screenWidth = engine->getScreenWidth();
	this->screenHeight = engine->getScreenHeight();
	// Create the background of our state.
	this->background = SDL_CreateRGBSurface(SDL_HWSURFACE, this->screenWidth, this->screenHeight, 32, 0, 0, 0, 0);
	// Start the timer so we can keep track.
	this->fps.start();
	// Setup the map
	SDL_Surface* mapClips = loadImageFromFile("gfx/Background.png");
	this->map = new TileManager();
	if (this->map == NULL) {
		throw ERR::Out_Of_Memory();
	}
	this->map->init(mapClips, 8, 6, engine->getScreenWidth(), engine->getScreenHeight());
}

void WMPlayState::cleanUp()
{
	if (this->font != NULL) {
		TTF_CloseFont(this->font);
	}
	if (this->background != NULL) {
		SDL_FreeSurface(this->background);
	}
	if (this->map) {
		delete this->map;
	}
}

void WMPlayState::pause()
{

}

void WMPlayState::resume()
{

}

void WMPlayState::handleEvent(JEngine* engine, SDL_Event* event)
{

}

void WMPlayState::calculate(JEngine * engine)
{

}

void WMPlayState::update(JEngine * engine)
{
	// Fill the background color.
	SDL_FillRect(this->background, &this->background->clip_rect, SDL_MapRGB(this->background->format, 0x00, 0x00, 0x00));
}

void WMPlayState::show(JEngine* engine)
{
    SDL_BlitSurface(this->background, NULL, engine->screen, &engine->screen->clip_rect);

    if (SDL_Flip(engine->screen) == -1) {
        throw ( ERR::SDL_Err());
    }

    if (this->fps.getTicks() < 1000 / engine->getFps()) {
        SDL_Delay((1000 / engine->getFps()) - this->fps.getTicks());
    }

    this->frame++;
}


