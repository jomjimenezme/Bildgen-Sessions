#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include <cppqt.h>

using namespace std;

void drawAntialiasedWideLine(Drawing& pic, DPoint2D p1, DPoint2D p2,
                             double w, int f = 4, int colour = 0)
// zeichnet eine Linie von p1 nach p2 der Dicke w, der Algorithmus verwendet
// zur Berechnung ein f-fach in x- und y-Richtung verfeinertes Raster
{
  // HIER ERGÄNZEN






































  if (p1.y != p2.y)
    {
      // Bereich zwischen p11.y und p21.y
      einsdurchm = (p11.x - p21.x) / (p11.y - p21.y);
      x = p11.x + (ceil(p11.y) - p11.y) * einsdurchm;
      for (y = static_cast<int>(ceil(p11.y)); y <= floor(p21.y); ++y)
        {
          linkerrand[y] = min(linkerrand[y], static_cast<int>(round(x)));
          rechterrand[y] = max(rechterrand[y], static_cast<int>(round(x)));
          x += einsdurchm;
        }
      // Bereich zwischen p12.y und p22.y
      einsdurchm = (p12.x - p22.x) / (p12.y - p22.y);
      x = p12.x + (ceil(p12.y) - p12.y) * einsdurchm;
      for (y = static_cast<int>(ceil(p12.y)); y <= floor(p22.y); ++y)
        {
          linkerrand[y] = min(linkerrand[y], static_cast<int>(round(x)));
          rechterrand[y] = max(rechterrand[y], static_cast<int>(round(x)));
          x += einsdurchm;
        }
    }//endif
}

int maindraw()
{
  Drawing pic1(170, 170);

  pic1.show();

  DPoint2D p1, p2;
  double width;
  int f;

  cout << "Dicke, Raster: ";
  cin >> width >> f;

  for (int theta = 0; theta <= 90; theta += 9)
    {
      p1 = DPoint2D(10,10);
      p2 = p1 + 150.0 * DPoint2D(sin(theta * M_PI / 180.0),
                                 cos(theta * M_PI / 180.0));
      drawAntialiasedWideLine(pic1, p1, p2, width, f, 0);
    }

  cin.get();
  cout << "Return zum Vergrößern" << endl;
  cin.get();
  pic1.setZoom(4);

  IOThread::waitForWindow(60);

  return 0;
}
