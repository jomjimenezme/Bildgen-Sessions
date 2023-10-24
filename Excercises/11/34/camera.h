// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

using namespace Eigen;

namespace rtai {

  class Camera
  {
  private:
    Vector3d position {0,0,0};
    Vector3d direction {0,0,-1};
    Vector3d up {0,1,0};
    double   horangle = 90.0;

  public:
    Camera() {};

    void setPosition( const Vector3d &pos );
    void setDirection( const Vector3d &dir );
    void setHorAngle( double ang );
    void lookAt( const Vector3d &pos );

    Vector3d getPosition() const;
    Vector3d getDirection() const;
    double getHorAngle() const;
    Vector3d getUp() const;
  };

}
