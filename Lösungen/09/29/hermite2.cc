#include <iostream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <vector>

#include <cppqt.h>

const DrawColour BLUE = DrawColour(0, 0, 255);
const DrawColour RED = DrawColour(255, 0, 0);

using namespace std;

#include "hermite.h"

int maindraw()
{
  Drawing pic(600, 600);
  DPoint2D p1, p4, r1, r4;
  int i;

  pic.show();
  int r = 175;
  p1 = DPoint2D(200, 300);
  p4 = DPoint2D(400, 300);
  r1 = DPoint2D(r, 0);

  for (i = 0; i <= 100; i++)
    {
      r4 = DPoint2D(r * cos(2.0 * M_PI * i / 100.0),
                    r * sin(2.0 * M_PI * i / 100.0));
      pic = 255;
      maleHermiteKurve(pic, p1, p4, r1, r4, 50);
      IOThread::msleep(100);
    }

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
