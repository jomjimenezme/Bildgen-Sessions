#include <iostream>
#include <string>

#include <cppqt.h>

using namespace std;

/// Beispiel-Programm, das die wichtigstes Funktionen zum Zeichnen von Bildern
/// enthält.
int maindraw()
{
  cout << "Hallo" << endl;
  cin.get();

  Drawing pic1(400, 300);
  pic1.show();
  cin.get();

  pic1.setZoom(2);
  cin.get();

  IPoint2D p1;
  while(true)
    {
      cout << "Eingabe von p1: ";
      cin >> p1;
      if (p1.x < 0 || p1.y < 0)
         break;
      pic1.drawPoint(p1);
    }
  cin.get();

  for (int i = 0; i <= 299; i++)
    pic1.drawPoint(i, i, DrawColour(255,0,0));
  cin.get();

  pic1.setZoom(1);
  cin.get();

  for (int i = 0; i <= 99; i++)
    pic1.drawPoint(i, i+10, DrawColour(0,0,255), true);
  cin.get();

  pic1.drawLine(0, 12, 100, 212, DrawColour(255, 0, 255));
  cin.get();

  Drawing pic2(100, 500, DrawColour(0, 255, 0));
  pic1 = pic2;
  cin.get();

  string filename;
  cout << "Datei: ";
  cin >> filename;
  pic1.loadImage(filename);
  pic1.savePNG("pic1.png");
  cin.get();

  IOThread::waitForWindow(10);

  cout << "Tschüss" << endl;

  return 0;
}
