// Rotationskörper
// Autor:  Holger Arndt, Sebastian Birk, Martin Galgon
// Datum:  09.01.2007

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

#include "splinerotk.h"

void berechneRotationsKoerper( vector<Kante>& vk, const vector<Vec3D> &p,
                               int anzkurv = 5, int anzlinku = 20,
                               int anzkreis = 5, int anzlinkr = 20 )
{
  // berechnet den Rotationskoerper, der durch Rotation der durch die Punkte p
  // in der x-y-Ebene gegebenen Bezierkurve um die x-Achse entsteht.

  // HIER ERGAENZEN
}

int maindraw()
{
  Drawing pic(1200, 800);
  pic.show();

  vector<Vec3D> p;
  int c;
  int anzkurv, anzlinku, anzkreis, anzlinkr;

  cout << "Art der Figur (0..4): ";
  cin >> c;
  cin.ignore(9999, '\n');
  cout << "Anzahl der Kurven: ";
  cin >> anzkurv;
  cin.ignore(9999, '\n');
  cout << "Anzahl der Geradenstuecke pro Kurvenabschnitt: ";
  cin >> anzlinku;
  cin.ignore(9999, '\n');
  cout << "Anzahl der Kreise pro Abschnitt: ";
  cin >> anzkreis;
  cin.ignore(9999, '\n');
  cout << "Anzahl der Geradenstuecke pro Kreis: ";
  cin >> anzlinkr;
  cin.ignore(9999, '\n');
  cout << endl;

  p = initRotationskoerper(c);

  vector<Kante> kanten;
  berechneRotationsKoerper(kanten, p, anzkurv, anzlinku, anzkreis, anzlinkr);
  berechneRotationsPunkte(kanten, p);

  cout << kanten.size() << " Kanten berechnet; beginne Zeichnung" << endl;

  ClipQuad clip = ClipQuadDefault;               // clipping Information
  Vec3D cop(200, 600, 3000);     // center of projection = Augenposition
  Vec3D tgt(150, 0, 0);                   // target = Betrachteter Punkt
  Vec3D vrp;                // view reference point = Bildebenenursprung
  Vec3D vup(1, 0, 0);           // ACHTUNG: Die x-Achse zeigt nach oben!
  Matrix4x4 nzen;                   // Transformation zur Normalisierung
                                                   //auf kanon. Bildraum

  bool doClip = false;

  vrp = lookAt(cop, tgt);
  nzen = berechneTransformation(cop, vrp, vup, clip,
                                pic.getWidth(), pic.getHeight());

  maleClipRegion(pic, clip);

  maleLinien(pic, kanten, nzen, clip, doClip);

  pic.savePNG("rotk.png");

  IOThread::waitForWindow(60);

  return 0;
}
