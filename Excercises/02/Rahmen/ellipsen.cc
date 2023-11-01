// Scan Conversion für Ellipsen
// Autor:  Sebastian Birk, Holger Arndt
// Datum:  2015-11-09

#include <iostream>

#include <cppqt.h>
 
using namespace std;

//######################################################################
// malt vier Punkte oder zwei Linien
void drawEllipsePoints(Drawing& pic, int x, int y, int xcenter, int ycenter,
                       bool filled, int colour = 0)
{
  // HIER ERGÄNZEN
}

//######################################################################
// Scan Conversion für Ellipse
void drawEllipse(Drawing& pic, IPoint2D center, int a, int b, bool filled,
                 int colour = 0)
{
  // HIER ERGÄNZEN
}

//######################################################################
int maindraw()
{
  Drawing pic1(200, 200);
  Drawing pic2(pic1);

  pic1.show();
  pic1.setZoom(2);
  pic2.show();
  pic2.setZoom(2);

  IPoint2D center;
  int a, b;
  int colour;

  while (true)
    {
      cout << "Eingabe von center, a, b, colour: ";
      cin >> center >> a >> b >> colour;
      if (center.x < 0 || center.y < 0)
        break;
      pic1.show();
      drawEllipse(pic1, center, a, b, false, colour);
      cin.get();
      cout << "Weiter mit Return" << endl;
      cin.get();
      pic2.show();
      drawEllipse(pic2, center, a, b, true, colour);
      cout << "Weiter mit Return" << endl;
      cin.get();
    }

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
