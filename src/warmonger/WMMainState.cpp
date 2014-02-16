/**
 * @author Jasper van Herpt
 * @date 2014-2-16
 */

#include "../jaztec/ErrHandling.hpp";
#include "WMMainState.hpp";

#include <fstream>
#include <sstream>

using namespace jasdlib;

/**
 * Initiate the static state instance.
 */
WMMainState WMMainState::WarmongerMainState;

void WMMainState::init(JEngine* engine)
{
	// Load the font.
	this->font = TTF_OpenFont("obj.sans.ttf", 30);
	if (this->font == NULL) {
		throw ERR::Out_Of_Memory();
	}
	// Set some internal variables.
	this->frame = 0;
	this->screenWidth = engine->getScreenWidth();
	this->screeHeight = engine->getScreenHeight()();
	// Start the timer so we can keep track.
	this->fps.start();
	// Create the background of our state.
	this->background = SDL_CreateRGBSurface(SDL_HWSURFACE, this->screenWidth, this->screeHeight, 32, 0, 0, 0, 0);
	// Load the logo for the background.
	this->logo = loadImageFromFile("gfx/MainHeader.png");
}

void WMMainState::cleanUp()
{
	if (this->font != NULL) {
		TTF_CloseFont(this->font);
	}
	if (this->background != NULL) {
		SDL_FreeSurface(this->background);
	}
	if (this->playButton != NULL) {
		delete this->playButton;
	}
	if (this->quitButton != NULL) {
		delete this->quitButton;
	}
	if (this->resumeButton != NULL) {
		delete this->resumeButton;
	}
}

void WMMainState::pause()
{

}

void WMMainState::resume()
{

}

void WMMainState::handleEvents(JEngine* engine)
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		// Test for the button events.
		if (this->playButton->handleEvents(&event)) {
			return;
		}
		if (this->quitButton->handleEvents(&event)) {
			engine->exit();
			return;
		}
		if (this->resumeButton->handleEvents(&event)) {
			engine->popState();
			return;
		}
		// Test for other events.
		switch (event.type) {
		case (SDL_QUIT):
				engine->exit();
		break;
		case (SDL_VIDEORESIZE):
				engine->toggleFullscreen();
		break;
		case (SDL_KEYDOWN):
				switch(event.key.keysym.sym) {
				case (SDLK_ESCAPE):
						engine->exit();
				break;
				case (SDLK_F4):
						engine->toggleFullscreen();
				break;
				}
		}
	}
}
