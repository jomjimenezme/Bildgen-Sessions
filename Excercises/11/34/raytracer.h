// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 22.12.2015

#pragma once

#include <Eigen/Core>

#include "scene.h"
#include "object.h"
#include "image.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  class Raytracer
  {
  public:
    Raytracer() : Raytracer(5) {};
    Raytracer( int d ) : depth(d) {};
    ~Raytracer() {};

    void raytrace( const Scene &scene, Image &image );

    // Rekursionstiefe der Strahlverfolung
    void setDepth( int d ) { depth = d; }
    int getDepth() { return depth; }

    // Antialiasing
    void setAntialiasing( bool d ) { antialiasing = d; }
    bool getAntialiasing() { return antialiasing; }

  private:
    int depth {10};
    bool antialiasing {true};

    Vector3d raytrace( const Scene &scene, const Ray &ray, int dep,
                       Object *origin = nullptr );
    void clampCol( Vector3d &rgb );
  };


}
