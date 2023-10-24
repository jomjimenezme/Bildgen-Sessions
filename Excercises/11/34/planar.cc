// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <cmath>
#include <utility>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "planar.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  void Planar::setPos( const Vector3d& p1, const Vector3d& p2,
                       const Vector3d& p3 )
  {
    vert1 = p1;
    vert2 = p2;
    vert3 = p3;

    d1 = vert3 - vert1;
    d2 = vert2 - vert1;

    normal = d1.cross(d2);
    normal = normal/normal.norm();

    // damit da was sinnvolles steht
    position = vert1 + (1.0/3.0)*vert1 + (1.0/3.0)*vert2;
  }

  Vector3d Planar::getParams( const Ray &ray )
  {
    Vector3d params;

    if (ray.direction.dot(normal) == 0) {
      // Sonderfall: Strahl verläuft parallel zu von d1 und d2 aufgespannter
      // Ebene
      Matrix<double,3,2> mat;
      mat.col(0) = d1;
      mat.col(1) = d2;

      Matrix<double,2,1> pars;
      pars = mat.colPivHouseholderQr().solve(vert1 - ray.origin);
      params(0) = 0.0;
      params(1) = -pars(0);
      params(2) = -pars(1);
    }
    else {
      Matrix3d mat;
      mat.col(0) = ray.direction;
      mat.col(1) = d1;
      mat.col(2) = d2;

      params = mat.colPivHouseholderQr().solve(vert1 - ray.origin);
      params(1) *= -1;
      params(2) *= -1;
    }

    return params;
  }

}
