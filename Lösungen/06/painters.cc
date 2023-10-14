// Painter's Algorithm

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

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  // cos(30°) = Ankathete/Hypotenuse
  // sin(30°) = Gegenkathete/Hypotenuse
  // len(Hypotenuse) = 0.5
  // => Ankathete    = 0.5 * cos(30°) = 0.5 * sqrt(3)/2 = sqrt(3)/4
  // => Gegenkathete = 0.5 * sin(30°) = 0.5 * 0.5 = 0.25

  /*  /  1    sx    \
   *  |     1 sy    |
   *  |        0    |
   *  \           1 /  */
  Matrix4x4 spar;
  spar.el[0][0] = spar.el[1][1] = spar.el[3][3] = 1;
  spar.el[0][2] = -sqrt(3) / 4.0;
  spar.el[1][2] =     -1.0 / 4.0;

  /*  /  1       tx \
   *  |     1    ty |
   *  |        1    |
   *  \           1 /  */
  Matrix4x4 transl;
  transl.el[0][0] = transl.el[1][1] = transl.el[2][2] = transl.el[3][3] = 1;
  transl.el[0][3] = -umin;
  transl.el[1][3] = -vmin;

  /*  / sx          \
   *  |    sy       |
   *  |        1    |
   *  \           1 /  */
  Matrix4x4 scale;
  scale.el[0][0] = 1 / (umax - umin);
  scale.el[1][1] = 1 / (vmax - vmin);
  scale.el[2][2] = scale.el[3][3] = 1;

  Matrix4x4 mpar;
  mpar = scale * transl * spar;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

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

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Von hinten nach vorne. Hängt stark von der Blickrichtung ab.
  for (int z = 0; z < num; ++z)
    {
      for (int x = 0; x < num; ++x)
        {
          // Nur für die Färbung: bilde ab 0 ↦ 0 und num ↦ 1, jeweils für
          // x und z. Die Steigung ist dann 1/num.
          // Skalierung von [0; 1] auf [0; 255] erfolgt später.
          double xval = static_cast<double>(x) / num;
          double zval = static_cast<double>(z) / num;

          // Jedes Viereck besteht aus zwei Dreiecken.
          Dreieck d1, d2;

          // Berechne Koordinaten im Raum.
          // 0 ↦ xmin, num ↦ xmax, Steigung ist dann (xmax - xmin) / num
          // z analog.
          double xl = xmin + x * (xmax - xmin) / num;
          double xh = xmin + (x + 1) * (xmax - xmin) / num;
          double zl = zmin + z * (zmax - zmin) / num;
          double zh = zmin + (z + 1) * (zmax - zmin) / num;

          // Berechne Funktionswerte und erzeuge 3D-Punkte (4D-hom.) mit
          // Funktionswert als y-Koordinate.
          Vec4D p1(xl, func(xl, zl), zl, 1);
          Vec4D p2(xl, func(xl, zh), zh, 1);
          Vec4D p3(xh, func(xh, zl), zl, 1);
          Vec4D p4(xh, func(xh, zh), zh, 1);

          // Weise Punkte zu Dreiecken zu.   (Assign)
          d1.ecke[0] = p1;
          d1.ecke[1] = p2;
          d1.ecke[2] = p3;

          d2.ecke[0] = p2;
          d2.ecke[1] = p4;
          d2.ecke[2] = p3;

          // Farbe. Rot in x-Richtung, Grün in z-Richtung, Blau invers in
          // beide Richtungen.
          d1.col = DrawColour(255 * xval, 255 * zval,
                              255 * (1 - xval) * (1 - zval));
          d2.col = DrawColour(255 * xval, 255 * zval,
                              255 * (1 - xval) * (1 - zval));

          // In Liste einfügen.
          dreiecke.push_back(d1);
          dreiecke.push_back(d2);
        }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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

  // Seitenverhältnis des Fensters. Wenn das Fenster und
  // [umin; umax] × [vmin; vmax] dasselbe Verhältnis haben, kann einfach
  // skaliert werden. Falls nicht, muss angepasst werden.
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

  // Male [umin,umax]x[vmin,vmax]
//   pic.drawLine(0+shix,  0+shiy,  0+shix  ,hei+shiy);
//   pic.drawLine(0+shix,  0+shiy,  wid+shix,0+shiy  );
//   pic.drawLine(wid+shix,0+shiy,  wid+shix,hei+shiy);
//   pic.drawLine(0+shix,  hei+shiy,wid+shix,hei+shiy);

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
    //{ return 10 * exp(-(s * s / 25 + t * t / 100)); };
  // Alternative zum Testen:
  { return 5 + 5 * cos(s + t) * cos(s - t) * exp(-(s * s + t * t) / 12); };

  erzeugeFlaeche(xmin, xmax, zmin, zmax, num, func, dreiecke);

  double ratio;
  mpar = berechneMpar(umin, umax, vmin, vmax, ratio);
  maleDreiecke(pic, dreiecke, mpar, ratio);

  pic.savePNG("painters.png");

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
