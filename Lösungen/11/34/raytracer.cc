// Raytracer
// Autor: Sebastian Birk, Martin Galgon, Holger Arndt
// Datum: 22.12.2015

#include <algorithm>
#include <limits>
#include <cmath>

#include <Eigen/Core>
#include <Eigen/Dense>

#include "raytracer.h"
#include "ray.h"

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

using namespace std;
using namespace Eigen;

namespace rtai {

  void Raytracer::raytrace( const Scene &scene, Image &image )
  {
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Ecke und spannende Vektoren der Projektionsebene bestimmen
    Vector3d campos = scene.getCamera().getPosition();
    Vector3d camdir = scene.getCamera().getDirection();
    Vector3d up     = scene.getCamera().getUp();
    double horangle = scene.getCamera().getHorAngle();
    double aspect   = static_cast<double>(image.getWidth())/image.getHeight();

    Vector3d planeright = camdir.cross(up);
    Vector3d planedown  = camdir.cross(planeright);

    camdir     /= camdir.norm();
    planeright /= planeright.norm();
    planedown  /= planedown.norm();

    double pi = M_PI;
    planeright *= tan(horangle * pi / 360.0);
    planedown  *= tan(horangle * pi / 360.0) / aspect;

    Vector3d topleft = campos + camdir - planeright - planedown;

    // Größe des Bildes ermitteln und temporäres Bild erzeugen
    int w, h;

    w = image.getWidth();
    h = image.getHeight();

    // Ein größeres Bild ist nicht nötig.
    vector<vector<Vector3d>> tmpImage;
    tmpImage.resize( h );
    for (int y = 0; y < h; y++) {
      tmpImage[y].resize( w, Vector3d(0, 0, 0) );
    }

    // Pixelmaße
    double w1 = 1.0 / w;
    double h1 = 1.0 / h;
    double w1_halbe = 0.5 * w1;
    double h1_halbe = 0.5 * h1;

    // alle Punkte auf der Projektionsebene erzeugen und Strahlen verfolgen
    Ray currentray;
    currentray.origin = campos;
    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        currentray.direction = topleft - campos
                             + 2 * planeright * ( x * w1 + w1_halbe )
                             + 2 * planedown  * ( y * h1 + h1_halbe );

        tmpImage[y][x] +=  raytrace( scene, currentray, 1 );
        // Farben im resultierenden Bild setzen + Beobachtung des Bildaufbaus
        Vector3d col;
       
        col = tmpImage[y][x] ;
        clampCol(col);
        image.setPixel( x, y, col );
      }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  }

  Vector3d Raytracer::raytrace( const Scene &scene, const Ray &ray, int dep,
                                Object *origin )
  {
    Vector3d rgbCol;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    HitInfo hi;
    for (auto o: scene.objects) {
      if (o.get() == origin)
        continue;

      HitInfo hitest = o->isHitBy( ray );
      if ( hitest.hit
           && (hitest.rayparam < hi.rayparam)
           && (hitest.rayparam >= 0) ) {
        hi = hitest;
      }
    }

    if (!hi.hit) {
      rgbCol = scene.getBackground();
    }
    else {
      // Ambienter Anteil
      rgbCol = scene.getAmbient().cwiseProduct(hi.object->getAmbient());

      // Lichtbeitrag aller Lichtquellen ermitteln
      VisibleInfo vi;
      for (auto l: scene.lights) {
        vi = l->visibleFrom( scene, hi, hi.object );

        if (vi.visible) {
          // diffuser und winkelabhängiger Anteil
          Vector3d diffspec(0,0,0);   // <-- kombinierter diffuser und
          //                                 winkelabhängiger Lichtanteil

          double dotNormalLightDir = hi.normal.dot(vi.direction);
          if (dotNormalLightDir > 0) {
            // diffuser Anteil
            diffspec = dotNormalLightDir*hi.object->getDiffuse();

            // winkelabhängiger Anteil
            Vector3d tocam = -ray.direction/ray.direction.norm();
            Vector3d lightreflectdir
              = 2*dotNormalLightDir*hi.normal-vi.direction;
            double dotTocamLightreflect = tocam.dot(lightreflectdir);
            if (dotTocamLightreflect > 0)
              diffspec += pow(dotTocamLightreflect,
                              hi.object->getSpecularityExponent())
                *hi.object->getSpecular();
          }

          // zusammen zur Farbe
          rgbCol += l->getAttenuation(vi.distance)  // is this g?
            *l->getIntensity().cwiseProduct(diffspec);
        }
      }

      // Spiegelung
      if (hi.object->getReflective() && dep < depth) {
        Vector3d tocam = -ray.direction/ray.direction.norm();
        Ray reflray;
        reflray.origin = hi.position;
        reflray.direction = 2*tocam.dot(hi.normal)*hi.normal-tocam;
        Vector3d rgbRefl = raytrace( scene, reflray, dep+1, hi.object );
        rgbCol += rgbRefl.cwiseProduct(hi.object->getReflection());
      }

    
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return rgbCol;
  }

  void Raytracer::clampCol( Vector3d &rgb )
  {
    rgb[0] = min(rgb[0],1.0);
    rgb[1] = min(rgb[1],1.0);
    rgb[2] = min(rgb[2],1.0);
    rgb[0] = max(rgb[0],0.0);
    rgb[1] = max(rgb[1],0.0);
    rgb[2] = max(rgb[2],0.0);
  }

}
