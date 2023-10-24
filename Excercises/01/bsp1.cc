#include <iostream>
#include <string>

#include <cppqt.h>

using namespace std;

/// Beispiel-Programm, das die wichtigstes Funktionen zum Zeichnen von Bildern
/// enthält.
int maindraw()
{

  Drawing pic1(400, 300);
  pic1.show();

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
