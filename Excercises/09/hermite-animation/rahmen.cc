// Datei:       hermite.cpp
// Autor:       Holger Arndt, Sebastian Birk
// Datum:       12.12.2006

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cppqt.h>

const DrawColour BLUE = DrawColour(0, 0, 255);
const DrawColour RED = DrawColour(255, 0, 0);

using namespace std;

void maleHermiteKurve( Drawing& pic, DPoint2D p1, DPoint2D p4, DPoint2D r1,
                       DPoint2D r4, int n )
{
  pic.drawCircle(p1, 3, true, BLUE, BLUE, true);
  pic.drawCircle(p4, 3, true, BLUE, BLUE, true);
  pic.drawLine(p1, p1 + r1, RED, true);
  pic.drawLine(p4, p4 + r4, RED, true);

  // HIER ERGAENZEN
}

int maindraw()
{
  Drawing pic(600, 600);
  DPoint2D p1, p4, r1, r4;
  int n;

  pic.show();
  cout << "Punkte p1, p4 und Tangenten r1, r4; Anzahl n der Strecken: ";
  cin >> p1 >> p4 >> r1 >> r4 >> n;

  maleHermiteKurve(pic, p1, p4, r1, r4, n);

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
