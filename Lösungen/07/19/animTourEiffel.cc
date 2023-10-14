// Rundflug um Eiffelturm
// Autor: Holger Arndt, Sebastian Birk, Martin Galgon
// Datum: 07.12.2015

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

#include <cppqt.h>

using namespace std;

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

#include "in-reader.h"
#define CPPQT_DUMMY_CLIP3D
#include "proj-clip.h"

#include "zbuffer.h"
#include "lighting.h"

int maindraw()
{
  bool doClip = false;

  vector<Dreieck> dreiecke;                       // die Dreiecke selbst
  ClipQuad clip = ClipQuadDefault;               // clipping Information
  int i;
  Vec3D cop;                     // center of projection = Augenposition
  Vec3D tgt;                              // target = Betrachteter Punkt
  Vec3D vup(0, 1, 0);               // view-up vector = Aufwärtsrichtung
  Matrix4x4 nzen;                   // Transformation zur Normalisierung
                                                   //auf kanon. Bildraum

  modellEinlesen(dreiecke, cop, tgt);

  // Position der Lichtquelle
  // Vec3D l(1,1,1);
  // Vec3D l = standardLichtQuelle(tgt, cop, vup);

  int npics = 101;
  vector<Vec3D> cops(npics);
  vector<Vec3D> vrps(npics);
  // Höhe: -662 bis 550
  double step = (550.0 + 662.0) / (npics - 1.0);
  for (i = 0; i < npics; ++i)
    {
      // 3.6° pro Schritt, 360° insgesamt
      cops[i].el[0] = 800 * cos(0.02 * M_PI * i);
      cops[i].el[1] = step * i - 662;
      cops[i].el[2] = 800 * sin(0.02 * M_PI * i);
      vrps[i].el[0] = 0;
      vrps[i].el[1] = step * i - 662;
      vrps[i].el[2] = 0;
    }

  vector<Drawing> pics(npics);
  Drawing pic(250, 400, 255);
  pic.show();
  cout << "Erzeuge Bilder:" << endl;
  for (i = 0; i < npics; ++i)
    {
      cout << i << endl;
      pic = 255;

      Vec3D l = standardLichtQuelle(vrps[i], cops[i], vup); // Position der Lichtquelle

      nzen = berechneTransformation(cops[i], vrps[i], vup, clip,
                                    pic.getWidth(), pic.getHeight());

//       maleDreiecke(pic, dreiecke, nzen, clip, doClip, cop, l, false, false); // phong
      maleDreiecke(pic, dreiecke, nzen, clip, doClip, cop, l, false, false, false); // no phong

      pics[i] = pic;
    }
  cout << endl;

  Drawing::makeanim(pics, "TourEiffel", "gif", 4);
  Drawing::makeanim(pics, "TourEiffel", "mpg");

  return 0;
}
