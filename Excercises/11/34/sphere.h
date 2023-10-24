// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

#include "ray.h"
#include "object.h"

namespace rtai {


  class Sphere : public Object
  {
  public:
    Sphere() = delete;
    Sphere( const Vector3d& cent, double rad ) { setPos( cent, rad ); }

    HitInfo isHitBy( const Ray &ray ) override;

    void setCenter( const Vector3d &cent ) { setPos( cent, radius ); }
    void setRadius( double rad ) { setPos( center, rad ); };
    inline Vector3d getCenter() const { return center; }
    inline double getRadius() const { return radius; }

  private:

    void setPos( const Vector3d& cent, double rad );
    Vector3d center;
    double radius;
  };

}
