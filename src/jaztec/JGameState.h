#ifndef _JAS_GAME_STATE_H_
#define _JAS_GAME_STATE_H_

#include "JEngine.h"

class JGameState {
protected:

	/**
	 * Private constructor for singleton implementation.
	 */
    JGameState() {
    }

public:

    /**
     * Destructor.
     */
    virtual ~JGameState() {
    }

    /**
     * Initiate this game state. Creating objects will be done here.
     * @param JEngine* game
     */
    virtual void init(JEngine* game) = 0;

    /**
     * Clean up the game state. Deallocating objects will be done here.
     */
    virtual void cleanUp() = 0;

    /**
     * Pause this game state. Stop timers for example.
     */
    virtual void pause() = 0;

    /**
     * Resume the game state.
     */
    virtual void resume() = 0;

    /**
     * Hook function to process user input events.
     * @param JEngine* game
     * @param SDL_Event event
     */
    virtual void handleEvent(JEngine* game, SDL_Event* event) = 0;

    /**
     * Calculate the new states for any used objects in the memory pool.
     * @param JEngine* game
     */
    virtual void calculate(JEngine* game) = 0;

    /**
     * Update any objects that need to react to the new calculated states of previously set objects.
     * @param JEngine* game
     */
    virtual void update(JEngine* game) = 0;

    /**
     * Render the needed objects to the screen.
     * @param JEngine* game
     */
    virtual void show(JEngine* game) = 0;

    /**
     * Change the game state through the game engine.
     * @param JEngine* game
     * @param JGameState* gameState
     */
    void changeState(JEngine* game, JGameState* gameState) {
        game->changeState(gameState);
    }

};

#endif
