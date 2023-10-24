// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>

#include "plane.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  HitInfo Plane::isHitBy( const Ray &ray )
  {
    HitInfo hi;
    // Test, ob Strahl parallel zur Ebene ist
    if (ray.direction.dot(normal) == 0.0) {
      // Strahl liegt in Ebene, wenn Ausgangspunkt in Ebene liegt
      if (abs(ray.origin.dot(distance*normal)) == 0) {
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
      // Strahl trifft Ebene
      hi.rayparam = (distance - ray.origin.dot(normal))/ray.direction.dot(normal);
      if (hi.rayparam < 0 && ray.insideObject != this)
        hi.hit=false; // Strahl trifft hinter origin
      else {
        hi.hit = true;
        hi.position = ray.origin + hi.rayparam*ray.direction;
        // Normale muss in Richtung des Ausgangspunktes des
        // eintreffenden Strahls zeigen
        hi.normal = ((ray.direction.dot(normal) < 0)?1:-1) * normal;
        hi.object = this;
      }
    }

    return hi;
  }

  void Plane::setPos( const Vector3d& norm, double dist )
  {
    normal = norm/norm.norm();
    distance = dist;

    // damit da was sinnvolles steht
    position = normal*distance;
  }

}
