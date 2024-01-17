#include <cmath>
#include <ctime>
#include <random>
#include <stack>
#include <string>

#include <cppqt.h>

using namespace std;

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

// 64-bit Mersenne Twister: Pseudo-Zufallszahlengenerator, basierend auf
// der Mersenne-Primzahl 2^19937-1.
mt19937_64 rnd(time(nullptr));

const int anzahlBilder = 30;

// Zufallszahl in [0,1]
double rnd01()
{
  static double rndmax = rnd.max();
  return rnd() / rndmax;
}

class Blume
{
private:
  DPoint2D position;
  string ableitung;
public:
  Blume(DPoint2D pos)
  {
    position = pos;
    ableitung = "zK";
  }
  void weiter()
  {
    string neu = "";
    double r;
    for (auto z : ableitung)
      switch (z)
        {
          case 'K':
            r = rnd01();
            if (r < 0.2)               // 20%
              neu += 'K';
            else if (r < 0.25)         // 5%
              break; // ε
            else if (r < 0.375)        // 12,5%
              neu += 'B';
            else if (r < 0.5)          // 12,5%
              neu += 'l';
            else if (r < 0.625)        // 12,5%
              neu += "zK";
            else if (r < 0.75)         // 12,5%
              neu += "(K)zK";
            else if (r < 0.875)        // 12,5%
              neu += "[K]zK";
            else                       // 12,5%
              neu += "(K)[K]zK";
            break;
          case 'B':
            r = rnd01();
            if (r < 0.5)
              neu += 'B';
            // ε für r ≥ 0.5
            break;
          default:
            neu += z;
        }
    ableitung = neu;
  }
  
  void maleMich(Drawing& pic) const
  {
    DPoint2D pos = position;
    DPoint2D posneu;
    double winkel(M_PI / 2);
    stack<DPoint2D> posstack;
    stack<double> winkelstack;
    int level = 1;
    
    for (auto z : ableitung)
      switch (z)
        {
          case 'K': // Knospe: kleiner blauer Kreis
            pic.drawCircle(pos, 3, true, DrawColour(95, 95, 255),
                           DrawColour(197, 197, 255));
            break;
          case 'B': // Blüte: drei rote Kreise, zwei klein, einer größer
            pic.drawCircle(pos + DPoint2D(-6, 0), 3, true,
                           DrawColour(251, 19, 20), DrawColour(254, 193, 193));
            pic.drawCircle(pos + DPoint2D(6, 0), 3, true,
                           DrawColour(251, 19, 20), DrawColour(254, 193, 193));
            pic.drawCircle(pos, 5, true,
                           DrawColour(251, 19, 20), DrawColour(254, 193, 193));
            break;
          case 'z': // Zweig: braune Linie, kürzer für tiefere Rekursionsebenen
            posneu
              = pos + 14 * exp(1 / level) * DPoint2D(cos(winkel), sin(winkel));
            pic.drawLine(pos, posneu, DrawColour(164, 70, 8));
            pos = posneu;
            break;
          case 'l': // Blatt: größerer grüner Kreis
            pic.drawCircle(pos, 5, true, DrawColour(12, 180, 12),
                           DrawColour(197, 237, 197));
            break;
          case '(': // Neigung nach Links
            posstack.push(pos);
            winkelstack.push(winkel);
            winkel += M_PI / 8 * pow(0.9, level - 1);
            ++level;
            break;
          case '[': // Neigung nach Rechts
            posstack.push(pos);
            winkelstack.push(winkel);
            winkel -= M_PI / 8 * pow(0.9, level - 1);
            ++level;
            break;
          case ')':
          case ']':
            pos = posstack.top();
            posstack.pop();
            winkel = winkelstack.top();
            winkelstack.pop();
            --level;
        }
    cout << "Gemalt: " << ableitung << endl;
  }
};

int maindraw()
{
  vector<Drawing> pics(anzahlBilder);
  vector<Blume> blumen{ DPoint2D(100, 10), DPoint2D(300, 10), DPoint2D(500, 10),
      DPoint2D(700, 10), DPoint2D(900, 10), DPoint2D(1100, 10) } ;
  
  Drawing pic(1400, 800, 0);
  pic.show();
  for (int i = 0; i < anzahlBilder; ++i)
    {
      pic = DrawColour(227, 247, 227); // hellgrün
      // Blumen weiterentwickeln lassen
      for (auto& b : blumen)
        b.weiter();
      // Blume malen
      for (auto& b : blumen)
        b.maleMich(pic);
      // Bild fertig
      pics[i] = pic;
    }
  
  Drawing::makeanim(pics, "blume", "gif", 30);
  //Drawing::makeanim(pics, "blume", "mpg");
  
  return 0;
}
