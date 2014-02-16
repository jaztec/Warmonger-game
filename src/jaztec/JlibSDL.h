#ifndef _JAS_LIB_SDL_H_
#define _JAS_LIB_SDL_H_

/**
 * @author Jasper van Herpt
 * @date 2014-02-16
 */

/******************************************************************
 Global settings.
 ******************************************************************/
#ifdef __WIN32
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"
#endif

#include <cmath>
#include <string>

#include "ErrHandling.hpp"

/**
 * Name space start
 */
namespace jasdlib {

/**
 * Basic functions defines.
 */
#define		JLIB_COLLIDE_NONE						0x00000000
#define 	JLIB_COLLIDE_TOP 						0x00000001
#define 	JLIB_COLLIDE_LEFT 						0x00000002
#define 	JLIB_COLLIDE_RIGHT 						0x00000004
#define 	JLIB_COLLIDE_BOTTOM                     0x00000008
#define		JLIB_COLLIDE_TOPLEFT                    0x00000010
#define		JLIB_COLLIDE_TOPRIGHT                   0x00000020
#define		JLIB_COLLIDE_BOTTOMLEFT                 0x00000040
#define		JLIB_COLLIDE_BOTTOMRIGHT                0x00000080
/**
 * Class Timer defines.
 */
#define		TIMER_TICKS_NONE						0x00000000
#define		TIMER_TICKS_STARTED						0x00000001
#define		TIMER_TICKS_PAUSED						0x00000002
#define		TIMER_RUNNING							0x00000004
#define		TIMER_PAUSED							0x00000008
/**
 * Class Button defines.
 */
#define		BUTTON_ENABLED							0x00000001
#define		BUTTON_CLICKED							0x00000002
#define		BUTTON_DEFAULT_WIDTH                    0x0000012C
#define		BUTTON_DEFAULT_HEIGHT                   0x0000003C
/**
 * 2DObject classes defines,
 */
#define		JGOBJ_IS_ACTIVE							0x00000001
#define		JGOBJ_IS_ALIVE							0x00000002
#define		JGOBJ_IS_DEAD							0x00000004
#define		JGOBJ_IS_FACING_LEFT                    0x00000008
#define		JGOBJ_IS_FACING_RIGHT                   0x00000010
#define		JGOBJ_IS_FACING_UP						0x00000020
#define		JGOBJ_IS_FACING_DOWN                    0x00000040

#define		JMGOBJ_IS_MOVING						0x00000100
#define		JMGOBJ_MOVING_UP						0x00000200
#define		JMGOBJ_MOVING_DOWN						0x00000400
#define		JMGOBJ_MOVING_LEFT						0x00000800
#define		JMGOBJ_MOVING_RIGHT						0x00001000

#define		JFGOBJ_IS_FALLING						0x00010000
#define		JFGOBJ_IS_RISING						0x00020000
#define		JFGOBJ_IS_SUPPORTED						0x00040000

/**
 * Internal class Timer, uses SDL millisecond calculations for precision frame counting
 */
class Timer {
public:
	/**
	 * Constructor
	 */
	Timer();

	/**
	 * Copy constructor
	 * @param Timer& rhs
	 */
	Timer(const Timer& rhs);

	/**
	 * Destructor
	 */
	virtual ~Timer() {
	}

	/**
	 * Start the timer.
	 */
	virtual void start();

	/**
	 * Stop the timer
	 */
	virtual void stop();

	/**
	 * Pause the timer. This will save the current ticks.
	 */
	virtual void pause();

	/**
	 * Resume the timer from the last known saved ticks.
	 */
	virtual void resume();

	/**
	 * Set the ticks to zero. Will not stop timer.
	 */
	virtual void reset();

	/**
	 *
	 * @param Uint32 type Either 0|TIMER_TICKS_STARTED|TIMER_TICKS_PAUSED
	 * @return Uint32 The currently registered ticks.
	 */
	virtual Uint32 getTicks(Uint32 type = TIMER_TICKS_NONE) const;

	/**
	 * Test if the timer is started.
	 * @return bool
	 */
	virtual bool isStarted() const;

	/**
	 * Test if the timer is paused.
	 * @return bool
	 */
	virtual bool isPaused() const;

protected:
	/**
	 * @var Uint32 startTicks
	 */
	Uint32 startTicks;

	/**
	 * @var Uint32 pausedTicks
	 */
	Uint32 pausedTicks;

	/**
	 * @var Uint32 i_settings
	 */
	Uint32 i_settings;
};

/**
 * Basis of all objects in the game, all to be used object must derive from these base classes.
 */
class J2DGameObject {
public:
	/**
	 * Constructor.
	 */
	J2DGameObject();

	/**
	 * Constructor.
	 * @param Uint16 x
	 * @param Uint16 y
	 */
	J2DGameObject(Sint16 x, Sint16 y);

	/**
	 * Constructor
	 * @param Uint16 x
	 * @param Uint16 y
	 * @param Uint16 w
	 * @param Uint16 h
	 */
	J2DGameObject(Sint16 x, Sint16 y, Uint16 w, Uint16 h);

	/**
	 * Destructor
	 */
	virtual ~J2DGameObject();

	/**
	 * Abstract function, draws the object to the screen.
	 */
	virtual void draw(SDL_Surface* screen) = 0;

	/**
	 * Returns the inner SDL_Rect which is the main part of
	 * any 2D object,
	 * @return SDL_Rect*
	 */
	virtual SDL_Rect* getInnerRect();

	/**
	 * Return the complete settings for this object,
	 * @return
	 */
	Uint32 getSettings() const;
	/**
	 * Functions for getting info about the status of the object.
	 */
	bool isActive() const;
	bool isAlive() const;
	bool isFacingUp() const;
	bool isFacingDown() const;
	bool isFacingLeft() const;
	bool isFacingRight() const;
	/**
	 * Functions for setting the status of the object.
	 */
	void setActive();
	void setUnactive();
	void setDead();
	void setInner(SDL_Rect* other);
	void setFacing(Uint16 facing);
	/**
	 * Functions for setting the different internal parameters.
	 */
	void setPos(Sint16 x, Sint16 y);
	void setSize(Uint16 w, Uint16 h);

protected:
	Sint16 x_pos, y_pos;
	Sint16 width, height;
	SDL_Rect* inner;

	Uint32 i_settings;

	void updateInner();
};

/**
 * J2DMovGameObject
 */
class J2DMovGameObject: public J2DGameObject {
public:
	J2DMovGameObject();
	J2DMovGameObject(Sint16 x, Sint16 y);
	J2DMovGameObject(Sint16 x, Sint16 y, Uint16 w, Uint16 h);
	J2DMovGameObject(Sint16 x, Sint16 y, Uint16 w, Uint16 h, float xs, float ys);
	virtual ~J2DMovGameObject();

	virtual void update();

	float getXspeed() const;
	float getYspeed() const;

	bool isMoving() const;
	void startMoving();
	void stopMoving();

protected:
	float x_speed, y_speed;
};

/**
 * J2DPhysicalGameObject
 */
class J2DPhysicalGameObject: public J2DMovGameObject {
public:
	J2DPhysicalGameObject();
	J2DPhysicalGameObject(Sint16 x, Sint16 y);
	J2DPhysicalGameObject(Sint16 x, Sint16 y, Uint16 w, Uint16 h);
	J2DPhysicalGameObject(Sint16 x, Sint16 y, Uint16 w, Uint16 h, float we);
	virtual ~J2DPhysicalGameObject();

	float getWeight() const;
	void setWeight(float w);

	/**
	 * Overloading the update function for physics.
	 */
	virtual void update();
	/**
	 * Check if this object is supported by another object.
	 */
	bool checkSupportedByObject(J2DGameObject* target);
	/**
	 *
	 */
	void startFalling();
	void stopFalling();
	void startJump();
	void stopJump();

protected:
	float weight;
};

class Button: public J2DGameObject {
public:
	Button(std::string text, TTF_Font* font, Sint32 x_pos = 0, Sint32 y_pos = 0,
			Uint32 flags = 0);
	~Button();

	bool handleEvents(SDL_Event* event);
	void draw(SDL_Surface* surface);

	bool enabled() const;
	void enabled(bool flag);

	std::string getCaption() const;
	void setCaption(std::string cap, TTF_Font* font);

	int getX() const;
	int getY() const;

protected:
	SDL_Surface* visual;
	Mix_Chunk* click_sound;
	std::string caption;
};

/**
 * General functions
 */

SDL_Surface* loadImageFromFile(const char* fileName);

double distance(int x1, int y1, int x2, int y2);

bool isCollision(SDL_Rect& rect1, SDL_Rect& rect2);
bool isCollision(SDL_Rect& rect1, SDL_Rect& rect2, int side);

bool inRange(int x_c, int y_c, SDL_Rect& rect);

} // End name space jasdlib

#endif
