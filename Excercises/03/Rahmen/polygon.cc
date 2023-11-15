#include <iostream>
#include <vector>

#include <cppqt.h>

using namespace std;

struct Kante
{
  int ymax; // maximale y-Koordinate
  double x; // AKTUELLE x-Koordinate
  double einsdurchm; // Da die Kanten in y-Richtung ausgewertet werden,
                     // wird die inverse Steigung benötigt.
  /*
   * Sortierung in horizontaler Anordnung; es gibt keine sich 
   * überschneidenden Kanten, deshalb liegt eine Kante „links neben“
   * einer weiteren Kante, wenn
   *  • sie am (zur Zeit) untersten Punkt links neben der zweiten Kante 
   *    liegt, oder
   *  • beide Kanten zwar an der selben x-Koordinate beginnen, die Erste
   *    aber eine steilere Steigung hat.
   * 
   * Die oberen Punkte spielen dabei keine Rolle (es gibt in dieser 
   * Variante keine Liste für Kanten, die an einer bestimmten 
   * y-Koordinate inaktiv werden).
   * Die zweite Bedingung ist nötig, um neu hinzukommende Kanten einfach
   * einsortieren zu können, ohne die alten Kanten neu sortieren zu 
   * müssen.
   */
  friend bool operator<(const Kante& k1, const Kante& k2)
  {
    if (k1.x != k2.x)
      return k1.x < k2.x;
    else
      return k1.einsdurchm < k2.einsdurchm;
  }
};



void drawFilledPolygon(Drawing& pic, const vector<IPoint2D>& ecken,
                       int colour = 0)
{


































  /*
   * Erzeuge die Kanten aus der Punkteliste. Wichtig
   * 1. sortieren wir die beiden Punkte einer Kante nach ihrer y-Koordinate.
   * 2. Die aktuelle x-Koordinate ist dann zunächst die x-Koordinate des unteren Punktes.
   * 3. Wir fügen die Kante entsprechend ihrer y-Koordinate in die Liste ein.
   */





























  /*
   * Das Polygon wird geschlossen, indem der letzte und der erste Punkt
   * verbunden werden.
   */
  if (ecken[n - 1].y != ecken[0].y)
    {// wir machen das gleiche wie vorher. aber für die erste und letzte Ecke
      if (ecken[n - 1].y < ecken[0].y)
        {
          k.ymax = ecken[0].y;
          k.x = ecken[n - 1].x;
          kymin = ecken[n - 1].y;
        }
      else
        {
          k.ymax = ecken[n - 1].y;
          k.x = ecken[0].x;
          kymin = ecken[0].y;
        }
      k.einsdurchm = static_cast<double>(ecken[0].x - ecken[n - 1].x) /
        (ecken[0].y - ecken[n - 1].y);
      et[kymin].push_back(k);
    }


}

int maindraw()
{
  Drawing pic1(300, 300);
  pic1.setSleepTime(3);

  pic1.show();
  pic1.setZoom(2);

  int n;
  IPoint2D p;
  vector<IPoint2D> ecken;
  // zum STL-Typ vector vergleiche z.B.:
  //http://en.cppreference.com/w/cpp/container/vector
  //http://www.cplusplus.com/reference/vector/vector/
  //http://www.sgi.com/tech/stl/Vector.html

  cout << "Anzahl der Ecken: ";
  cin >> n;

  for (int i = 1; i <= n; ++i)
    {
      cout << "Ecke " << i << ": ";
      cin >> p;
      ecken.push_back(p);
    }

  cout << "Eingegebenes Polygon: ";
  for (unsigned int i = 0; i < ecken.size(); ++i)
    cout << ecken[i] << '-';
  cout << ecken[0] << endl;

  drawFilledPolygon(pic1, ecken, 0);

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}
