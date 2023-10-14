// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

using namespace Eigen;

namespace rtai {

  class Object;

  typedef struct {
    Vector3d origin;
    Vector3d direction;
    Object* insideObject {nullptr};
  } Ray;

}
