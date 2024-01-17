#include <iostream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include <unistd.h>

#include <cppqt.h>
const DrawColour BLUE = DrawColour(0, 0, 255);
const DrawColour RED = DrawColour(255, 0, 0);

using namespace std;

string itos( int i )
{
  stringstream ss;
  ss << i;
  return ss.str();
}
using namespace std;

#include "hermite.h"

int maindraw()
{
  Drawing pic(600, 600);
  Drawing buff(600, 600);
  DPoint2D p1, p4, r1, r4;
  int b = 250;
  int c = 100;
  string strRho;

  pic.show();

  p1 = DPoint2D(b, b);
  p4 = DPoint2D(b+c, b+c);

  for (int rho = -800; rho <= 1000; rho += 10)
    {
      r1 = DPoint2D( rho, 0 );
      r4 = DPoint2D( 0, rho );

      buff = 255;

      maleHermiteKurve(buff, p1, p4, r1, r4, 50);
      strRho = itos( rho );
      buff.drawText( IPoint2D(0,0), strRho.c_str() );

      pic = buff;

      if( rho == c*6 )
        IOThread::sleep( 2 );

      IOThread::msleep(70);
    }

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}

