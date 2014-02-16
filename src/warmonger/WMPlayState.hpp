/*
 * WMPlayState.hpp
 *
 *  Created on: Feb 16, 2014
 *      Author: jasper
 */

#ifndef WMPLAYSTATE_HPP_
#define WMPLAYSTATE_HPP_


#ifdef __WIN32
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#endif

#include "../jaztec/ErrHandling.hpp"
#include "../jaztec/JEngine.h"
#include "../jaztec/JGameState.h"
#include "../jaztec/JlibSDL.h"
#include "../jaztec/TileManager.h"

#include <sstream>

using namespace jasdlib;

class WMPlayState: public JGameState {
protected:
	TTF_Font* font;
	SDL_Surface* background;
	Uint32 frame;
	Uint32 screenWidth;
	Uint32 screenHeight;
	Timer fps;
	static WMPlayState WarmongerPlayState;
	TileManager* map;

	WMPlayState();
public:
	virtual ~WMPlayState(){}

	virtual void init(JEngine* engine);
	virtual void cleanUp();
	virtual void pause();
	virtual void resume();
	virtual void handleEvent(JEngine* engine, SDL_Event* event);
	virtual void calculate(JEngine* engine);
	virtual void update(JEngine* engine);
	virtual void show(JEngine* game);

	static WMPlayState* instance() {
		return &WarmongerPlayState;
	}

};

#endif /* WMPLAYSTATE_HPP_ */
