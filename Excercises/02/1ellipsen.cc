#include <iostream>

#include <cppqt.h>

using namespace std;

//######################################################################
// malt vier Punkte oder zwei Linien // (Wie Kreis)
void drawEllipsePoints(Drawing& pic, int x, int y, int xcenter, int ycenter,
                       bool filled, int colour = 0)
{
    int xpx = xcenter + x;
    int xmx = xcenter - x;
    int ypy = ycenter + y;
    int ymy = ycenter - y;

    if (filled)
    {
        //Bemale alle betroffenen Pixel von rechts nach links
        for (int i = xpx; i >= xmx; --i)
        {
            pic.drawPoint(i, ypy, colour, false);
            pic.drawPoint(i, ymy, colour, false);
        }
    }
    else
    {
        pic.drawPoint(xpx, ypy, colour, false);
        pic.drawPoint(xpx, ymy, colour, false);
        pic.drawPoint(xmx, ypy, colour, false);
        pic.drawPoint(xmx, ymy, colour, false);
    }
}

//######################################################################
// Scan Conversion für Ellipse
void drawEllipse(Drawing& pic, IPoint2D center, int a, int b, bool filled,
                 int colour = 0)
{
    int bQuadrat = b * b;
    int aQuadrat = a * a;

    int x = 0;
    int y = b;                                             //Startpunkt (0,b)
    int BDurchAQuadrat = 4 * bQuadrat;                     //(b/a)^2 skaliert mit 4a^2
    int D1 = aQuadrat * (4 * b - 1) - BDurchAQuadrat;      //d1(0) * 4a^2

    while (aQuadrat * y >= bQuadrat * x)                   //solange wir noch in Region1 sind
    {
        drawEllipsePoints(pic, x, y, center.x, center.y, filled, colour);

        ++x;

        if (D1 >= 0)
        {
            D1 -= BDurchAQuadrat * (2 * x + 3);     //O
        }
        else
        {
            --y;
            D1 -= 4 * (bQuadrat * (2 * x + 3) - 2 * aQuadrat * y);     //SO
        }
    }


    //Wir tauschen die Rollen von x und y, und a und b, spiegeln die Funktion also an der Geraden y = x
    //Dadurch erhalten wir eine Funktion an der wir das gleiche (bis auf Vertauschung der Variablen) Verfahren wie oben anwenden koennen
    x = a;
    y = 0;                                                  //Startpunkt (a,0)
    int ADurchBQuadrat = 4 * aQuadrat;                      //(a/b)^2 skaliert mit 4b^2
    int D2 = bQuadrat * (4 * a - 1) - ADurchBQuadrat;       //d2(0) * 4b^2

    while (aQuadrat * y <= bQuadrat * x)                    //solange wir noch in Region2 sind
    {
        drawEllipsePoints(pic, x, y, center.x, center.y, filled, colour);

        ++y;

        if (D2 >= 0)
        {
            D2 -= ADurchBQuadrat * (2 * y + 3);     //S
        }
        else
        {
            --x;
            D2 -= 4 * (aQuadrat * (2 * y + 3) - 2 * bQuadrat * x);     //SO
        }
    }
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
      drawEllipse(pic1, center, a, b, true, colour);
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
