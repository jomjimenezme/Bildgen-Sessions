// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <Eigen/Core>

#include "ray.h"
#include "hitinfo.h"

namespace rtai {

  class Object
  {
  public:
    Object() {};
    Object( const Vector3d &pos ) : position(pos) {};
    virtual ~Object() {};

    // Position
    inline Vector3d getPosition() const { return position; };

    // Strahltest
    virtual HitInfo isHitBy( const Ray &ray ) = 0;

    // Reflexion des ambienten Lichts
    inline void setAmbient( const Vector3d &rgb ) { rgbAmbient = rgb; };
    inline Vector3d getAmbient() const { return rgbAmbient; };
    // Reflexion des diffusen Lichts
    inline void setDiffuse( const Vector3d &rgb ) { rgbDiffuse = rgb; };
    inline Vector3d getDiffuse() const { return rgbDiffuse; };
    // Reflexion des winkelabhängigen Lichts
    inline void setSpecular( const Vector3d &rgb ) { rgbSpecular = rgb; };
    inline void setSpecularityExponent( double expo )
    { specularityExponent = expo; };
    inline Vector3d getSpecular() const { return rgbSpecular; };
    inline double getSpecularityExponent() const
    { return specularityExponent; };

    // Spiegelung an der Oberfläche
    inline void setReflection( const Vector3d &rgb ) {
      reflective = (rgb.norm() != 0);
      rgbReflection = rgb;
    }
    inline bool getReflective() { return reflective; }
    inline Vector3d getReflection() { return rgbReflection; }

    // Lichtbrechung
    // Umgebung (Luft) = 1.0
    inline void setRefraction( double ref ) {
      refractive = (ref != 0.0);
      refraction = ref;
    }
    inline bool getRefractive() { return refractive; }
    inline double getRefraction() { return refraction; }
    inline void setPigment( const Vector3d &pig ) { rgbPigment = pig; }
    inline Vector3d getPigment() { return rgbPigment; }

  protected:
    Vector3d position;

    Vector3d rgbAmbient {.1,.1,.1};
    Vector3d rgbDiffuse {.1,.1,.1};
    Vector3d rgbSpecular {.1,.1,.1};
    double specularityExponent {10};  // Kapitel 7.2.3
    bool reflective {false};
    Vector3d rgbReflection {0,0,0};

    double refraction {0.0};
    bool refractive {false};
    Vector3d rgbPigment {1.0,1.0,1.0}; // Lichtdurchlässigkeit für RGB-Anteile
    //                                  des Lichts - nur, wenn refraction != 0.0
  };

}
