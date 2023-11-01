// Scan Conversion für Linien
// Autor: Holger Arndt, Sebastian Birk
// Datum: 13.11.2015

#include <algorithm>
#include <iostream>

#include <cppqt.h>

using namespace std;

void drawLine(Drawing& pic, IPoint2D p1, IPoint2D p2, int colour = 0)
{
  // zeichnet eine Linie von p1 nach p2
  int dx = p2.x - p1.x;
  int dy = p2.y - p1.y;
  int x = p1.x;
  int y = p1.y;

  // Sonderfälle zuerst
  if (dy == 0)
    // Linie W–O oder O–W
    for (x = min(p1.x, p2.x); x <= max(p1.x, p2.x); ++x)
      pic.drawPoint(x, y, colour, true);
  else if (dx == 0)
    // Linie N–S oder S–N
    for (y = min(p1.y, p2.y); y <= max(p1.y, p2.y); ++y)
      pic.drawPoint(x, y, colour, true);
  else
    {
      double m = static_cast<double>(dy) / dx;

      if (m > 0 && m <= 1 && p1.x < p2.x)
        {
          // Linie WSW–ONO
          int mm = 2 * dy;
          int halb = dx;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (++x; x <= p2.x; ++x)
            {
              D += mm;
              if (D > halb) // NO
                {
                  ++y;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (m > 0 && m <= 1 && p1.x > p2.x)
        {
          // Linie ONO–WSW
          int mm = -2 * dy;
          int halb = -dx;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (--x; x >= p2.x; --x)
            {
              D += mm;
              if (D >= halb) // SW
                {
                  --y;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (m < 0 && m >= -1 && p1.x < p2.x)
        {
          // Linie WNW–OSO
          int mm = 2 * dy;
          int halb = dx;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (++x; x <= p2.x; ++x)
            {
              D -= mm;
              if (D > halb) // SO
                {
                  --y;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (m < 0 && m >= -1 && p1.x > p2.x)
        {
          // Linie OSO–WNW
          int mm = -2 * dy;
          int halb = -dx;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (--x; x >= p2.x; --x)
            {
              D -= mm;
              if (D >= halb) // NW
                {
                  ++y;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
    }
  // HIER ERGÄNZEN
  /*
    Hinweise:
     • Linie SSW–NNO
     • Linie NNO–SSW
     • Linie SSO–NNW
     • Linie NNW–SSO
  */
}

int maindraw()
{
  Drawing pic(200, 200);

  pic.show();
  pic.setZoom(3);

  IPoint2D p1, p2;
  int colour;

  while (true)
    {
      cout << "Eingabe von p1, p2, colour: ";
      cin >> p1 >> p2 >> colour;
      if (p1.x < 0 || p1.y < 0 || p2.x < 0 || p2.y < 0)
        break;
      drawLine(pic, p1, p2, colour);
    }

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
