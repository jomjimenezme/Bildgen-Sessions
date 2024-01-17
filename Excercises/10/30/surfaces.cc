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

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
Vec4D mult(double s, Matrix4x4 C[3], double t)
{
  // berechnet den Punkt fuer die Parameter s und t

  Vec4D ss(s * s * s, s * s, s , 1);
  Vec4D tt(t * t * t, t * t, t , 1);
  return Vec4D( skalarprod(ss, (C[0] * tt)),
                skalarprod(ss, (C[1] * tt)),
                skalarprod(ss, (C[2] * tt)),
                1 );
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void berechneBezierFlaeche( vector<Kante>& vk, const vector<vector<Vec3D> > &p,
                            int anzkurv = 5, int anzlin = 20 )
{
  // berechnet ein Netz aus Bezier-Flaechen, alle Kantenstuecke werden dem
  // Vektor vk hinzugefuegt

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  Vec4D anf, end;
  int m = p.size() - 1;
  int n = p[0].size() - 1;
  double s, t;
  double deltakurv = 1.0 / (anzkurv - 1);
  double deltalin = 1.0 / anzlin;
  double mbel[4][4] = { { -1,  3, -3, 1 },
                        {  3, -6,  3, 0 },
                        { -3,  3,  0, 0 },
                        {  1,  0,  0, 0 } };
  Matrix4x4 MB(mbel);                                      // Bezier-Basismatrix
  Matrix4x4 G;                                                // Geometriematrix
  Matrix4x4 C[3];                                    // C[i] = MB^T * GB[i] * MB
  int i, j, k, l, d;

  if (m % 3 != 0)
  {
    cerr << "FEHLER: m != 3k+1" << endl;
    exit(1);
  }
  if (n % 3 != 0)
  {
    cerr << "FEHLER: n != 3k+1" << endl;
    exit(1);
  }

  // Schleifen ueber die Einzel-Flaechen
  for (k = 3; k <= m; k += 3)
    for (l = 3; l <= n; l += 3)
    {
      // berechne vorweg die Matrizen C[i]
      for (d = 0; d < 3; d++)
      {
        for (i = 0; i < 4; i++)
          for (j = 0; j < 4; j++)
            G.el[i][j] = p[k - 3 + i][l - 3 + j].el[d];
        C[d] = MB * G * MB; // MB^T = MB
      }
      // Linien fuer jeweils festes s
      for (i = 0, s = 0; i < anzkurv; i++, s += deltakurv)
      {
        end = mult(s, C, 0);
        for (j = 1, t = deltalin; j <= anzlin; j++, t += deltalin)
        {
          anf = end;
          end = mult(s, C, t);
          if (i == 0 || i == anzkurv - 1)
            vk.push_back(Kante(anf, end, BLUE));
          else
            vk.push_back(Kante(anf, end, BLACK));
        }
      }
      // Linien fuer jeweils festes t
      for (i = 0, t = 0; i < anzkurv; i++, t += deltakurv)
      {
        end = mult(0, C, t);
        for (j = 1, s = deltalin; j <= anzlin; j++, s += deltalin)
        {
          anf = end;
          end = mult(s, C, t);
          if (i == 0 || i == anzkurv - 1)
            vk.push_back(Kante(anf, end, BLUE));
          else
            vk.push_back(Kante(anf, end, BLACK));
        }
      }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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

  IOThread::waitForWindow(15);

  return 0;
}
