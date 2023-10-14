// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <limits>

#include <Eigen/Core>

using namespace Eigen;

namespace rtai {

  class Light;

  typedef struct {
    bool visible {false};
    Vector3d direction {0,0,0};  // Richtung vom Punkt zur Lichtquelle, normiert
    double distance {0};
    Vector3d pos {0,0,0};
    Light *light {nullptr};
  } VisibleInfo;

}
