// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "box.h"

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

using namespace std;
using namespace Eigen;

namespace rtai {

  HitInfo Box::isHitBy( const Ray &ray )
  {
    HitInfo hi = parepiped.isHitBy( ray );
    hi.object = this;

    return hi;
  }

  void Box::setPos( const Vector3d& cen, const Vector3d& dims,
                    const Vector3d& rot )
  {
    //  Rotation um durch rot gegebene Achse mit durch norm(rot) gegebenem
    //  Winkel in Grad
    position = cen;
    center = cen;
    dimensions = dims;
    rotation = rot;

    // Box-Koordinaten ermitteln
    Vector3d d1(dims(0)/2,0,0);
    Vector3d d2(0,dims(1)/2,0);
    Vector3d d3(0,0,dims(2)/2);

    // Rotation anwenden
    if ( rotation.norm() > 0 ) {
      auto rad = [](double d) { return 2*M_PI*d/360.0; };
      AngleAxisd aa = AngleAxisd(rad(rotation.norm()),rotation/rotation.norm());
      Matrix3d rotmat = aa.matrix();
      d1 = rotmat*d1;
      d2 = rotmat*d2;
      d3 = rotmat*d3;
    }

    // Seitenflächen mittels Parallelepiped erstellen
    parepiped = Parallelepiped( cen-d1-d2-d3, 2*d1, 2*d2, 2*d3 );
  }

}
