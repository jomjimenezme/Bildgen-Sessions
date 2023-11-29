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

  double fov = 90.0; // 90° Sichtbereich
  double aspect = static_cast<double>(w) / static_cast<double>(h); // Seitenverhältnis
  vmax = znear * tan(fov * M_PI / 360.0); // Halber Winkel
  vmin = -vmax;
  umin = vmin * aspect;
  umax = vmax * aspect;

  // *** Hinweise:
  // Zugriff auf Matrixelemente: matrix.el[#][#], # aus [0,3]
  // Zugriff auf 4D-Vektorelemente: vektor.el[#], # aus [0,3]
  // Zugriff auf 3D-Vektorelemente: vektor.el[#], # aus [0,2]

  // 1. Verschiebung von vrp in den Ursprung (Seite 4-18, 1.)

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 2. a) Bestimme (u,v,n)-Koordinatensystem (Seite 4-18, 2.)

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 2. b) Rotation: (x, y, z) ⇝ (u, v, n) (Seite 4-18, 2.)

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 3. Verschieben des transformierten Augenpunktes in den Ursprung
  // (Seite 4-19, 3.)

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 4. Standard perspektivische Projektion (Seite 4-24)

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 5. a) Translation des Bereichs [umin; umax] × [vmin; vmax]
  //       so dass (umin, vmin) ⇝ (0, 0)

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // 5. b) Skalierung, so dass das Fenster in das Einheitsquadrat passt.
  double ft;         // finaler Streckfaktor

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Wir merken uns die Ausdehnung des Bildes im Einheitsquadrat
  un = (umax - umin) * ft;
  vn = (vmax - vmin) * ft;

  // 6. Multiplikation der einzelnen Transformationen

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  return mzen;
}

void maleLinien(Drawing& pic, const vector<Kante>& kanten,
                const Matrix4x4& t, double un, double vn)
{
  // malt die transformierten Kanten ins Bild pic

  Vec4D nanf, nend;    // Anfangs- und Endpunkt nach Normalisierung
  DPoint2D panf, pend; // Anfangs- und Endpunkt im Bild pic

  int n = kanten.size();
  for (int i = 0; i < n; ++i)
    {
      // 1. Normalisiere Anfangs- und Endpunkt der Kante
      //    d.h. Tranformation in kanonischen Bildraum
      nanf = t * kanten[i].anf;
      nend = t * kanten[i].end;

      // 2. Perspektivendivision, Umwandlung von homogenen in 2D-Koordinaten.
      // *** Hinweise:
      //    nanf -> panf, nend -> pend
      //    Zugriff auf die Elemente von nanf, nend: nanf.el[#], # aus [0,3]
      //    Zugriff auf die Elemente von panf, pend: panf.x, panf.y

      // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

      // 3. Skalierung auf Gerätekoordinaten, beachte un, vn.
      //    Die Pixel liegen in beiden Richtungen gleich dicht.

      // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

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

  IOThread::waitForWindow(60);

  return 0;
}
