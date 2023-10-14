// Bézier-Flächen
// Autor: Holger Arndt, Sebastian Birk, Martin Galgon
// Datum: 08.01.2007

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <cppqt.h>

using namespace std;

const DrawColour BLACK = DrawColour(0, 0, 0);
const DrawColour BLUE = DrawColour(0, 0, 255);
const DrawColour RED = DrawColour(255, 0, 0);

#include "in-reader.h"
#define CPPQT_DUMMY_CLIP3D
#define CPPQT_DUMMY_CLIP3DPOINT
#define CPPQT_DUMMY_DRAWPOINTZ
#define CPPQT_DUMMY_LIGHTING
#include "proj-clip.h"

#include "splinesurf.h"

void berechneBezierFlaeche( vector<Kante>& vk, const vector<vector<Vec3D> > &p,
                            int anzkurv = 5, int anzlin = 20 )
{
  // berechnet ein Netz aus Bezier-Flaechen, alle Kantenstuecke werden dem
  // Vektor vk hinzugefuegt

  // HIER ERGAENZEN
}

int maindraw()
{
  Drawing pic(1200, 800);
  pic.show();

  vector<vector<Vec3D> > p;
  int c;
  int anzkurv, anzlin;

  cout << "Art der Flaeche (0..3): ";
  cin >> c;
  cin.ignore(9999, '\n');
  cout << "Anzahl der Kurven pro Flaechenstueck: ";
  cin >> anzkurv;
  cin.ignore(9999, '\n');
  cout << "Anzahl der Geradenstuecke pro Kurve: ";
  cin >> anzlin;
  cin.ignore(9999, '\n');
  cout << endl;

  p = initSurface(c);

  vector<Kante> kanten;
  berechneBezierFlaeche(kanten, p, anzkurv, anzlin);
  berechneBezierPunkte(kanten, p);

  cout << kanten.size() << " Kanten berechnet; beginne Zeichnung" << endl;

  ClipQuad clip = ClipQuadDefault;               // clipping Information
  Vec3D cop(400, 800, 1000);     // center of projection = Augenposition
  Vec3D tgt(0, -400, 0);                  // target = Betrachteter Punkt
  Vec3D vrp;                // view reference point = Bildebenenursprung
  Vec3D vup(0, 1, 0);               // view-up vector = Aufwärtsrichtung
  Matrix4x4 nzen;                   // Transformation zur Normalisierung
                                                   //auf kanon. Bildraum

  bool doClip = false;

  vrp = lookAt(cop, tgt);
  nzen = berechneTransformation(cop, vrp, vup, clip,
                                pic.getWidth(), pic.getHeight());

  maleClipRegion(pic, clip);

  maleLinien(pic, kanten, nzen, clip, doClip);

  pic.savePNG("surfaces.png");

  IOThread::waitForWindow(60);

  return 0;
}
