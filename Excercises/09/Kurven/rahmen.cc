// Bézier-Kurven, B-Splines, CR-Splines
// Autor: Holger Arndt
// Datum: 25.01.2016

#include <cstdlib>
#include <iostream>
#include <vector>

#include <cppqt.h>

const DrawColour BLUE = DrawColour(0, 0, 255);
const DrawColour GREEN = DrawColour(0, 255, 0);
const DrawColour RED = DrawColour(255, 0, 0);

using namespace std;

void maleBezierKurve(Drawing& pic, const vector<DPoint2D>& p, int n)
{
  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void maleBSpline(Drawing& pic, const vector<DPoint2D>& p, int n)
{
  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void maleCRSpline(Drawing& pic, const vector<DPoint2D>& p, int n)
{
  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

int maindraw()
{
  Drawing pic(650, 650);

  int nump;
  cin >> nump;

  vector<DPoint2D> p(nump);
  int i;
  for (i = 0; i < nump; ++i)
    cin >> p[i];

  pic.show();

  // Kontrollpunkte
  for (i = 0; i < nump; i++)
    pic.drawCircle(p[i], 3, true, RED, RED);

  maleBezierKurve(pic, p, 50);
  //maleBSpline(pic, p, 50);
  //maleCRSpline(pic, p, 50);

  IOThread::waitForWindow(60);

  return 0;
}
