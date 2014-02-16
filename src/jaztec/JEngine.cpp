
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
    while (!gameStates.empty()) {
        gameStates.back()->cleanUp();
        gameStates.pop_back();
    }

    // Close SDL.
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    printf("Game engine terminated\n");
}

/**
 * Change the active game state.
 */
void JEngine::changeState(JGameState* gameState) {
    PRINT("JEngine::change_state");
    // Close any existing game state.
    if (!gameStates.empty()) {
        gameStates.back()->cleanUp();
        gameStates.pop_back();
    }

    // Setup the new game state and initiate it.
    gameStates.push_back(gameState);
    gameStates.back()->init(this);

}

/**
 * Pause the currently loaded game state and push one above,
 */
void JEngine::pushState(JGameState* gameState) {
    PRINT("JEngine::push_state");
    // Close any existing game state.
    if (!gameStates.empty()) {
        gameStates.back()->pause();
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
    if (!gameStates.empty()) {
        gameStates.back()->cleanUp();
        gameStates.pop_back();
    }

    // If another game state remains resume this state.
    if (!gameStates.empty()) {
        gameStates.back()->resume();
    }

}

/**
 * Call handle events in the loaded game state.
 */
void JEngine::handleEvents() {
    if (!gameStates.empty()) {
        gameStates.back()->handleEvents(this);
    }
}

/**
 * Calculate new moves.
 */
void JEngine::calculate() {
    if (!gameStates.empty()) {
        gameStates.back()->calculate(this);
    }
}

/**
 * Update the objects.
 */
void JEngine::update() {
    if (!gameStates.empty()) {
        gameStates.back()->update(this);
    }
}

/**
 * Show the currently loaded game state.
 */
void JEngine::show() {
    if (!gameStates.empty()) {
        gameStates.back()->show(this);
    }
}

/**
 * Set the game in full screen or get it out of fullscreen.
 */
void JEngine::toggleFullscreen() {
    unsigned int flag = 0x00000000;

    if (!this->isFullscreen()) {
    	// Set flags.
        i_settings |= ISFULLSCREEN;
        flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_FULLSCREEN;
    } else {
    	// Set flags
        i_settings &= ~ISFULLSCREEN;
        flag = SDL_HWSURFACE | SDL_HWACCEL | SDL_DOUBLEBUF | SDL_RESIZABLE;
    }

    // Reset the screen.
    screen = SDL_SetVideoMode(screen->clip_rect.w, screen->clip_rect.h, 0, flag);

}

