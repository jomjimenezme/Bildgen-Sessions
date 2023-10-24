// Datei:       hermite.cpp
// Autor:       Holger Arndt
// Datum:       12.12.2006
// Modifiziert: Sebastian Birk, 09.12.2008

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <cppqt.h>

const DrawColour BLUE = DrawColour(0, 0, 255);
const DrawColour RED = DrawColour(255, 0, 0);

using namespace std;

#include "hermite.h"

int maindraw()
{
  Drawing pic(600, 600);
  DPoint2D p1, p4, r1, r4;
  int n;

  pic.show();
  cout << "Punkte p1, p4 und Tangenten r1, r4; Anzahl n der Strecken: ";
  cin >> p1 >> p4 >> r1 >> r4 >> n;

  maleHermiteKurve(pic, p1, p4, r1, r4, n, true);

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
