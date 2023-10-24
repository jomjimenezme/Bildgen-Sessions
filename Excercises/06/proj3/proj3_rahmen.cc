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

bool clip3DPoint(const Vec3D& p, double zmin)
{
  // 3D-Clipping im kanonischen Bildraum der Zentralprojektion
  // hier für einen einzelnen Punkt statt einer Linie

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

inline void drawPointZ (Drawing& pic, int x, int y, double z,
                        vector<vector<double> >& zbuf, DrawColour colour)
{
  // malt ins Bild pic einen Punkt an die Stelle (x, y), der die Tiefe z
  // hat, falls es dort noch keinen Punkt geringerer oder gleicher Tiefe
  // gibt; in diesem Fall wird zusätzlich der Eintrag im z-Buffer
  // aktualisiert

  if (x < 0 || x >= static_cast<int>(zbuf.size())
      || y < 0 || y >= static_cast<int>(zbuf[0].size()))
    return;

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

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

  maleDreiecke(pic, dreiecke, nzen, clip, doClip);

  // da es bis hier u.u. lange gedauert hat, Ergebnis verewigen
  pic.savePNG("proj3.png");

  IOThread::waitForWindow(60);

  return 0;
}
