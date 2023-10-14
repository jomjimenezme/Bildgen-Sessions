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
          // Linie WSW–ONO            // schnelle Richtung: x, y positiv
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
          // Linie ONO–WSW           // schnelle Richtung: -x, y negativ
          int mm = -2 * dy;                      // Vorzeichen getauscht
          int halb = -dx;                        // Vorzeichen getauscht
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (--x; x >= p2.x; --x)              // Vorzeichen getauscht
            {
              D += mm;
              if (D >= halb) // SW
                {
                  --y;                           // Vorzeichen getauscht
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (m < 0 && m >= -1 && p1.x < p2.x)
        {
          // Linie WNW–OSO            // schnelle Richtung: x, y negativ
          int mm = 2 * dy;
          int halb = dx;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (++x; x <= p2.x; ++x)
            {
              D -= mm;                           // Vorzeichen getauscht
              if (D > halb) // SO
                {
                  --y;                           // Vorzeichen getauscht
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (m < 0 && m >= -1 && p1.x > p2.x)
        {
          // Linie OSO–WNW           // schnelle Richtung: -x, y positiv
          int mm = -2 * dy;                      // Vorzeichen getauscht
          int halb = -dx;                        // Vorzeichen getauscht
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (--x; x >= p2.x; --x)              // Vorzeichen getauscht
            {
              D -= mm;                           // Vorzeichen getauscht
              if (D >= halb) // NW
                {
                  ++y;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      else if (m > 1 && p1.y < p2.y)
        {
          // Linie SSW–NNO            // schnelle Richtung: y, x positiv
          int mm = 2 * dx;                                    // x <-> y
          int halb = dy;                                      // x <-> y
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (++y; y <= p2.y; ++y)                           // x <-> y
            {
              D += mm;
              if (D > halb) // NO
                {
                  ++x;                                        // x <-> y
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (m > 1 && p1.y > p2.y)
        {
          // Linie NNO–SSW           // schnelle Richtung: -y, x negativ
          int mm = -2 * dx;
          int halb = -dy;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (--y; y >= p2.y; --y)
            {
              D += mm;
              if (D >= halb) // SW
                {
                  --x;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else if (p1.y < p2.y) // && m < -1
        {
          // Linie SSO–NNW            // schnelle Richtung: y, x negativ
          int mm = 2 * dx;
          int halb = dy;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (++y; y <= p2.y; ++y)
            {
              D -= mm;
              if (D > halb) // NW
                {
                  --x;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
      else
        {
          // Linie NNW–SSO           // schnelle Richtung: -y, x positiv
          int mm = -2 * dx;
          int halb = -dy;
          int eins = 2 * halb;
          int D = 0;
          pic.drawPoint(x, y, colour, true);
          for (--y; y >= p2.y; --y)
            {
              D -= mm;
              if ( D >= halb ) // SO
                {
                  ++x;
                  D -= eins;
                }
              pic.drawPoint(x, y, colour, true);
            }
        }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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

/*
(  0,100) (199,100) 127
(100,  0) (100,199) 200 
( 29, 29) (171,171) 50 
( 29,171) (171, 29) 25 */
