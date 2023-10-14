// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <array>

#include <Eigen/Core>

#include "ray.h"
#include "object.h"
#include "parallelogram.h"
#include "parallelepiped.h"

namespace rtai {

  class Box : public Object
  {
  public:
    Box() = delete;
    Box( const Vector3d& cen, const Vector3d& dims, const Vector3d& rot )
    { setPos( cen, dims, rot ); }

    HitInfo isHitBy( const Ray &ray ) override;

    void setCenter( const Vector3d &cen )
    { setPos( cen, dimensions, rotation ); }
    void setDimensions( const Vector3d &dims )
    { setPos( center, dims, rotation ); }
    void setRotation( const Vector3d &rot )
    { setPos( center, dimensions, rot ); }

  private:
    void setPos( const Vector3d& cen, const Vector3d& dims,
                 const Vector3d& rot );

    Vector3d center, dimensions, rotation;

    Parallelepiped parepiped;
  };

}
