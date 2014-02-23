
#include "WMCamera.hpp"

WMCamera::WMCamera() {

}

WMCamera::~WMCamera() {

}

Sint32 WMCamera::getXSpeed() const {
    return this->xSpeed;
}

Sint32 WMCamera::getYSpeed() const {
    return this->ySpeed;
}

void WMCamera::setXSpeed(const Sint32 speed) {
    this->xSpeed = speed;
}

void WMCamera::setYSpeed(const Sint32 speed) {
    this->ySpeed = speed;
}

void WMCamera::update() {
    Sint32 x = this->itsView.x + this->getXSpeed();
    Sint32 y = this->itsView.y + this->getYSpeed();
    this->setPosition(x, y);
}
