#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

#include <cppqt.h>

using namespace std;

// 64-bit Mersenne Twister: Pseudo-Zufallszahlengenerator, basierend auf
// der Mersenne-Primzahl 2^19937-1.
mt19937_64 rnd(time(nullptr));

const int anzahlBilder = 500;
const int neuProBild = 500;
const double g = 0.003;       // Gravitation

const DrawColour farbverlauf[] = { DrawColour(255, 255, 255), // weiß
                                   DrawColour(255, 255, 0),   // gelb
                                   DrawColour(255, 05, 0),    // rot
                                   DrawColour(0, 0, 0) };     // schwarz

// Zufallszahl in [0,1]
double rnd01()
{
  static double rndmax = rnd.max();
  return rnd() / rndmax;
}

class Partikel
{
private:
  DPoint2D position;
  DPoint2D geschwindigkeit;
  double gluehzustand; // Alter des Partikels: 0 bis 4, Start bei 0.8 (weiße Phase soll kürzer sein)
public:
  Partikel(DPoint2D startpos);
  void weiter();
  bool verschwunden() const { return gluehzustand >= 4; }
  DrawColour farbe() const
  { return farbverlauf[static_cast<int>(gluehzustand)]; }
  // Partikel als einfache Punkte
  void maleMich(Drawing& pic) const { pic.drawPoint(position, farbe()); }
};

Partikel::Partikel(DPoint2D startpos)
{
  // x variiert in [-20,20]
  position = startpos + DPoint2D(40 * rnd01() - 20, 0);
  // x variiert in [-1,1], y variiert in [1.5,3.5]
  geschwindigkeit
    = DPoint2D(0, 2.5) + DPoint2D(2 * rnd01() - 1, 2 * rnd01() - 1);
  gluehzustand = 0.6;
}

void Partikel::weiter()
{
  // dx, dy variieren in [-0.3,0.3]
  geschwindigkeit
    = geschwindigkeit + 0.6 * DPoint2D(rnd01() - 0.5, rnd01() - 0.5);
  // Gravitation: y reduzieren um 0.0015
  position = position + geschwindigkeit - 0.5 * DPoint2D(0, g);
  // Gravitation: y reduzieren um 0.003
  geschwindigkeit = geschwindigkeit - DPoint2D(0, g);
  // Leuchtkraft reduzieren
  gluehzustand += 0.06 * rnd01();// * position.y;
}

int maindraw()
{
  vector<Drawing> pics(anzahlBilder);
  vector<Partikel> p;

  Drawing pic(600, 600, 0);
  pic.show();
  for (int i = 0; i < anzahlBilder; ++i)
    {
      pic = DrawColour(90, 180, 90); // grün
      // neue Partikel
      for (int j = 0; j < neuProBild; ++j)
        p.push_back(Partikel(DPoint2D(300, 10)));
      // Partikel bewegen
      for (auto& pp : p)
        pp.weiter();
      // verschwundene Partikel löschen
      p.erase(remove_if(p.begin(), p.end(),
                        [] (Partikel& p) { return p.verschwunden(); }),
              p.end());
      // Partikel malen
      for (auto pit = p.rbegin(); pit != p.rend(); ++pit)
        pit->maleMich(pic);
      // Bild fertig
      pics[i] = pic;
      cout << "Bild " << i << ": " << p.size() << " Partikel" << endl;
    }

  Drawing::makeanim(pics, "feuer", "gif", 4);
  //Drawing::makeanim(pics, "feuer", "mpg");

  return 0;
}
