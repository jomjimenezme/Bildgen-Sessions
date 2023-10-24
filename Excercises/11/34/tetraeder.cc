// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "tetraeder.h"

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

using namespace std;
using namespace Eigen;

namespace rtai {

  HitInfo Tetraeder::isHitBy( const Ray &ray )
  {
    HitInfo hi;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    hi.hit = false;
    if (ray.insideObject != nullptr)
      hi.rayparam = 0.0;

    // mit allen Seitenflächen testen
    for (int i = 0; i < 4; i++ ) {
      HitInfo hitest = sides.at(i).isHitBy( ray );

      if (ray.insideObject == nullptr) {
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

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return hi;
  }

  void Tetraeder::setPos( const Vector3d& cen, double dim, const Vector3d& rot )
  {
    //  Rotation um durch rot gegebene Achse mit durch norm(rot) gegebenem
    //  Winkel in Grad
    position = cen;
    center = cen;
    dimension = dim;
    rotation = rot;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Vektoren vom Zentrum des Tetraeders zu den Ecken bestimmen
    Vector3d v1(0,dim,0);  // nach oben zeigender Vektor
    Vector3d v2;           // nach hinten unten zeigender Vektor
    Vector3d v3;           // nach vorne links unten zeigender Vektor
    Vector3d v4;           // nach vorne rechts unten zeigender Vektor

    double angledown = 2*atan( sqrt(2) );
    auto rad = [](double d) { return 2*M_PI*d/360.0; };

    AngleAxisd aa = AngleAxisd(-angledown,Vector3d(1,0,0));
    v2 = aa.matrix()*v1;
    aa = AngleAxisd(rad(120),Vector3d(0,1,0));
    v3 = aa.matrix()*v2;
    v4 = aa.matrix()*v3;

    // Rotation anwenden
    if ( rotation.norm() > 0 ) {
      aa = AngleAxisd(rad(rotation.norm()),rotation/rotation.norm());
      Matrix3d rotmat = aa.matrix();
      v1 = rotmat*v1;
      v2 = rotmat*v2;
      v3 = rotmat*v3;
      v4 = rotmat*v4;
    }

    // Positionen der Ecken des Tetraeders bestimmen
    Vector3d p1 = cen + v1;
    Vector3d p2 = cen + v2;
    Vector3d p3 = cen + v3;
    Vector3d p4 = cen + v4;

    // Seitenflächen erstellen
    sides.at(0) = Triangle( p1, p2, p3 );
    sides.at(1) = Triangle( p1, p2, p4 );
    sides.at(2) = Triangle( p1, p3, p4 );
    sides.at(3) = Triangle( p2, p3, p4 );

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  }

}
