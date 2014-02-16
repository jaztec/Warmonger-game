/*
 * WMMainState.hpp
 *
 *  Created on: Feb 16, 2014
 *      Author: jasper
 */

#ifndef WMMAINSTATE_HPP_
#define WMMAINSTATE_HPP_

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
#include "../jaztec/JlibSDL.h";

#include <sstream>

using jasdlib;

class WMMainState: public JGameState {
protected:
	TTF_Font* font;
	SDL_Surface* background;
	SDL_Surface* logo;
	Uint32 frame;
	Uint32 screenWidth;
	Uint32 screeHeight;
	Timer fps;
	static WMMainState WarmongerMainState;

	WMMainState();
public:
	virtual ~WMMainState(){}

	virtual void init(JEngine* engine);
	virtual void cleanUp();
	virtual void pause();
	virtual void resume();
	virtual void handleEvents(JEngine* engine);
	virtual void calculate(JEngine* engine);
	virtual void update(JEngine* engine);
	virtual void show(JEngine* game);

	static WMMainState* instance() {
		return &this->WarmongerMainState;
	}

	Button* playButton;
	Button* quitButton;
	Button* resumeButton;
};



#endif /* WMMAINSTATE_HPP_ */
