// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <limits>

#include <Eigen/Core>

using namespace Eigen;

namespace rtai {

  class Object;

  typedef struct {
    bool hit {false};
    // rayparam:   position = origin + rayparam*direction
    // -> für Entfernungsbestimmung
    double rayparam {std::numeric_limits<double>::max()};
    Vector3d position {0,0,0};
    Vector3d normal {0,0,0};
    Object *object {nullptr};
  } HitInfo;

}
