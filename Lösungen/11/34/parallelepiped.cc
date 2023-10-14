// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>

#include "parallelepiped.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  HitInfo Parallelepiped::isHitBy( const Ray &ray )
  {
    HitInfo hi;
    hi.hit = false;
    if (ray.insideObject != nullptr)
      hi.rayparam = 0.0;

    // mit allen Seitenflächen testen
    for (int i = 0; i < 6; i++ ) {
      HitInfo hitest = sides.at(i).isHitBy( ray );

      if (ray.insideObject == nullptr) {
        // Strahl trifft von außen auf Objekt
        if ( hitest.hit
             && (hitest.rayparam < hi.rayparam)
             && (hitest.rayparam >= 0) ) {
          hi = hitest;
          hi.object = this;
        }
      }
      else {
        // Strahl verläuft innerhalb des Objektes
        if ( hitest.hit
             && (hitest.rayparam > hi.rayparam)
             && (hitest.rayparam >= 0) ) {
          hi = hitest;
          hi.object = this;
          hi.normal *= -1;
        }
      }
    }

    return hi;
  }

  void Parallelepiped::setPos( const Vector3d& p, const Vector3d& d1,
                               const Vector3d& d2, const Vector3d& d3 )
  {
    position = p;
    dir1 = d1;
    dir2 = d2;
    dir3 = d3;

    sides.at(0) = Parallelogram( p, p+d1, p+d2 );
    sides.at(1) = Parallelogram( p, p+d1, p+d3 );
    sides.at(2) = Parallelogram( p, p+d2, p+d3 );
    sides.at(3) = Parallelogram( p+d1+d2+d3, p+d1+d2, p+d2+d3 );
    sides.at(4) = Parallelogram( p+d1+d2+d3, p+d1+d2, p+d1+d3 );
    sides.at(5) = Parallelogram( p+d1+d2+d3, p+d2+d3, p+d1+d3 );
  }

}



