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
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  double cx[4], cy[4];
  DPoint2D anf, end;
  int m = p.size() - 1;
  double t;
  double ninv = 1.0 / n;
  int i, k;



  for (k = 3; k <= m; k += 3)
    {
      cx[0] = -p[k - 3].x + 3 * p[k - 2].x - 3 * p[k - 1].x + p[k].x;
      cx[1] = 3 * p[k - 3].x - 6 * p[k - 2].x + 3 * p[k - 1].x;
      cx[2] = -3 * p[k - 3].x + 3 * p[k - 2].x;
      cx[3] = p[k - 3].x;
      cy[0] = -p[k - 3].y + 3 * p[k - 2].y - 3 * p[k - 1].y + p[k].y;
      cy[1] = 3 * p[k - 3].y - 6 * p[k - 2].y + 3 * p[k - 1].y;
      cy[2] = -3 * p[k - 3].y + 3 * p[k - 2].y;
      cy[3] = p[k - 3].y;

      end.x = cx[3];
      end.y = cy[3];
      for (i = 1; i <= n; ++i)
        {
          t = ninv * i;
          anf = end;
          end.x = ((cx[0] * t + cx[1]) * t + cx[2]) * t + cx[3];
          end.y = ((cy[0] * t + cy[1]) * t + cy[2]) * t + cy[3];
          pic.drawLine(round(anf), round(end));
        }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void maleBSpline(Drawing& pic, const vector<DPoint2D>& p, int n)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  double cx[4], cy[4];
  DPoint2D anf, end;
  int m = p.size() - 1;
  double t;
  double ninv = 1.0 / n;
  int i, k;

  for (k = 3; k <= m; ++k)
    {
      cx[0] = (-p[k - 3].x + 3 * p[k - 2].x - 3 * p[k - 1].x + p[k].x) / 6.0;
      cx[1] = (3 * p[k - 3].x - 6 * p[k - 2].x + 3 * p[k - 1].x) / 6.0;
      cx[2] = (-3 * p[k - 3].x + 3 * p[k - 1].x) / 6.0;
      cx[3] = (p[k - 3].x + 4 * p[k - 2].x + p[k - 1].x) / 6.0;
      cy[0] = (-p[k - 3].y + 3 * p[k - 2].y - 3 * p[k - 1].y + p[k].y) / 6.0;
      cy[1] = (3 * p[k - 3].y - 6 * p[k - 2].y + 3 * p[k - 1].y) / 6.0;
      cy[2] = (-3 * p[k - 3].y + 3 * p[k - 1].y) / 6.0;
      cy[3] = (p[k - 3].y + 4 * p[k - 2].y + p[k - 1].y) / 6.0;

      end.x = cx[3];
      end.y = cy[3];
      for (i = 1; i <= n; ++i)
        {
          t = ninv * i;
          anf = end;
          end.x = ((cx[0] * t + cx[1]) * t + cx[2]) * t + cx[3];
          end.y = ((cy[0] * t + cy[1]) * t + cy[2]) * t + cy[3];
          pic.drawLine(round(anf), round(end), BLUE);
        }

      if (k != m)
        pic.drawCircle(end, 3, true, BLUE, BLUE);        // Knotenpunkt
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void maleCRSpline(Drawing& pic, const vector<DPoint2D>& p, int n)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  double cx[4], cy[4];
  DPoint2D anf, end;
  int m = p.size() - 1;
  double t;
  double ninv = 1.0 / n;
  int i, k;

  for (k = 3; k <= m; ++k)
    {
      cx[0] = (-p[k - 3].x + 3 * p[k - 2].x - 3 * p[k - 1].x + p[k].x) / 2.0;
      cx[1] = (2 * p[k - 3].x - 5 * p[k - 2].x + 4 * p[k - 1].x - p[k].x) / 2.0;
      cx[2] = (-p[k - 3].x + p[k - 1].x) / 2.0;
      cx[3] = p[k - 2].x;
      cy[0] = (-p[k - 3].y + 3 * p[k - 2].y - 3 * p[k - 1].y + p[k].y) / 2.0;
      cy[1] = (2 * p[k - 3].y - 5 * p[k - 2].y + 4 * p[k - 1].y - p[k].y) / 2.0;
      cy[2] = (-p[k - 3].y + p[k - 1].y) / 2.0;
      cy[3] = p[k - 2].y;

      end.x = cx[3];
      end.y = cy[3];
      for (i = 1; i <= n; ++i)
        {
          t = ninv * i;
          anf = end;
          end.x = ((cx[0] * t + cx[1]) * t + cx[2]) * t + cx[3];
          end.y = ((cy[0] * t + cy[1]) * t + cy[2]) * t + cy[3];
          pic.drawLine(round(anf), round(end), GREEN);
        }
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
  maleBSpline(pic, p, 50);
  maleCRSpline(pic, p, 50);

  IOThread::waitForWindow(60);

  return 0;
}
