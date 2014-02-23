#include <limits.h>

#include "JlibSDL.h"
#include "warmonger/WMCamera.hpp"

/******************************************************************************************
 Class function
 ******************************************************************************************/

/**
 * Name space
 */
namespace jasdlib {

/**
 * Timer
 */
Timer::Timer() {
	this->i_settings = 0;
	this->startTicks = 0;
	this->pausedTicks = 0;
}

Timer::Timer(const Timer& rhs) {
	this->startTicks = rhs.getTicks(TIMER_TICKS_STARTED);
	this->pausedTicks = rhs.getTicks(TIMER_TICKS_PAUSED);
	if (rhs.isStarted()) {
		this->i_settings |= TIMER_RUNNING;
	}
	if (rhs.isPaused()) {
		this->i_settings |= TIMER_PAUSED;
	}
}

void Timer::start() {
	this->i_settings |= TIMER_RUNNING;
	this->i_settings &= ~TIMER_PAUSED;
	this->startTicks = SDL_GetTicks();
}

void Timer::stop() {
	this->i_settings = 0;
}

void Timer::pause() {
	if ((this->isStarted()) && (!this->isPaused())) {
		this->i_settings |= TIMER_PAUSED;
		this->pausedTicks = SDL_GetTicks() - this->startTicks;
	}
}

void Timer::resume() {
	if (this->isPaused()) {
		this->i_settings &= ~TIMER_PAUSED;
		this->startTicks = SDL_GetTicks() - this->pausedTicks;
		this->pausedTicks = 0;
	}
}

void Timer::reset() {
	this->stop();
	this->start();
}

Uint32 Timer::getTicks(Uint32 type) const {
	if (type == 0) {
		if (this->isStarted()) {
			if (this->isPaused()) {
				return this->pausedTicks;
			} else {
				return SDL_GetTicks() - this->startTicks;
			}
		}
	}
	switch (type) {
		case ( TIMER_TICKS_STARTED):
			return this->startTicks;
			break;
		case ( TIMER_TICKS_PAUSED):
			return this->pausedTicks;
			break;
		default:
			return 0;
			break;
	}

	return 0;
}

bool Timer::isStarted() const {
	return ((this->i_settings & TIMER_RUNNING) == TIMER_RUNNING);
}

bool Timer::isPaused() const {
	return ((this->i_settings & TIMER_PAUSED) == TIMER_PAUSED);
}

/**
 * Object classes
 */

/**
 * J2DGameObject
 */
J2DGameObject::J2DGameObject() {
	this->inner = new SDL_Rect();
	this->x_pos = 0;
	this->y_pos = 0;
	this->width = 0;
	this->height = 0;
	this->i_settings = 0;
	this->updateInner();
}

J2DGameObject::J2DGameObject(Sint32 x, Sint32 y) {
	this->inner = new SDL_Rect();
	this->x_pos = x;
	this->y_pos = y;
	this->width = 0;
	this->height = 0;
	this->i_settings = 0;
	this->updateInner();
}

J2DGameObject::J2DGameObject(Sint32 x, Sint32 y, Uint32 w, Uint32 h) {
	this->inner = new SDL_Rect();
	this->x_pos = x;
	this->y_pos = y;
	this->width = w;
	this->height = h;
	this->i_settings = 0;
	this->updateInner();
}

J2DGameObject::~J2DGameObject() {
	if (this->inner != 0) {
		delete this->inner;
	}
}

SDL_Rect* J2DGameObject::getInnerRect() {
	if (this->inner != 0) {
		return this->inner;
	}
	return 0;
}

unsigned int J2DGameObject::getSettings() const {
	return this->i_settings;
}

bool J2DGameObject::isActive() const {
	return ((this->i_settings & JGOBJ_IS_ACTIVE) == JGOBJ_IS_ACTIVE);
}

bool J2DGameObject::isAlive() const {
	return ((this->i_settings & JGOBJ_IS_ALIVE) == JGOBJ_IS_ALIVE);
}

bool J2DGameObject::isFacingUp() const {
	return ((this->i_settings & JGOBJ_IS_FACING_UP) == JGOBJ_IS_FACING_UP);
}

bool J2DGameObject::isFacingDown() const {
	return ((this->i_settings & JGOBJ_IS_FACING_DOWN) == JGOBJ_IS_FACING_DOWN);
}

bool J2DGameObject::isFacingLeft() const {
	return ((this->i_settings & JGOBJ_IS_FACING_LEFT) == JGOBJ_IS_FACING_LEFT);
}

bool J2DGameObject::isFacingRight() const {
	return ((this->i_settings & JGOBJ_IS_FACING_RIGHT) == JGOBJ_IS_FACING_RIGHT);
}

void J2DGameObject::setActive() {
	this->i_settings |= JGOBJ_IS_ACTIVE;
}

void J2DGameObject::setUnactive() {
	this->i_settings &= ~JGOBJ_IS_ACTIVE;
}

void J2DGameObject::setDead() {
	this->i_settings &= ~JGOBJ_IS_ALIVE;
	this->i_settings |= JGOBJ_IS_DEAD;
}

void J2DGameObject::setInner(SDL_Rect* other) {
	if (this->inner) {
		delete this->inner;
	}
	this->inner = new SDL_Rect;
	*this->inner = *other;
}

void J2DGameObject::setFacing(Uint16 facing) {
	this->i_settings &= ~(JGOBJ_IS_FACING_LEFT | JGOBJ_IS_FACING_RIGHT
			| JGOBJ_IS_FACING_UP | JGOBJ_IS_FACING_DOWN);

	if (facing == JGOBJ_IS_FACING_LEFT) {
		this->i_settings |= JGOBJ_IS_FACING_LEFT;
	} else if (facing == JGOBJ_IS_FACING_RIGHT) {
		this->i_settings |= JGOBJ_IS_FACING_RIGHT;
	} else if (facing == JGOBJ_IS_FACING_UP) {
		this->i_settings |= JGOBJ_IS_FACING_UP;
	} else if (facing == JGOBJ_IS_FACING_DOWN) {
		this->i_settings |= JGOBJ_IS_FACING_DOWN;
	} else {
		this->i_settings |= JGOBJ_IS_FACING_DOWN;
	}
}

void J2DGameObject::setPos(Sint32 x, Sint32 y) {
	this->x_pos = x;
	this->y_pos = y;
	this->updateInner();
}

void J2DGameObject::setSize(Uint32 w, Uint32 h) {
	this->width = w;
	this->height = h;
	this->updateInner();
}

void J2DGameObject::updateInner() {
	this->inner->x = this->x_pos;
	this->inner->y = this->y_pos;
	this->inner->w = this->width;
	this->inner->h = this->height;
}

Sint32 J2DGameObject::getXPosition() const
{
    return this->getInnerRect()->x;
}

Sint32 J2DGameObject::getYPosition() const
{
    return this->getInnerRect()->y;
}

/**
 * J2DMovGameObject
 */
J2DMovGameObject::J2DMovGameObject() {
	this->x_speed = 0;
	this->y_speed = 0;
}

J2DMovGameObject::J2DMovGameObject(Sint32 x, Sint32 y) :
		J2DGameObject(x, y) {
	this->x_speed = 0;
	this->y_speed = 0;
}

J2DMovGameObject::J2DMovGameObject(Sint32 x, Sint32 y, Uint32 w, Uint32 h) :
		J2DGameObject(x, y, w, h) {
	this->x_speed = 0;
	this->y_speed = 0;
}

J2DMovGameObject::J2DMovGameObject(Sint32 x, Sint32 y, Uint32 w, Uint32 h, float xs,
		float ys) :
		J2DGameObject(x, y, w, h) {
	this->x_speed = xs;
	this->y_speed = ys;
}

J2DMovGameObject::~J2DMovGameObject() {
}

void J2DMovGameObject::update() {
	this->x_pos += this->x_speed;
	this->y_pos += this->y_speed;
	this->updateInner();
}

float J2DMovGameObject::getXspeed() const {
    return this->x_speed;
}

void J2DMovGameObject::setXSpeed(const float speed) {
    this->x_speed = speed;
}


float J2DMovGameObject::getYspeed() const {
	return this->y_speed;
}

void J2DMovGameObject::setYSpeed(const float speed) {
    this->y_speed = speed;
}


bool J2DMovGameObject::isMoving() const {
	return ((this->i_settings & JMGOBJ_IS_MOVING) == JMGOBJ_IS_MOVING);
}

void J2DMovGameObject::startMoving() {
	this->i_settings |= JMGOBJ_IS_MOVING;
}

void J2DMovGameObject::stopMoving() {
	this->i_settings &= ~JMGOBJ_IS_MOVING;
}

/**
 * J2DPhysicalGameObject
 */
J2DPhysicalGameObject::J2DPhysicalGameObject() {
	this->weight = 1.0f;
}

J2DPhysicalGameObject::J2DPhysicalGameObject(Sint32 x, Sint32 y) :
		J2DMovGameObject(x, y) {
	this->weight = 1.0f;
}

J2DPhysicalGameObject::J2DPhysicalGameObject(Sint32 x, Sint32 y, Uint32 w, Uint32 h) :
		J2DMovGameObject(x, y, w, h) {
	this->weight = 1.0f;
}

J2DPhysicalGameObject::J2DPhysicalGameObject(Sint32 x, Sint32 y, Uint32 w, Uint32 h,
		float we) :
		J2DMovGameObject(x, y, w, h) {
	this->weight = we;
}

J2DPhysicalGameObject::~J2DPhysicalGameObject() {
}

float J2DPhysicalGameObject::getWeight() const {
	return this->weight;
}

void J2DPhysicalGameObject::setWeight(float w) {
	this->weight = w;
}

void J2DPhysicalGameObject::update() {
	if (JFGOBJ_IS_FALLING) {
		this->y_speed += this->weight / 20;
	}

	J2DMovGameObject::update();
}

bool J2DPhysicalGameObject::checkSupportedByObject(J2DGameObject* target) {
	Uint32 dir = isCollision(*target->getInnerRect(), *this->getInnerRect());
	if (dir == JLIB_COLLIDE_BOTTOM || dir == JLIB_COLLIDE_BOTTOMLEFT
			|| dir == JLIB_COLLIDE_BOTTOMRIGHT || dir == JLIB_COLLIDE_RIGHT
			|| dir == JLIB_COLLIDE_LEFT)
	{
		return true;
	}
	return false;
}

void J2DPhysicalGameObject::startFalling() {
	this->i_settings &= ~JFGOBJ_IS_SUPPORTED;
	this->i_settings |= JFGOBJ_IS_FALLING;
}

void J2DPhysicalGameObject::stopFalling() {
	this->i_settings &= ~JFGOBJ_IS_FALLING;
	this->i_settings |= JFGOBJ_IS_SUPPORTED;
	this->y_speed = 0;
}

void J2DPhysicalGameObject::startJump() {
}

void J2DPhysicalGameObject::stopJump() {
}

/**
 * Button
 */
Button::Button(std::string text, TTF_Font* font, Sint32 x_pos, Sint32 y_pos,
		Uint32 flags) {
	this->visual = SDL_CreateRGBSurface(SDL_HWSURFACE, BUTTON_DEFAULT_WIDTH,
			BUTTON_DEFAULT_HEIGHT, 32, 0, 0, 0, 0);
	this->click_sound = NULL;

	this->setInner(&visual->clip_rect);
	this->setPos(x_pos, y_pos);
	this->setSize(BUTTON_DEFAULT_WIDTH, BUTTON_DEFAULT_HEIGHT);

	this->setActive(); // Set the inner object active
	if (flags) {
		this->i_settings |= flags; // Set the button with the given flags
	}
	this->setCaption(text, font);
}

Button::~Button() {
	if (this->visual) {
		SDL_FreeSurface(this->visual);
	}
	if (this->click_sound) {
		Mix_FreeChunk(this->click_sound);
	}
}

bool Button::handleEvents(SDL_Event* event) {
	this->i_settings &= ~BUTTON_CLICKED;
	if (this->i_settings & BUTTON_ENABLED) {
		if (event->type == SDL_MOUSEBUTTONUP) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				if (inRange(event->button.x, event->button.y, *this->getInnerRect())) {
					this->i_settings |= BUTTON_CLICKED;
					if (this->click_sound) {
						Mix_PlayChannel(-1, this->click_sound, 0);
					}
				}
			}
		}
	}
	return ((this->i_settings & BUTTON_CLICKED) == BUTTON_CLICKED);
}

void Button::draw(SDL_Surface* surface) {
	if ((this->i_settings & BUTTON_ENABLED) == BUTTON_ENABLED) {
		SDL_BlitSurface(this->visual, NULL, surface, this->inner);
	}
}

void Button::setCaption(std::string cap, TTF_Font* font) {
	SDL_Color whiteText = { 0xFF, 0xFF, 0xFF };

	this->caption = cap;

	SDL_FillRect(this->visual, &this->visual->clip_rect,
			SDL_MapRGB(this->visual->format, 0xFF, 0, 0));

	SDL_Surface* tempSurface = TTF_RenderText_Solid(font, this->caption.c_str(),
			whiteText);
	tempSurface->clip_rect.x = (this->visual->clip_rect.w - tempSurface->clip_rect.w)
			/ 2;
	tempSurface->clip_rect.y = (this->visual->clip_rect.h - tempSurface->clip_rect.h)
			/ 2;

	SDL_BlitSurface(tempSurface, NULL, this->visual, &tempSurface->clip_rect);

	SDL_FreeSurface(tempSurface);
}

bool Button::enabled() const {
	return ((this->i_settings & BUTTON_ENABLED) == BUTTON_ENABLED);
}

void Button::enabled(bool flag) {
	if (flag)
		this->i_settings |= BUTTON_ENABLED;
	else
		this->i_settings &= ~BUTTON_ENABLED;
}

std::string Button::getCaption() const {
	return this->caption;
}

int Button::getX() const {
	return this->inner->x;
}

int Button::getY() const {
	return this->inner->y;
}

/******************************************************************************
 Algemene functies
 ******************************************************************************/

std::string getAbsolutePath(const std::string& relativePath)
{
    char resolvedPath[PATH_MAX + 1];
    const char* relativePathPtr = relativePath.c_str();
    char* ptr;
    ptr = realpath(relativePathPtr, resolvedPath);
    return relativePath;
}

SDL_Surface* loadImageFromFile(const char* fileName) {
	// Create 2 SDL Surfaces. 1 to read from the file and the other to build a optimized images.
	SDL_Surface* loadedIMG = NULL;
	SDL_Surface* optimizedIMG = NULL;

	loadedIMG = IMG_Load(fileName);

	// If the file loaded successfully
	if (loadedIMG != NULL) {
		// Build an optimized image and deallocate the previous.
		optimizedIMG = SDL_DisplayFormat(loadedIMG);
		SDL_FreeSurface(loadedIMG);

		// When the optimized image loaded successfully apply the color key.
		if (optimizedIMG != NULL) {
			SDL_SetColorKey(optimizedIMG, SDL_SRCCOLORKEY,
					SDL_MapRGB(optimizedIMG->format, 0, 0xFF, 0xFF));
		}
	}

	return optimizedIMG;
}

double distance(int x1, int y1, int x2, int y2) {
	//Return the distance between two points
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool isCollision(SDL_Rect& rect1, SDL_Rect& rect2) {
	return false;
}

bool isCollision(SDL_Rect& rect1, SDL_Rect& rect2, int side) {
	return false;
}

bool inRange(int x_c, int y_c, SDL_Rect& rect) {
	if ((x_c >= rect.x) && (x_c <= (rect.x + rect.w)) && (y_c >= rect.y)
			&& (y_c <= (rect.y + rect.h))) {
		return true;
	}
	return false;
}

} // End of name space jasdlib
