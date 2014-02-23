
#include <vector>
#include <deque>
#include <string>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include "ErrHandling.hpp"
#include "JEngine.h"
#include "JGameState.h"

/**
 * Initiate the game engine
 * 
 * @param char* title
 * @param int width
 * @param int height
 * @param int bpp
 * @param Uint32 flags
 */
void JEngine::init(const char* title, Uint32 width, Uint32 height, Uint32 bpp, Uint32 flags) {
    Uint32 flag;

    i_settings = flags;

    // Init SDL libs.
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        throw ( ERR::SDL_Err());
    }
    if (TTF_Init() == -1) {
        throw ( ERR::SDL_Err());
    }
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) == -1) {
        throw ( ERR::SDL_Err());
    }

    // Set the screen caption.
    SDL_WM_SetCaption(title, title);

    // Setup launch flags
    if ((i_settings & ISFULLSCREEN) == ISFULLSCREEN) {
        flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_FULLSCREEN;
    } else {
        flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;
    }

    // Create the screen.
    this->screen = SDL_SetVideoMode(width, height, bpp, flag);

    this->startRunning();

    printf("Game engine fired up\n");

}

/**
 * Close SDL and release resources.
 */
void JEngine::cleanUp() {
    PRINT("JEngine::clean_up");
    // Set the screen to smaller mode. Otherwise it can get stuck after an error.
    screen = SDL_SetVideoMode(640, 480, 0, SDL_RESIZABLE);
    SDL_Flip(screen);

    // Release the loaded game states.
    while (!this->gameStates.empty()) {
    	this->gameStates.back()->cleanUp();
    	this->gameStates.pop_back();
    }

    // Close SDL.
    std::cout << "Closing audio" << std::endl;
    Mix_CloseAudio();
    std::cout << "Closing font" << std::endl;
    TTF_Quit();
    std::cout << "Closing SDL" << std::endl;
    SDL_Quit();

    printf("Game engine terminated\n");
}

/**
 * Change the active game state.
 */
void JEngine::changeState(JGameState* gameState) {
    PRINT("JEngine::change_state");
    // Close any existing game state.
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->cleanUp();
    	this->gameStates.pop_back();
    }

    // Setup the new game state and initiate it.
    this->gameStates.push_back(gameState);
    this->gameStates.back()->init(this);

}

/**
 * Pause the currently loaded game state and push one above,
 */
void JEngine::pushState(JGameState* gameState) {
    PRINT("JEngine::push_state");
    // Close any existing game state.
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->pause();
    }

    // Setup the new game state and initiate it.
    gameStates.push_back(gameState);
    gameStates.back()->init(this);

}

/**
 * Clean the currently loaded game state. If any paused state is loaded resume this.
 */
void JEngine::popState() {
    PRINT("JEngine::pop_state");
    // Close any existing game state.
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->cleanUp();
    	this->gameStates.pop_back();
    }

    // If another game state remains resume this state.
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->resume();
    }

}

/**
 * Call handle events in the loaded game state.
 */
void JEngine::handleEvents() {
	// Handle the SDL events.
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		// And issue to the game states.
	    if (!this->gameStates.empty()) {
	    	this->gameStates.back()->handleEvent(this, &event);
	    }

	    if (this->stopEventHandling()) {
	    	break;
	    }

		switch (event.type) {
		case (SDL_QUIT):
			this->exit();
		break;
		case (SDL_VIDEORESIZE):
			this->toggleFullscreen();
		break;
		case (SDL_KEYDOWN):
			switch(event.key.keysym.sym) {
			case (SDLK_ESCAPE):
				this->exit();
			break;
			case (SDLK_F4):
				this->toggleFullscreen();
			break;
			}
		}
	}
}

/**
 * Calculate new moves.
 */
void JEngine::calculate() {
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->calculate(this);
    }
}

/**
 * Update the objects.
 */
void JEngine::update() {
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->update(this);
    }
}

/**
 * Show the currently loaded game state.
 */
void JEngine::show() {
    if (!this->gameStates.empty()) {
    	this->gameStates.back()->show(this);
    }
}

/**
 * Set the game in full screen or get it out of fullscreen.
 */
void JEngine::toggleFullscreen() {
    unsigned int flag = 0x00000000;

    if (!this->isFullscreen()) {
    	// Set flags.
    	this->i_settings |= ISFULLSCREEN;
        flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_FULLSCREEN;
    } else {
    	// Set flags
    	this->i_settings &= ~ISFULLSCREEN;
        flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;
    }

    // Reset the screen.
    this->screen = SDL_SetVideoMode(screen->clip_rect.w, screen->clip_rect.h, 0, flag);

}

/**
 * Set the executable path of the application for file reference.
 * @param std::string path
 */
void JEngine::setBasePath(std::string path)
{
    this->basePath = path;
}

/**
 * Retrieve the base path of the executable
 * @return std::string
 */
std::string JEngine::getBasePath() const
{
    return this->basePath;
}
