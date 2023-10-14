// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

#include "ray.h"
#include "object.h"
#include "planar.h"

namespace rtai {

  class Triangle : public Planar
  {
  public:
    using Planar::Planar;

    HitInfo isHitBy( const Ray &ray ) override;
  };

}
