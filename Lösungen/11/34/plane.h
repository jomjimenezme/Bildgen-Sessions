// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

#include "ray.h"
#include "object.h"

namespace rtai {


  class Plane : public Object
  {
  public:
    Plane() = delete;
    Plane( const Vector3d& norm, double dist ) { setPos( norm, dist ); }

    HitInfo isHitBy( const Ray &ray ) override;

    void setNormal( const Vector3d &norm ) { setPos( norm, distance ); }
    void setDistance( double dist ) { setPos( normal, dist ); };
    inline Vector3d getNormal() const { return normal; }
    inline double getDistance() const { return distance; }

  private:

    void setPos( const Vector3d& norm, double dist );
    Vector3d normal;
    double distance;
  };

}
