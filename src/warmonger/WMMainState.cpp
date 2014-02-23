/**
 * @author Jasper van Herpt
 * @date 2014-2-16
 */

#include "../jaztec/ErrHandling.hpp"
#include "WMMainState.hpp"

#include <fstream>
#include <sstream>

using namespace jasdlib;

/**
 * Initiate the static state instance.
 */
WMMainState WMMainState::WarmongerMainState;

WMMainState::WMMainState()
{
	this->font = NULL;
	this->background = NULL;
	this->playButton = NULL;
	this->resumeButton = NULL;
	this->quitButton = NULL;
	this->logo = NULL;
	this->frame = 0;
	this->screenHeight = 0;
	this->screenWidth = 0;
}

void WMMainState::init(JEngine* engine)
{
	// Load the font.
	this->font = TTF_OpenFont("../font/sans.ttf", 30);
	if (this->font == NULL) {
		throw ERR::Out_Of_Memory();
	}
	// Set some internal variables.
	this->frame = 0;
	this->screenWidth = engine->getScreenWidth();
	this->screenHeight = engine->getScreenHeight();
	this->playButton = new Button("Start game", this->font, 370, 510, BUTTON_ENABLED);
	this->quitButton = new Button("Quit game", this->font, 370, 600, BUTTON_ENABLED);
	this->resumeButton = new Button("Resume game", this->font, 370, 510, BUTTON_ENABLED);
	// Start the timer so we can keep track.
	this->fps.start();
	// Create the background of our state.
	this->background = SDL_CreateRGBSurface(SDL_HWSURFACE, this->screenWidth, this->screenHeight, 32, 0, 0, 0, 0);
	// Load the logo for the background.
	this->logo = loadImageFromFile("../gfx/MainHeader.png");
}

void WMMainState::cleanUp()
{
	PRINT("WMMainState::cleanUp");
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

void WMMainState::handleEvent(JEngine* engine, SDL_Event* event)
{
	// Test for the button events.
	if (this->playButton->handleEvents(event)) {
		engine->stopEventHandling(true);
		engine->changeState(WMPlayState::instance());
		return;
	}
	if (this->quitButton->handleEvents(event)) {
		engine->stopEventHandling(true);
		engine->exit();
		return;
	}
	if (this->resumeButton->handleEvents(event)) {
		engine->stopEventHandling(true);
		engine->popState();
		return;
	}
}

void WMMainState::calculate(JEngine * engine)
{

}

void WMMainState::update(JEngine * engine)
{
	// Fill the background color.
	SDL_FillRect(this->background, &this->background->clip_rect, SDL_MapRGB(this->background->format, 0x00, 0x00, 0x00));

	// Render the logo to the screen.
	SDL_Rect position;
	position.x = (this->screenWidth - this->logo->clip_rect.w) / 2;
	position.y = 50;
	position.w = this->logo->clip_rect.w;
	position.h = this->logo->clip_rect.h;
	SDL_BlitSurface(this->logo, NULL, this->background, &position);
	this->resumeButton->enabled(false);
	this->playButton->enabled(true);
	this->quitButton->enabled(true);
	// Test if the resume button should be on instead of the play button.
	if (engine->getStateCount() > 1) {
		this->resumeButton->enabled(true);
		this->playButton->enabled(false);
	}
    if (this->playButton->enabled()) {
    	this->playButton->draw(this->background);
    }
    if (this->quitButton->enabled()) {
    	this->quitButton->draw(this->background);
    }
    if (this->resumeButton->enabled()) {
    	this->resumeButton->draw(this->background);
    }
}

void WMMainState::show(JEngine* engine)
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
