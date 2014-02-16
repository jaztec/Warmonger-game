#include "src/jaztec/ErrHandling.hpp"
#include "src/jaztec/JEngine.h"
#include "src/jaztec/JlibSDL.h"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {

    // Setup the window and game engine.
    JEngine game;
    game.init("Warmonger", 800, 640, 32, 0);

    try {
//        game.change_state(HMainState::instance());

        // Main loop.
        while (game.checkRunning()) {
            game.handleEvents();
            game.calculate();
            game.update();
            game.show();
        }
    } catch( ... ) {
        // An exception has occured. Cleanup and return error.
        game.cleanUp();
        return -1;
    }

    // Cleanup and return nothing to report.
    game.cleanUp();
    return 0;
}
