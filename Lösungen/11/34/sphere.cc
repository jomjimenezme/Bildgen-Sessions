// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>

#include "sphere.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  HitInfo Sphere::isHitBy( const Ray &ray )
  {
    HitInfo hi;

    // Sphere wird getroffen, wenn
    //   || origin+lambda*direction-center || == radius
    // Treffer kann an 0, 1 oder 2 Punkten erfolgen
    // Das führt zu folgender quadr. Gleichung:
    //   l^2*<d,d> + 2l(<o,d>-<d,c>) + <o,o>+<c,c>-2<o,c>-r*r = 0
    //     <=> mit Voraussetzung d != 0
    //   l^2 + 2(<o,d>-<d,c>)/<d,d>*l + (<o,o>+<c,c>-2<o,c>-r*r)/<d,d> = 0
    // Dabei sind:
    //   l ... lambda
    //   o ... origin des Strahls
    //   d ... direction des Strahls
    //   c ... center der Sphere
    //   r ... radius der Sphere
    // Nach pq-Formel:
    //   p = 2(<o,d>-<d,c>)/<d,d>
    //   q = (<o,o>+<c,c>-2<o,c>-r*r)/<d,d>
    //   l_12 = -p/2 +- sqrt((p/2)^2 - q)

    const Vector3d &o = ray.origin;
    const Vector3d &d = ray.direction;
    const Vector3d &c = center;
    double r = radius;
    double p_2 = (o.dot(d) - d.dot(c)) / d.dot(d);
    double q = (o.dot(o)+c.dot(c)-2*o.dot(c)-r*r)/d.dot(d);

    if (p_2*p_2-q < 0) {
      // kein Treffer
      hi.hit = false;
    }
    else if (p_2*p_2-q == 0) {
      // Strahl trifft Sphere tangential
      if (-p_2 < 0)
        hi.hit = false; // Strahl trifft hinter origin
      else {
        hi.hit = true;
        hi.rayparam = -p_2;
        hi.position = o - p_2*d;
        hi.normal = hi.position - center;
        hi.normal /= hi.normal.norm();
        hi.object = this;
      }
    }
    else {
      // Strahl trifft Sphere in zwei Punkten
      double l1 = -p_2-sqrt(p_2*p_2-q);
      double l2 = -p_2+sqrt(p_2*p_2-q);
      // Es gilt immer l1 < l2, aber der Strahl kann auch in der
      // Sphere verlaufen (z.B. Material Glas). Dann wäre l1 <= 0.
      if (l2 < 0)
        hi.hit = false; // Strahl trifft hinter origin
      else {
        hi.hit = true;
        hi.rayparam = (l1 < 0 || ray.insideObject == this) ? l2 : l1;
        hi.position = o + hi.rayparam*d;
        hi.normal = hi.position-center;
        hi.normal /= hi.normal.norm();
        hi.object = this;
      }
    }

    return hi;
  }

  void Sphere::setPos( const Vector3d& cent, double rad )
  {
    center = cent;
    radius = abs(rad);

    position = center;
  }

}
