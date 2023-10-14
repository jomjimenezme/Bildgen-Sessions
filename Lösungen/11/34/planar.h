// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

#include "ray.h"
#include "object.h"

namespace rtai {

  class Planar : public Object
  {
  public:
    Planar() { setPos( Vector3d(1,0,0), Vector3d(0,1,0), Vector3d(0,0,1) ); }
    Planar( const Vector3d& p1, const Vector3d& p2, const Vector3d& p3 )
    { setPos( p1, p2, p3 ); }

    virtual HitInfo isHitBy( const Ray &ray ) = 0;

    void setP1( const Vector3d &p1 ) { setPos( p1, vert2, vert3 ); }
    void setP2( const Vector3d &p2 ) { setPos( vert1, p2, vert3 ); }
    void setP3( const Vector3d &p3 ) { setPos( vert1, vert2, p3 ); }
    inline Vector3d getNormal() const { return normal; }

  protected:
    void setPos( const Vector3d& p1, const Vector3d& p2, const Vector3d& p3 );
    Vector3d getParams( const Ray &ray );

    Vector3d vert1, vert2, vert3;
    Vector3d d1, d2;
    Vector3d normal;
  };

}
