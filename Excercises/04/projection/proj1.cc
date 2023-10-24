// Perspektivische Projektion
// Autor: Holger Arndt, Sebastian Birk, Martin Galgon
// Datum: 01.12.2015

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include <cppqt.h>

using namespace std;

#include "in-reader.h"

Matrix4x4 berechneTransformation(const Vec3D& cop, const Vec3D& vrp,
                                 const Vec3D& vup, int w, int h,
                                 double& un, double& vn)
{
  // Berechnet die normalisierende Transformation mzen, die die Projektion
  // in den kanonischen Bildraum bewirkt für:
  // IN:  Projektionszentrum cop (Augenposition)
  // IN:  Bildebenenursprung vrp
  // IN:  Aufwärtsvektor vup
  // IN:  Fensterdimensionen (Breite, Höhe) w, h.
  // OUT: Ausdehnung des Bildes im Einheitsquadrat [0; un], [0; vn].

  Matrix4x4 mzen;     // fertige Matrix
  Matrix4x4 tvrp;     // Translation vrp
  Matrix4x4 rot;      // Rotation uvn
  Matrix4x4 transcop; // Translation des transformierten cop in den Ursprung
  Matrix4x4 proj;     // Projektion
  Matrix4x4 t;        // Translation für normalisierte Koordinaten
  Matrix4x4 s;        // Skalierung für normalisierte Koordinaten
  Vec3D u, v, n;
  Vec3D vpn;          // Viewport normal

  int i;
  double umin, umax, vmin, vmax;
  double znear;

  // Da wir (zumindest hier) immer senkrecht auf die Projektionsebene
  // schauen wollen, legen wir die Normale fest wie folgt.

  vpn = cop - vrp;
  znear = norm(vpn); // Der Abstand zur Projektionsebene

  // Um die Projektion möglichst unabhängig von der Größe des späteren
  // Fensters zu machen, verwendet man lediglich das Seitenverhältnis
  // sowie den Winkel des Sichtbereichs, fov (field of view), um die
  // Begrenzungen des viewports, umin, umax, vmin und vmax, zu bestimmen.
  // Diese Werte können auch fest vorgegeben werden um z.B. eine schiefe
  // Projektion zu erhalten.

  double fov = 90.0;                      // 90° Sichtbereich
  double aspect = static_cast<double>(w) / static_cast<double>(h);
  //                                                            Seitenverhältnis
  vmax = znear * tan(fov * M_PI / 360.0); // Halber Winkel
  vmin = -vmax;
  umin = vmin * aspect;
  umax = vmax * aspect;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 1. Verschiebung von vrp in den Ursprung (Seite 4-18, 1.)
  /*
    / 1     -vrpx \
    |   1   -vrpy |
    |     1 -vrpz |
    \         1   /
  */
  tvrp.el[0][0] = tvrp.el[1][1] = tvrp.el[2][2] = tvrp.el[3][3] = 1;
  tvrp.el[0][3] = -vrp.el[0];
  tvrp.el[1][3] = -vrp.el[1];
  tvrp.el[2][3] = -vrp.el[2];

  // 2. a) Bestimme (u,v,n)-Koordinatensystem (Seite 4-18, 2.)
  n = vpn / norm(vpn);
  // Projektion von vup auf die Bildebene (Gram-Schmidt)
  v = vup - skalarprod(vup, n) * n;
  v = v / norm(v);
  // Das Kreuzprodukt zweier normierter Vektoren ist selbst normiert.
  u = kreuzprod(v, n);

  // 2. b) Rotation: (x, y, z) ⇝ (u, v, n) (Seite 4-18, 2.)
  /*
    / ux uy uz   \
    | vx vy vz   |
    | nx ny nz   |
    \          1 /
  */
  for (i = 0; i < 3; ++i)
    {
      rot.el[0][i] = u.el[i];
      rot.el[1][i] = v.el[i];
      rot.el[2][i] = n.el[i];
    }
  rot.el[3][3] = 1;

  // 3. Verschieben des transformierten Augenpunktes in den Ursprung
  // (Seite 4-19, 3.)
  /*
    / 1         \
    |   1       |
    |     1 -zn |
    \        1  /
  */
  transcop.el[0][0] = transcop.el[1][1] = transcop.el[2][2] = transcop.el[3][3]
    = 1;
  transcop.el[2][3] = -znear;


  // 4. Standard perspektivische Projektion (Seite 4-24)
  /*
    / -zn         \
    |     -zn     |
    |             |
    \         1   /
  */
  proj.el[0][0] = proj.el[1][1] = -znear;
  proj.el[2][2] = 0;
  proj.el[3][2] = 1;

  // 5. a) Translation des Bereichs [umin; umax] × [vmin; vmax]
  //       so dass (umin, vmin) ⇝ (0, 0)
  /*
    / 1     -umin \
    |   1   -vmin |
    |     ?       |
    \         1   /
  */
  t.el[0][0] = t.el[1][1] = t.el[3][3] = 1;
  t.el[2][2] = 0; 
  t.el[0][3] = -umin;
  t.el[1][3] = -vmin;

  // 5. b) Skalierung, so dass das Fenster in das Einheitsquadrat passt.
  /*
    / ft        \
    |    ft     |
    |       ?   |
    \         1 /
  */
  double fu = 1.0 / (umax - umin); // Faktor für u
  double fv = 1.0 / (vmax - vmin); // Faktor für v
  double ft = min(fu, fv);         // finaler Streckfaktor
  s.el[0][0] = s.el[1][1] = ft;
  //s.el[2][2] = 0; // Die z-Koordinate interessiert uns hier nicht
  s.el[3][3] = 1;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // Wir merken uns die Ausdehnung des Bildes im Einheitsquadrat
  un = (umax - umin) * ft;
  vn = (vmax - vmin) * ft;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 6. Multiplikation der einzelnen Transformationen
  mzen =  s * t * proj * transcop * rot * tvrp;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  return mzen;
}

void maleLinien(Drawing& pic, const vector<Kante>& kanten,
                const Matrix4x4& t, double un, double vn)
{
  // malt die transformierten Kanten ins Bild pic

  Vec4D nanf, nend;    // Anfangs- und Endpunkt nach Normalisierung
  DPoint2D panf, pend; // Anfangs- und Endpunkt im Bild pic

  int n = kanten.size();
  // Skalierungsfaktoren, s.u.
  double sx = pic.getWidth() / un;
  double sy = pic.getHeight() / vn;

  for (int i = 0; i < n; ++i)
    {
      // 1. Normalisiere Anfangs- und Endpunkt der Kante
      //    d.h. Tranformation in kanonischen Bildraum
      nanf = t * kanten[i].anf;
      nend = t * kanten[i].end;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

      // 2. Perspektivendivision, Umwandlung von homogenen in 2D-Koordinaten.
      panf.x = nanf.el[0] / nanf.el[3];
      panf.y = nanf.el[1] / nanf.el[3];
      pend.x = nend.el[0] / nend.el[3];
      pend.y = nend.el[1] / nend.el[3];

      // 3. Skalierung auf Gerätekoordinaten, beachte un, vn.
      //    Die Pixel liegen in beiden Richtungen gleich dicht.
      panf.x *= sx;
      panf.y *= sy;
      pend.x *= sx;
      pend.y *= sy;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

      // 4. Male Linien
      pic.drawLine(panf, pend, kanten[i].col, n < 100 || i % (n / 500) == 0);
    }
}

int maindraw()
{
  Drawing pic(1200, 800);
  pic.show();

  vector<Dreieck> dreiecke;                       // die Dreiecke selbst
  vector<Kante> kanten;                             // die Kanten selbst
  Vec3D cop;                     // center of projection = Augenposition
  Vec3D tgt;                              // target = Betrachteter Punkt
  Vec3D vrp;                // view reference point = Bildebenenursprung

  double un, vn;                      // merkt sich später die Bildgröße
  Vec3D vup(0, 1, 0);               // view-up vector = Aufwärtsrichtung
  Matrix4x4 mzen;                   // Transformation zur Normalisierung
                                                   //auf kanon. Bildraum

  modellEinlesen(dreiecke, cop, tgt);
  dreieckeToKanten(dreiecke, kanten);

  vrp = lookAt(cop, tgt);
  mzen = berechneTransformation(cop, vrp, vup,
                                pic.getWidth(), pic.getHeight(),
                                un, vn);

  maleLinien(pic, kanten, mzen, un, vn);

  pic.savePNG("proj1.png");

  IOThread::waitForWindow(10);

  return 0;
}
