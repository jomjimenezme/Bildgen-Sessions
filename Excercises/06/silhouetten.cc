

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

// ##############################
// Berechnung der Parallelprojektions-Matrix

Matrix4x4 berechneMpar(double umin, double umax, double vmin, double vmax,
                       double& ratio)
{
 
  Matrix4x4 spar;
  spar.el[0][0] = spar.el[1][1] = spar.el[3][3] = 1;
  spar.el[0][2] = -sqrt(3) / 4.0;
  spar.el[1][2] =     -1.0 / 4.0;

  
  Matrix4x4 transl;
  transl.el[0][0] = transl.el[1][1] = transl.el[2][2] = transl.el[3][3] = 1;
  transl.el[0][3] = -umin;
  transl.el[1][3] = -vmin;

  
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
// Kurven berechnen

void erzeugeKurven(double xmin, double xmax, double zmin, double zmax,
                   int num, int pieces,
                   const std::function<double(double,double)>& func,
                   std::vector<std::vector<Vec3D>>& kurven )
{
  // Ausgabe:
  //   Der vector kurven enthält die num darzustellenden Kurven des
  //   Silhouetten-Plots.
  //   Jede der num Kurven ist als ein std::vector gespeichert, der pieces
  //   Punkte (x, func(x, z), z) als Vec3D enthält.
  //   wichtig: Die Kurven müssen von vorne nach hinten gezeichnet werden, weil
  //            die hinter der bereits gezeichneten Kontur liegenden Kurven
  //            nicht dargestellt werden.
  //
  // Eingabe:
  //   xmin, xmax, zmin, zmax - Ausdehnung des Parameterrechtecks
  //   num                    - Anzahl der Kurven
  //   pieces                 - Anzahl der Strecken aus denen jede Kurve
  //                            zusammengesetzt wird
  //   func                   - auszuwertende Funktion: y = func(x, z)

  kurven.clear();
  kurven.resize(num);

  // Erzeuge Kurven als Menge von Punkten in x-Richtung; fester Abstand
  // der Kurven zueinender in z-Richtung.
  for (int z = 0; z < num; ++z)
    {
      // Speichere rückwärts. Mit zunehmendem z bewegen wir uns von
      // „hinten“ nach „vorne“.
      kurven[num - z - 1].resize(pieces + 1);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

      // z-Koordinate des Punktes.
      double posz = zmin + (zmax - zmin) * static_cast<double>(z) / num;

      // Einzelne Kurve, konstantes z, verbundene Punkte in x.
      for (int x = 0; x < pieces + 1; ++x)
        {
          // x-Koordinate des Punktes.
          double posx = xmin + (xmax - xmin) * static_cast<double>(x) / pieces;

          // Speichere rückwärts, s.o.
          kurven[num - z - 1][x] = Vec3D(posx, func(posx, posz), posz);
        }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    }
}

// ##############################
// Zeichnen

void maleSilhouetten(Drawing& pic,
                     const std::vector<std::vector<Vec3D>>& kurven,
                     const Matrix4x4& mpar, double ratio)
{
  // malt die Kurven ins Bild pic

  // Für jede Pixelspalte, merke die Ausdehnung der bisher gezeichneten
  // Kurven.
  std::vector<double> konturmin(pic.getWidth() + 1, pic.getHeight() + 1);
  std::vector<double> konturmax(pic.getWidth() + 1, -1);

  // Anpassung an das Seitenverhältnis für verzerrungsfreies Bild
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

  // Hilfsfunktion für die Projektion
  auto proj = [&](const Vec3D& v) -> DPoint2D
    {
      Vec4D phom = mpar * v;
      DPoint2D pd = homTo2D(phom);
      pd.x = pd.x * wid + shix;
      pd.y = pd.y * hei + shiy;
      return pd;
    };

  // Male [umin; umax] × [vmin; vmax]
//   pic.drawLine(0+shix,  0+shiy,  0+shix  ,hei+shiy);
//   pic.drawLine(0+shix,  0+shiy,  wid+shix,0+shiy  );
//   pic.drawLine(wid+shix,0+shiy,  wid+shix,hei+shiy);
//   pic.drawLine(0+shix,  hei+shiy,wid+shix,hei+shiy);

  // Schleife über die Kurven
  IPoint2D pl, pr;
  double schnitt;

  // Zeichne Kurve als Sequenz von Geradenstücken.
  for (unsigned int i = 0; i < kurven.size(); ++i)
    {
      // ersten Punkt des Streckenzugs berechnen (linker Punkt der Geraden)
      pl = proj(kurven[i][0]);

      // Über die restlichen Punkte laufen
      for (unsigned int j = 1; j < kurven[i].size(); ++j)
        {
          // führe Parallelprojektion aus für nächsten Punkt des Streckenzugs
          // Rechter Punkt der Geraden (linker Punkt ist der vorherige Punkt)
          pr = proj(kurven[i][j]);

          // in x-Richtung über alle Pixel zwischen den Punkten laufen
          double yl = pl.y;
          for (int x = pl.x; x < pr.x; ++x)
            {
              // Pixel außerhalb des Bildes ignorieren (um nicht im Vektor
              // aus den Grenzen zu laufen)
              if (x < 0 || x >= pic.getWidth())
                continue;

              // Geradengleichung. Steigung ist dy/dx. Gerade ist verschoben
              // um pl.x mit Achsenabschnitt pl.y.
              // y-Koordinate am nächsten Punkt.
              double yr = pl.y + (x + 1 - pl.x)
                * static_cast<double>(pr.y - pl.y) / (pr.x - pl.x);

              // Fälle unterscheiden (siehe S. 6-44)
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
              if ((konturmin[x] >= yl && konturmin[x+1] >= yr)
                  || (konturmax[x] <= yl && konturmax[x+1] <= yr))
                {
                  // Fall 1: beide auf selber Seite der Kontur
                  pic.drawLine(x, round(yl), x + 1, round(yr));
                  konturmin[x] = min(konturmin[x], yl);
                  konturmin[x + 1] = min(konturmin[x + 1], yr);
                  konturmax[x] = max(konturmax[x], yl);
                  konturmax[x + 1] = max(konturmax[x + 1], yr);
                }
              if (yl < konturmin[x])
                {
                  // Fall 2/3-1: alt (x, yl) unterhalb Kontur
                  // Gerade 1 gegeben durch (x, yl), (x+1, yr)
                  // Gerade 2 gegeben durch (x, kmin(x)), (x+1, kmin(x+1))
                  // Steigungen entsprechend m1 = yr - yl
                  //                         m2 = kmin(x+1) - kmin(x)
                  // Verlege (x, yl) in den Ursprung, dann ist
                  //                         n1 = 0
                  //                         n2 = kmin(x) - yl
                  // Verschobene x-Koordinate des Schnittpunktes:
                  // (yr - yl)*x = (kmin(x+1) - kmin(x))*x + (kmin(x) - yl)
                  // (yr - yl - kmin(x+1) + kmin(x))*x = kmin(x) - yl
                  // x = (kmin(x) - yl) / (yr - yl - kmin(x+1) + kmin(x))
                  schnitt = static_cast<double>(konturmin[x] - yl)
                    / (yr - yl - konturmin[x + 1] + konturmin[x]);
                  // Nur zeichnen, wenn das Geradenstück von x nach x+1
                  // wirklich geschnitten wird.
                  if (schnitt >= 0 && schnitt <= 1)
                    pic.drawLine(x, round(yl), round(x + schnitt),
                                 round(yl + schnitt * (yr - yl)));
                  konturmin[x] = yl;
                }
              if (yl > konturmax[x])
                {
                  // Fall 2/3-2: alt (x, yl) oberhalb Kontur
                  schnitt = static_cast<double>(konturmax[x] - yl)
                    / (yr - yl - konturmax[x + 1] + konturmax[x]);
                  // Nur zeichnen, wenn das Geradenstück von x nach x+1
                  // wirklich geschnitten wird.
                  if (schnitt >= 0 && schnitt <= 1)
                    pic.drawLine(x, round(yl), round(x + schnitt),
                                 round(yl + schnitt * (yr - yl)));
                  konturmax[x] = yl;
                }
              if (yr < konturmin[x + 1])
                {
                  // Fall 2/3-3: neu (x + 1, yl) unterhalb Kontur
                  schnitt = static_cast<double>(konturmin[x] - yl)
                    / (yr - yl - konturmin[x + 1] + konturmin[x]);
                  // Nur zeichnen, wenn das Geradenstück von x nach x+1
                  // wirklich geschnitten wird.
                  if (schnitt >= 0 && schnitt <= 1)
                    pic.drawLine(round(x + schnitt),
                                 round(yl + schnitt * (yr - yl)), x + 1,
                                 round(yr));
                  konturmin[x + 1] = yr;
                }
              if (yr > konturmax[x + 1])
                {
                  // Fall 2/3-4: neu (x + 1, yl) oberhalb Kontur
                  schnitt = static_cast<double>(konturmax[x] - yl)
                    / (yr - yl - konturmax[x + 1] + konturmax[x]);
                  // Nur zeichnen, wenn das Geradenstück von x nach x+1
                  // wirklich geschnitten wird.
                  if (schnitt >= 0 && schnitt <= 1)
                    pic.drawLine(round(x + schnitt),
                                 round(yl + schnitt * (yr - yl)), x + 1,
                                 round(yr));
                  konturmax[x + 1] = yr;
                }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
              // Fall 4: beide innerhalb der Kontur
              // nichts tun

              yl = yr;
            }

          // letzten Punkt des Streckenzugs merken
          pl = pr;
          IOThread::msleep(5);
        }
    }
}

int maindraw()
{
  Drawing pic(1200, 800, DrawColour(255, 255, 255));
  pic.show();

  std::vector<std::vector<Vec3D>> kurven;  // die zu zeichnenden Kurven
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
  int num = 20;
  int pieces = 40;

  auto func = [](double s, double t)
  { return 10 * exp(-(s * s / 25 + t * t / 100)); };
  // Alternative zum Testen:
  //{ return 5 + 5 * cos(s + t) * cos(s - t) * exp(-(s * s + t * t) / 12); };

  erzeugeKurven(xmin, xmax, zmin, zmax, num, pieces, func, kurven);

  double ratio;
  mpar = berechneMpar(umin, umax, vmin, vmax, ratio);
  maleSilhouetten(pic, kurven, mpar, ratio);

  pic.savePNG("silhouetten.png");

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
