// 3D-Clipping für Perspektivische Projektion
// Autor: Holger Arndt, Sebastian Birk, Martin Galgon
// Datum: 01.12.2015

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include <cppqt.h>

using namespace std;

#include "in-reader.h"
#define CPPQT_DUMMY_CLIP3DPOINT
#define CPPQT_DUMMY_DRAWPOINTZ
#define CPPQT_DUMMY_LIGHTING
#include "proj-clip.h"

bool clip3D(Vec3D& anf, Vec3D& end, double zmin)
{
  // 3D-Clipping im kanonischen Bildraum der Zentralprojektion
  
  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  
  return true;
}

int maindraw()
{
  Drawing pic(1200, 800);
  pic.show();

  bool doClip = true;                                // clipping ja/nein

  vector<Dreieck> dreiecke;                       // die Dreiecke selbst
  vector<Kante> kanten;                             // die Kanten selbst
  ClipQuad clip;                                 // clipping Information
  Vec3D cop;                     // center of projection = Augenposition
  Vec3D tgt;                              // target = Betrachteter Punkt
  Vec3D vrp;                // view reference point = Bildebenenursprung

  Vec3D vup(0, 1, 0);               // view-up vector = Aufwärtsrichtung
  Matrix4x4 nzen;                   // Transformation zur Normalisierung 
                                                   //auf kanon. Bildraum

  modellEinlesen(dreiecke, cop, tgt);
  clipInfoEinlesen(clip);
  dreieckeToKanten(dreiecke, kanten);

  vrp = lookAt(cop, tgt);
  nzen = berechneTransformation(cop, vrp, vup, clip,
                                pic.getWidth(), pic.getHeight());

  maleClipRegion(pic, clip);

  maleLinien(pic, kanten, nzen, clip, doClip);

  pic.savePNG("proj2.png");

  IOThread::waitForWindow(60);

  return 0;
}
