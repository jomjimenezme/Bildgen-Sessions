// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <array>

#include <Eigen/Core>

#include "ray.h"
#include "object.h"
#include "parallelogram.h"

namespace rtai {

  class Parallelepiped : public Object
  {
  public:
    Parallelepiped() {};
    Parallelepiped( const Vector3d& p, const Vector3d& d1,
                    const Vector3d& d2, const Vector3d& d3 )
    { setPos( p, d1, d2, d3 ); }

    HitInfo isHitBy( const Ray &ray ) override;

    void setP( const Vector3d &p ) { setPos( p, dir1, dir2, dir3 ); }
    void setD1( const Vector3d &d ) { setPos( position, d, dir2, dir3 ); }
    void setD2( const Vector3d &d ) { setPos( position, dir1, d, dir3 ); }
    void setD3( const Vector3d &d ) { setPos( position, dir1, dir2, d); }

  private:
    void setPos( const Vector3d& p, const Vector3d& d1,
                 const Vector3d& d2, const Vector3d& d3 );

    Vector3d dir1, dir2, dir3;

    std::array<Parallelogram,6> sides;
  };

}

