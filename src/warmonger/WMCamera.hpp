/* 
 * File:   Camera.hpp
 * Author: jasper
 *
 * Created on February 23, 2014, 5:20 PM
 */

#ifndef _WARMONGER_CAMERA_HPP
#define	_WARMONGER_CAMERA_HPP

#include "../jaztec/TileManager.h"
#include <stdlib.h>

class WMCamera : public Camera
{
protected:
    Sint32 xSpeed, ySpeed;
public:
    WMCamera();
    virtual ~WMCamera();
    
    Sint32 getXSpeed() const;
    Sint32 getYSpeed() const;
    void setXSpeed(const Sint32 speed);
    void setYSpeed(const Sint32 speed);
    
    void update();
};

#endif	/* _WARMONGER_CAMERA_HPP */

