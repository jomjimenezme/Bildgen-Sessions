// z-Buffer-Verfahren
// Autor: Holger Arndt, Sebastian Birk, Martin Galgon
// Datum: 04.12.2015

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

#include <cppqt.h>

using namespace std;

#include "in-reader.h"
#define CPPQT_DUMMY_CLIP3D
#define CPPQT_DUMMY_LIGHTING
#include "proj-clip.h"

#include "zbuffer.h"

int maindraw()
{
  Drawing pic(1200, 800);
  pic.show();

  bool doClip = true;                                // clipping ja/nein

  vector<Dreieck> dreiecke;                       // die Dreiecke selbst
  ClipQuad clip;                                 // clipping Information
  Vec3D cop;                     // center of projection = Augenposition
  Vec3D tgt;                              // target = Betrachteter Punkt
  Vec3D vrp;                // view reference point = Bildebenenursprung

  Vec3D vup(0, 1, 0);               // view-up vector = Aufwärtsrichtung
  Matrix4x4 nzen;                   // Transformation zur Normalisierung 
                                                   //auf kanon. Bildraum

  modellEinlesen(dreiecke, cop, tgt);
  clipInfoEinlesen(clip);

  vrp = lookAt(cop, tgt);
  nzen = berechneTransformation(cop, vrp, vup, clip,
                                pic.getWidth(), pic.getHeight());

  maleClipRegion(pic, clip);

  maleDreiecke(pic, dreiecke, nzen, clip, doClip);

  // da es bis hier u.u. lange gedauert hat, Ergebnis verewigen
  pic.savePNG("proj3.png");

  IOThread::waitForWindow(6);

  return 0;
}
