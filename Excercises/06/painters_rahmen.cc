// Painter's Algorithm
// Autor:  Sebastian Birk, Martin Galgon, Holger Arndt
// Datum:  02.12.2015

#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

#include <cppqt.h>

using namespace std;

DPoint2D homTo2D(const Vec4D& v)
{
  // nur sinnvoll, wenn durch Projektion bereits v.el[2] == 0 ist
  return DPoint2D(v.el[0] / v.el[3], v.el[1] / v.el[3]);
}

class Dreieck                   // ein Dreieck im Raum mit homogenen Koordinaten
{
public:
  Vec4D ecke[3];                                                        // Ecken
  DrawColour col;                                                       // Farbe
};

// ##############################
// Berechnung der Parallelprojektions-Matrix

Matrix4x4 berechneMpar(double umin, double umax, double vmin, double vmax,
                       double& ratio)
{
  // Erzeugt die Matrix zur Parallelprojektion in gegebener Richtung
  // mit gegebenem Projektionsfenster [umin; umax] × [vmin; vmax]
  // in das Einheitsquadrat
  //
  // Ausgabe:
  //   ratio   Seitenverhältnis des Projektionsfensters

  Matrix4x4 mpar;

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  ratio = (umax - umin) / (vmax - vmin);

  return mpar;
}

// ##############################
// Fläche berechnen

void erzeugeFlaeche(double xmin, double xmax, double zmin, double zmax, int num,
                    const std::function<double(double, double)>& func,
                    std::vector<Dreieck>& dreiecke )
{
  // Ausgabe:
  //   Der vector dreiecke enthält alle zu zeichnenden Dreiecke
  //   Zu jedem Dreieck gehören die Eckpunkte und die Farbe.
  //   wichtig: Die Dreiecke müssen von hinten nach vorne gezeichnet werden,
  //            weil sie einfach überzeichnen und kein z-Buffer verwendet wird.
  //
  // Eingabe:
  //   xmin, xmax, zmin, zmax - Ausdehnung der Fläche
  //   num                    - Anzahl der Flächen-Stücke in jede Richtung
  //                          - Jedes Flächen-Stück besteht aus zwei Dreiecken.
  //   func                   - auszuwertende Funktion: y = func(x, z)

  // Hinweis:
  //   s-Achse entspricht x
  //   f-Achse entspricht y
  //   t-Achse entspricht z
  //   mit f(s, t) = … ergibt sich also y(x, z) = … in unserem Koordinatensystem

  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void maleDreiecke(Drawing& pic, const std::vector<Dreieck>& dreiecke,
                  const Matrix4x4& mpar, double ratio)
{
  // malt die transformierten Dreiecke ins Bild pic
  int n = dreiecke.size();
  std::vector<int> linkerrand(pic.getHeight());    // x-Bereich für ein festes y
  std::vector<int> rechterrand(pic.getHeight());

  int wid, hei;
  int shix, shiy;

  double wratio = static_cast<double>(pic.getWidth())
    / static_cast<double>(pic.getHeight());

  if (ratio > wratio)
    {
      wid = pic.getWidth();
      hei = wid / ratio;
      shix = 0;
      shiy = (pic.getHeight() - hei) / 2;
    }
  else
    {
      hei = pic.getHeight();
      wid = hei * ratio;
      shix = (pic.getWidth() - wid) / 2;
      shiy = 0;
    }

  // Schleife über die Dreiecke
  for (int i = 0; i < n; ++i)
    {
      // 1. Führe Parallelprojektion aus
      Vec4D eckenPar[3];
      eckenPar[0] = mpar * dreiecke[i].ecke[0];
      eckenPar[1] = mpar * dreiecke[i].ecke[1];
      eckenPar[2] = mpar * dreiecke[i].ecke[2];

      // 2. Umwandlung von 3D homogen nach 2D und Fensterkoordinaten
      DPoint2D eckenNorm[3];
      eckenNorm[0] = homTo2D(eckenPar[0]);
      eckenNorm[1] = homTo2D(eckenPar[1]);
      eckenNorm[2] = homTo2D(eckenPar[2]);
      eckenNorm[0].x = eckenNorm[0].x * wid + shix;
      eckenNorm[0].y = eckenNorm[0].y * hei + shiy;
      eckenNorm[1].x = eckenNorm[1].x * wid + shix;
      eckenNorm[1].y = eckenNorm[1].y * hei + shiy;
      eckenNorm[2].x = eckenNorm[2].x * wid + shix;
      eckenNorm[2].y = eckenNorm[2].y * hei + shiy;

      IPoint2D ecken2D[3];
      ecken2D[0] = round(eckenNorm[0]);
      ecken2D[1] = round(eckenNorm[1]);
      ecken2D[2] = round(eckenNorm[2]);

      // 3. Male Dreiecke
      // bestimme y-Intervall
      int ymin = min(min(ecken2D[0].y, ecken2D[1].y), ecken2D[2].y);
      int ymax = max(max(ecken2D[0].y, ecken2D[1].y), ecken2D[2].y);

      // Test, ob y-Koordinate im Bild liegt
      auto ytest = [&pic](int y) { return (0 <= y) && (y < pic.getHeight()); };

      // bestimme nun anhand der 3 Seiten für jede Bildzeile den x-Bereich
      for (int y = ymin; y <= ymax; ++y)
        if (ytest(y))
          {
            linkerrand[y] = numeric_limits<int>::max();
            rechterrand[y] = -1;
          }
      for (int j = 0; j < 3; ++j)
        {
          int k = (j + 1) % 3;
          IPoint2D e1;
          IPoint2D e2;
          if (ecken2D[j].y < ecken2D[k].y)
            {
              e1 = ecken2D[j];
              e2 = ecken2D[k];
            }
          else
            {
              e1 = ecken2D[k];
              e2 = ecken2D[j];
            }
          if (e1.y != e2.y)
            {
              double einsdurchm = static_cast<double>(e1.x - e2.x)
                / (e1.y - e2.y);
              double x = e1.x;
              for (int y = e1.y; y <= e2.y; ++y)
                {
                  if (x < linkerrand[y])
                    {
                      if (ytest(y))
                        linkerrand[y] = static_cast<int>(round(x));
                    }
                  if (x > rechterrand[y])
                    {
                      if (ytest(y))
                        rechterrand[y] = static_cast<int>(round(x));
                    }
                  x += einsdurchm;
                }
            }
        }

      // zeichne das Dreieck
      for (int y = ymin; y <= ymax; ++y)
        {
          if (ytest(y))
            for (int ix = linkerrand[y]; ix < rechterrand[y]; ++ix)
              pic.drawPoint(ix, y, dreiecke[i].col);
        }
      if (i % 4 == 0)
        IOThread::msleep(10);
    }
}

int maindraw()
{
  Drawing pic(1200, 800, DrawColour(255, 255, 255));
  pic.show();

  std::vector<Dreieck> dreiecke;  // die Dreiecke
  Matrix4x4 mpar;                 // Transformation M_par für Parallelprojektion

  // Verhältnis eigentlich 2:1, Fenster ist 3:2.
  double umin = -8;
  double umax = 8;
  double vmin = 2;
  double vmax = 10;

  double xmin = -5;
  double xmax = 5;
  double zmin = -5;
  double zmax = 5;
  int num = 30;

  auto func = [](double s, double t)
    { return 10 * exp(-(s * s / 25 + t * t / 100)); };
  // Alternative zum Testen:
  //{ return 5 + 5 * cos(s + t) * cos(s - t) * exp(-(s * s + t * t) / 12); };

  erzeugeFlaeche(xmin, xmax, zmin, zmax, num, func, dreiecke);

  double ratio;
  mpar = berechneMpar(umin, umax, vmin, vmax, ratio);
  maleDreiecke(pic, dreiecke, mpar, ratio);

  pic.savePNG("painters.png");

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
