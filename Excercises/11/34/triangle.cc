// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>

#include "triangle.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  HitInfo Triangle::isHitBy( const Ray &ray )
  {
    HitInfo hi;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Mit
    //   Strahl: x = o + l*d
    // und
    //   Dreieck: x = p + l1*d1 + l2*d2
    //            l1 >= 0, l2 >= 0, l1+l2 <= 1
    // gilt:
    //   (d | d1 | d2)*(l -l1 -l2)^T = p - o
    // getParams liefert den Vektor (l l1 l2) zurück mit l == 0.0, wenn
    // o in der von d1 und d2 aufgespannten Ebene liegt.
    Vector3d params = getParams( ray );

    // Test, ob Strahl parallel zur Ebene ist
    if (params(0) == 0.0) {
      // Strahl liegt in Dreieck, wenn Ausgangspunkt in Dreieck liegt
      if (params(1)>=0 && params(2) >= 0 && (params(1)+params(2)<=1)) {
        hi.hit = true;
        hi.rayparam = 0.0;
        hi.position = ray.origin;
        hi.normal = normal;
        hi.object = this;
      }
      else {
        hi.hit = false;
      }
    }
    else {
      // Strahl trifft Ebene, in der das Dreieck liegt
      hi.rayparam = params(0);
      if (hi.rayparam < 0 && ray.insideObject != this)
        hi.hit=false; // Strahl trifft hinter origin
      else {
        if (params(1)>=0 && params(2) >= 0 && (params(1)+params(2)<=1)) {
          hi.hit = true;
          hi.position = ray.origin + hi.rayparam*ray.direction;
          // Normale muss in Richtung des Ausgangspunktes des
          // eintreffenden Strahls zeigen
          hi.normal = ((ray.direction.dot(normal) < 0)?1:-1) * normal;
          hi.object = this;
        }
      }
    }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return hi;
  }

}
