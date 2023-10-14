// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <stdexcept>

#include "camera.h"

namespace rtai {

  void Camera::setPosition( const Vector3d &pos )
  {
    position = pos;
  }

  void Camera::setDirection( const Vector3d &dir )
  {
    direction = dir/dir.norm();
    if (direction == Vector3d(0,1,0)
        || direction == Vector3d(0,-1,0) )
      throw std::runtime_error("Kamera sollte nicht direkt nach oben oder unten zeigen!");
  }

  void Camera::setHorAngle( double ang )
  {
    horangle = ang;
  }

  void Camera::lookAt( const Vector3d &pos )
  {
    setDirection( pos - position );
  }


  Vector3d Camera::getPosition() const
  {
    return position;
  }

  Vector3d Camera::getDirection() const
  {
    return direction;
  }

  double Camera::getHorAngle() const
  {
    return horangle;
  }

  Vector3d Camera::getUp() const
  {
    return up;
  }

}
