#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

#include <cppqt.h>

using namespace std;

struct Kante
{
  int ymax; // maximale y-Koordinate
  double x; // AKTUELLE x-Koordinate
  double einsdurchm; // Da die Kanten in y-Richtung ausgewertet werden,
                     // wird die inverse Steigung benötigt.
  
  friend bool operator<(const Kante& k1, const Kante& k2)
  {
    if (k1.x != k2.x)
      return k1.x < k2.x;
    else
      return k1.einsdurchm < k2.einsdurchm;
  }
};

typedef list<Kante> KantenTabelle;

void drawFilledPolygon(Drawing& pic, const vector<IPoint2D>& ecken,
                       int colour = 0)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  int ymin = pic.getHeight() + 1;
  int ymax = -1;
  unsigned int n = ecken.size();
  cout<<ymin<<"  "<<ymax<<"  "<<endl;

  // bestimme kleinste und größte Bildzeile
  for (unsigned int i = 0; i < n; i++)
     {
       if (ecken[i].y < ymin)
         ymin = ecken[i].y;
       if (ecken[i].y > ymax)
         ymax = ecken[i].y;
     }

  cout<<ymin<<"  "<<ymax<<"  "<<endl;

  KantenTabelle aet(0);
  
  
  vector<KantenTabelle> et(ymax + 1);             // et(0, …, ymin) bleiben leer
  // Iteratoren
  KantenTabelle::iterator kitaet;        // Kanteniterator für Active-Edge-Table
  KantenTabelle::iterator kitety;      // Kanteniterator für Edge-Table, Zeile y
  Kante k;
  int kymin;
  double xanf, xend;

  /*
   * Erzeuge die Kanten 
   */
  for (unsigned int i = 0; i < n - 1; ++i)
    {
      if (ecken[i].y != ecken[i + 1].y)
        {
          if (ecken[i].y < ecken[i + 1].y)
            {
              k.ymax = ecken[i + 1].y;
              k.x = ecken[i].x;
              kymin = ecken[i].y;
            }
          else
            {
              k.ymax = ecken[i].y;
              k.x = ecken[i + 1].x;
              kymin = ecken[i + 1].y;
            }
          k.einsdurchm = static_cast<double>(ecken[i + 1].x - ecken[i].x) /
            (ecken[i + 1].y - ecken[i].y);
          et[kymin].push_back(k); //Füge die Kante in die zu ihrer y-Koordinate gehörenden Liste ein.
        }
    }
  /*
   * Das Polygon wird geschlossen, indem der letzte und der erste Punkt
   * verbunden werden.
   */
  if (ecken[n - 1].y != ecken[0].y)
    {
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

  /*
   * Sortiere die Kanten in et[y] horizontal (siehe struct Kante).
   * So lassen sich die Kanten einfach sortiert in die AET einfügen.
   */
  for (int y = ymin; y <= ymax; ++y)
    et[y].sort();
      

  /*
   * Hauptschleife über die y-Zeilen des Polygons
   */
  for (int y = ymin; y <= ymax; ++y)
    {
      // füge alle in Zeile y startenden Kanten sortiert ein
      kitety = et[y].begin();
      kitaet = aet.begin();
      while (kitety != et[y].end() && kitaet != aet.end())
        {
          if (*kitaet < *kitety)
            ++kitaet;
          else
            {
              aet.insert(kitaet, *kitety);
              ++kitety;
            }
        }
      /*
       * Am Ende sind u.U. noch Kanten in et[y] übrig. Aufgrund der
       * Sortierung können sie hier einfach angehängt werden.
       */
      while (kitety != et[y].end())
        aet.push_back(*kitety++);

      /*
       * Entferne alle bei y endenden Kanten aus der AET.
       */
      for (kitaet = aet.begin(); kitaet != aet.end(); )
        if (kitaet->ymax == y)
          kitaet = aet.erase(kitaet);
        else
          ++kitaet;

      /*
       * Malen!
       */
      for (kitaet = aet.begin(); kitaet != aet.end(); )
        {
          xanf = (kitaet++)->x;
          xend = (kitaet++)->x;
          cout<<" Zeile y=  " <<y<<"    xanf und xend:     "<<xanf<<"    "<<xend<<endl;
          for (int x = static_cast<int>(round(xanf));
               x <=  static_cast<int>(round(xend)); ++x)
            pic.drawPoint(x, y, colour, false);
          cout<<"----------------------------------------"<<endl;
        }
      /*
       * Aktualisiere die x-Werte aller Kanten in AET.
       */
      for (kitaet = aet.begin(); kitaet != aet.end(); ++kitaet)
        kitaet->x += kitaet->einsdurchm;
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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

  pic1.savePNG("polygon.png");

  cout << endl;
  IOThread::waitForWindow(5);

  return 0;
}

/*
 4
 (0,100)
 (40,250)
 (200, 200)
 (50,0)
  */

