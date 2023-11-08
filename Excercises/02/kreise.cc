#include <iostream>

#include <cppqt.h>

void drawCirclePoints(Drawing& pic, int x, int y, IPoint2D center,
                      bool filled, int colour = 0)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // malt acht Punkte oder vier Linien
  int xcenter = center.x;
  int ycenter = center.y;
  if (!filled)
    {
      pic.drawPoint(-x + xcenter,  y + ycenter, colour, true);
      pic.drawPoint( x + xcenter,  y + ycenter, colour, true);
      pic.drawPoint(-x + xcenter, -y + ycenter, colour, true);
      pic.drawPoint( x + xcenter, -y + ycenter, colour, true);
      pic.drawPoint(-y + xcenter,  x + ycenter, colour, true);
      pic.drawPoint( y + xcenter,  x + ycenter, colour, true);
      pic.drawPoint(-y + xcenter, -x + ycenter, colour, true);
      pic.drawPoint( y + xcenter, -x + ycenter, colour, true);
    }
  else
    {
      int k;
      for (k = -x; k <= x; k++)
        {
          pic.drawPoint(k + xcenter, y + ycenter, colour, true);
          pic.drawPoint(k + xcenter, -y + ycenter, colour, true);
        }
      for (k = -y; k <= y; k++)
        {
          pic.drawPoint(k + xcenter, x + ycenter, colour, true);
          pic.drawPoint(k + xcenter, -x + ycenter, colour, true);
        }
      IOThread::msleep(40);
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void drawCircle(Drawing& pic, IPoint2D center, int radius, bool filled,
                int colour = 0)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // zeichnet einen Kreis um center mit Radius radius
  int x = 0;
  int y = radius;
  int d = 4 * radius - 5;

  while (y >= x)
    {
      drawCirclePoints(pic, x, y, center, filled, colour);
      ++x;
      if (d >= 0)
        d -= 8 * x + 4;
      else
        {
          --y;
          d -= 8 * (x - y) + 4;
        }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
using namespace std;
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


/*
(100,100) 25 100


(100,100) 15 200


(30,70) 5 0


(-1,-1)
*/

