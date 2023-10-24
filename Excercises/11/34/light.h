// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

#include "visibleinfo.h"
#include "hitinfo.h"

namespace rtai {

  class Scene;
  class Object;

  // Punktförmige Lichtquelle mit Abstrahlung in alle Richtungen
  // TODO: andere Arten von Lichtquellen...
  class Light
  {
  public:
    Light() {};
    Light(const Vector3d &pos, const Vector3d &rgb)
      : position(pos), rgbIntensity(rgb) {}

    void setIntensity(const Vector3d &rgb) { rgbIntensity = rgb; }
    void setPosition(const Vector3d &pos) { position = pos; }
    void setAttenuationCoeff(const Vector3d &coeff)
    { coeffAttenuation = coeff; }

    inline Vector3d getIntensity() const { return rgbIntensity; }
    inline Vector3d getPosition() const { return position; }
    inline Vector3d getAttenuationCoeff() const { return coeffAttenuation; }

    double getAttenuation(double dist) const;

    VisibleInfo visibleFrom( const Scene &scene, const HitInfo &hi,
                             Object *origin );

  private:
    Vector3d position {0,0,0};
    Vector3d rgbIntensity {1,1,1};
    Vector3d coeffAttenuation {.5,.00001,.0000004};
    // Dämpfung, siehe 7.2.4, [c0, c1, c2]
  };

}
