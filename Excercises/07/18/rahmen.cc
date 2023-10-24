// Beleuchtung nach Phong
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
#include "proj-clip.h"

#include "zbuffer.h"

VecRGB berechneBeleuchtung(const Vec3D& ecke, const Vec3D& normale,
                           const Vec3D& auge, const Vec3D& licht,
                           const DrawColour& farbe)
{
  // Explizite Eigenschaften für die Lichtquelle als Vektor
  // weißes Licht für alle drei Arten
  VecRGB lightAmbient(1, 1, 1);
  VecRGB lightDiffuse(1, 1, 1);
  VecRGB lightSpecular(1, 1, 1);

  // Explizite Eigenschaften für das Material als Vektor
  VecRGB material(farbe.red() / 255.0, farbe.green() / 255.0,
                  farbe.blue() / 255.0);
  VecRGB materialAmbient(0.5 * material);
  VecRGB materialDiffuse(0.95 * material);
  VecRGB materialSpecular(0.4 * material + 0.3 * VecRGB(1, 1, 1));
  double materialSpecularity = 1.2;

  // Konstanten für entfernungsabhängige Dämpfung (attenuation)
  double c0 = 1.0;
  double c1 = 0.00001;
  double c2 = 0.0000004;

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Fertige Farbe
  VecRGB total = VecRGB(0.5, 0.5, 0.5); // ÄNDERN <<<<<<<<<<<<<<<<<<<<<<<<<
  // Farbwert auf maximal 1 begrenzen
  total.el[0] = min(total.el[0], 1.0);
  total.el[1] = min(total.el[1], 1.0);
  total.el[2] = min(total.el[2], 1.0);

  return total;
}

int maindraw()
{
  Drawing pic(1200, 800, DrawColour(192, 192, 192));
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

  Vec3D l = standardLichtQuelle(vrp, cop, vup); // Position der Lichtquelle

  maleDreiecke(pic, dreiecke, nzen, clip, doClip, cop, l, false);

  // da es bis hier u.u. lange gedauert hat, Ergebnis verewigen
  pic.savePNG("proj4.png");

  IOThread::waitForWindow(60);

  return 0;
}
