// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <algorithm>
#include <iostream>

#include "light.h"
#include "scene.h"
#include "object.h"

namespace rtai {

  double Light::getAttenuation( double dist ) const
  {
    Vector3d d(1.0,dist,dist*dist);
    double coeff = coeffAttenuation.dot(d);
    return min(1.0/coeff,1.0);
  }


  VisibleInfo Light::visibleFrom( const Scene &scene, const HitInfo &hipoint,
                                  Object *origin )
  {
    VisibleInfo vi;

    Ray ray;
    // position - Position der Lichtquelle
    ray.origin = hipoint.position;
    ray.direction = position - hipoint.position;
    HitInfo hi;
    hi.hit = false;
    if (ray.direction.dot(hipoint.normal) < 0) {
      vi.visible = false;
    }
    else {
      for (auto o: scene.objects) {
        if (o.get() == origin)
          continue;

        HitInfo hitest = o->isHitBy( ray );
        if (hitest.hit && hitest.rayparam <= 1) {
          hi = hitest;
          break;
        }
      }

      if (hi.hit) {
        vi.visible = false;
      }
      else {
        vi.visible = true;
        vi.direction = ray.direction;
        vi.distance = ray.direction.norm();
        vi.direction /= vi.distance;
        vi.pos = hipoint.position;
        vi.light = this;
      }
    }

    return vi;
  }

}
