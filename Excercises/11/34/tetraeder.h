// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <array>

#include <Eigen/Core>

#include "ray.h"
#include "object.h"
#include "triangle.h"

namespace rtai {

  class Tetraeder : public Object
  {
  public:
    Tetraeder() {};
    Tetraeder( const Vector3d& cen, double dim, const Vector3d& rot )
    { setPos( cen, dim, rot ); }

    HitInfo isHitBy( const Ray &ray ) override;

    void setCenter( const Vector3d &cen )
    { setPos( cen, dimension, rotation ); }
    void setDimension( double d ) { setPos( center, d, rotation ); }
    void setRotation( const Vector3d &rot )
    { setPos( center, dimension, rot ); }

  private:
    void setPos( const Vector3d& cen, double dim, const Vector3d& rot );

    Vector3d center, rotation;
    double dimension;

    std::array<Triangle,4> sides;
  };

}
