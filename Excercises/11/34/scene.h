// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Eigen/Core>

#include "object.h"
#include "camera.h"
#include "light.h"

using namespace std;
using namespace Eigen;

namespace rtai {

  class Scene
  {
  public:
    static string extension;

    void loadFromFile(const string &file);

    // Hintergrundfarbe
    void setBackground( const Vector3d &col );
    void setBackground( double r, double g, double b );
    inline Vector3d getBackground() const { return rgbBackground; };

    // Kamera
    Camera& getCamera();
    const Camera& getCamera() const;

    // Licht
    void setAmbient( const Vector3d &col );
    void setAmbient( double r, double g, double b );
    Vector3d getAmbient() const;
    void addLight( const std::shared_ptr<Light> &light );

    // Objekte
    void addObject( const std::shared_ptr<Object> &object );

  private:
    // Hintergrundfarbe
    Vector3d rgbBackground;

    // Kamera
    Camera cam;

    // Licht
    Vector3d rgbAmbient;
    vector<std::shared_ptr<Light>> lights;

    // Objekte
    vector<std::shared_ptr<Object>> objects;

    friend class Raytracer;
    friend class Light;
  };

}
