#include "src/jaztec/ErrHandling.hpp"
#include "src/jaztec/JEngine.h"
#include "src/jaztec/JlibSDL.h"

#include "src/warmonger/WMMainState.hpp"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    std::string appName = "Warmonger";
    std::string basePath;

    // Setting up the base path.
    char szTmp[32];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    char pBuf[1024];
    int bytes = readlink(szTmp, pBuf, sizeof(pBuf) - 1);
    if (bytes <= 0) {
        return 1;
    }
    pBuf[bytes] = '\0';
    basePath += pBuf;
    // Remove the application name from the string.
    unsigned int pos = basePath.find_last_of("/\\");
    basePath = basePath.substr(0, pos);
    
    // Setup the window and game engine.
    JEngine game;
    game.setFps(20);
    game.setBasePath(basePath);
    game.init(appName.c_str(), 1024, 768, 32, 0);

    try {
        game.changeState(WMMainState::instance());

        // Main loop.
        while (game.checkRunning()) {
            game.handleEvents();
            game.calculate();
            game.update();
            game.show();
        }
    } catch( ... ) {
        // An exception has occurred. Cleanup and return error.
        game.cleanUp();
        return -1;
    }

    // Cleanup and return nothing to report.
    game.cleanUp();
    return 0;
}
