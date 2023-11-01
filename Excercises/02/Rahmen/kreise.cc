// Scan Conversion für Kreise
// Autor: Holger Arndt, Sebastian Birk
// Datum: 02.11.2015

#include <iostream>

#include <cppqt.h>

using namespace std;

void drawCirclePoints(Drawing& pic, int x, int y, IPoint2D center,
                      bool filled, int colour = 0)
{
  // HIER ERGÄNZEN
}

void drawCircle(Drawing& pic, IPoint2D center, int radius, bool filled,
                int colour = 0)
{
  // zeichnet einen Kreis um center mit Radius radius

  // HIER ERGÄNZEN
}

int maindraw()
{
  Drawing pic1(200, 200);
  Drawing pic2(200, 200);

  pic1.show();
  pic1.setZoom(2);
  pic2.show();
  pic2.setZoom(2);

  IPoint2D center;
  int radius;
  int colour;

  while (true)
    {
      cout << "Eingabe von center, radius, colour: ";
      cin >> center >> radius >> colour;
      if (center.x < 0 || center.y < 0)
        break;
      pic1.show();
      drawCircle(pic1, center, radius, false, colour);
      cin.get();
      cout << "Weiter mit Return" << endl;
      cin.get();
      pic2.show();
      drawCircle(pic2, center, radius, true, colour);
      cout << "Weiter mit Return" << endl;
      cin.get();

    }
    
  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
