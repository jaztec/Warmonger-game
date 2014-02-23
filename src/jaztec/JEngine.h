
#ifndef _JAS_GAME_ENGINE_H_
#define _JAS_GAME_ENGINE_H_

#ifdef __WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#endif /* __WIN32 */

#include <vector>
#include <deque>
#include <string>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include "ErrHandling.hpp"

#define	ISRUNNING				0x00000001
#define ISFULLSCREEN            0x00000002
#define STOPEVENTHANDLING		0x00000004

class JGameState;


class JEngine {
private:

	/**
	 * @var std::vector<JGameState*> gameStates All loaded game states.
	 */
    std::vector<JGameState*> gameStates;
    
    /**
     * @var std::string basePath
     */
    std::string basePath;

    /**
     * @var Uint8 i_settings Placeholder integer to hold settings.
     */
    Uint32 i_settings;
    Uint16 fps;

    /**
     * Start the engine.
     */
    void startRunning() {
        this->i_settings |= ISRUNNING;
    }

public:

    /**
     * @var SDL_Surface* screen The main screen of the game.
     */
    SDL_Surface* screen;

    /**
     * Initiate the engine and setup the SDL environment.
     * @param char* title
     * @param Uint32 width
     * @param Uint32 height
     * @param Uint32 bpp
     * @param Uint32 flags
     */
    void init(const char* title, Uint32 width = 640, Uint32 height = 480, Uint32 bpp = 0, Uint32 flags = 0);

    /**
     * Deallocate all the accumulated resources and clean up all the loaded states.
     */
    void cleanUp();

    /**
     * Changes the currently active game state. The currently loaded state will be deallocated from the memory.
     * @param gameState* gameState
     */
    void changeState(JGameState* gameState);

    /**
     * Pushes a new game state on top of the currently loaded one.
     * @param gameState* gameState
     */
    void pushState(JGameState* gameState);

    /**
     * Closes and deallocates the currently loaded game state.
     */
    void popState();

    /**
     * Hook function for the main loop. Will call the according function in the
     * loaded states.
     */
    void handleEvents();

    /**
     * Hook function for the main loop. Will call the according function in the
     * loaded states.
     */
    void calculate();

    /**
     * Hook function for the main loop. Will call the according function in the
     * loaded states.
     */
    void update();

    /**
     * Hook function for the main loop. Will call the according function in the
     * loaded states.
     */
    void show();

    /**
     * Toggle showing in full screen modus.
     */
    void toggleFullscreen();

    /**
     * Set the executable path of the application for file reference.
     * @param std::string path
     */
    void setBasePath(std::string path);

    /**
     * Retrieve the base path of the executable
     * @return std::string
     */
    std::string getBasePath() const;

    /**
     * Get the count of loaded game states.
     * 
     * @return Uint32
     */
    Uint32 getStateCount() const {
        return gameStates.size();
    }

    /**
     * Check if the engine is running.
     * When no game state has been loaded the engine can't be running.
     * @return bool 
     */
    bool checkRunning() {
        return ((i_settings & ISRUNNING) == ISRUNNING);
    }

    /**
     * Test or we are running full screen.
     * @return bool
     */
    bool isFullscreen() {
        return ((i_settings & ISFULLSCREEN) == ISFULLSCREEN);
    }

    /**
     * Get the width of the main screen.
     * @return Uint32
     */
    Uint32 getScreenWidth()
    {
    	return this->screen->clip_rect.w;
    }

    /**
     * Get the height of the main screen.
     * @return Uint32
     */
    Uint32 getScreenHeight()
    {
    	return this->screen->clip_rect.h;
    }

    /**
     * Set the frames per second of the application.
     * @param Uint16 fps
     */
    void setFps(Uint16 fps)
    {
    	this->fps = fps;
    }

    /**
     * Get the frames per second of this application.
     * @return Uint16
     */
    Uint16 getFps()
    {
    	return this->fps;
    }

    bool stopEventHandling(bool stop)
    {
    	this->i_settings &= ~STOPEVENTHANDLING;
    	if (!stop) {
    		this->i_settings |= STOPEVENTHANDLING;
    	}
    	return ((this->i_settings & STOPEVENTHANDLING) == STOPEVENTHANDLING);
    }

    bool stopEventHandling()
    {
        return ((this->i_settings & STOPEVENTHANDLING) == STOPEVENTHANDLING);
    }

    /**
     * Stop the engine.
     */
    void exit() {
    	PRINT("The engine received the quit signal.")
        i_settings &= ~ISRUNNING;
    }

};

#endif
