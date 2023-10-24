
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

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  double c[4][2];
  Vec4D anf, end;
  Vec4D anf2, end2;
  int m = p.size() - 1;
  double t, phi;
  double deltakurv = 2.0 * M_PI / anzkurv;
  double deltakreis = 1.0 / (anzkreis - 1);
  double deltalinku = 1.0 / anzlinku;
  double deltalinkr = 2.0 * M_PI / anzlinkr;
  int i, j, k, d;

  if (m % 3 != 0)
  {
    cerr << "FEHLER: m != 3k+1" << endl;
    exit(1);
  }

  for (k = 3; k <= m; k += 3)
  {
    for (d = 0; d < 2; d++)
    {
      c[0][d] = -p[k - 3].el[d] + 3 * p[k - 2].el[d] - 3 * p[k - 1].el[d]
        + p[k].el[d];
      c[1][d] = 3 * p[k - 3].el[d] - 6 * p[k - 2].el[d]
        + 3 * p[k - 1].el[d];
      c[2][d] = -3 * p[k - 3].el[d] + 3 * p[k - 2].el[d];
      c[3][d] = p[k - 3].el[d];
    }

    // male Kurvenstuecke
    end = Vec4D(c[3][0], c[3][1], 0, 1);
    for (i = 1, t = deltalinku; i <= anzlinku; i++, t+= deltalinku)
    {
      anf = end;
      end = Vec4D(((c[0][0] * t + c[1][0]) * t + c[2][0]) * t + c[3][0],
                  ((c[0][1] * t + c[1][1]) * t + c[2][1]) * t + c[3][1],
                  0, 1);
      vk.push_back(Kante(anf, end, BLUE));
      for (j = 1, phi = deltakurv; j < anzkurv; j++, phi += deltakurv)
      {
        anf2 = Vec4D(anf.el[0], cos(phi) * anf.el[1],
                      sin(phi) * anf.el[1], 1);
        end2 = Vec4D(end.el[0], cos(phi) * end.el[1],
                      sin(phi) * end.el[1], 1);
        vk.push_back(Kante(anf2, end2, BLACK));
      }
    }
    // male Kreise
    end = Vec4D(c[3][0], c[3][1], 0, 1);
    for (i = 0, t = 0; i < anzkreis; i++, t+= deltakreis)
    {
      anf = Vec4D(((c[0][0] * t + c[1][0]) * t + c[2][0]) * t + c[3][0],
                  ((c[0][1] * t + c[1][1]) * t + c[2][1]) * t + c[3][1],
                  0, 1);
      end2 = anf;
      for (j = 1, phi = deltalinkr; j <= anzlinkr; j++, phi += deltalinkr)
      {
        anf2 = end2;
        end2 = Vec4D(anf.el[0], cos(phi) * anf.el[1],
                      sin(phi) * anf.el[1], 1);
        if (i == 0 || i == anzkreis - 1)
          vk.push_back(Kante(anf2, end2, BLUE));
        else
          vk.push_back(Kante(anf2, end2, BLACK));
      }
    }
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
