//======================================================================
class Kante              // eine Kante im Raum mit homogenen Koordinaten
{
public:
  Vec4D anf, end;                               // Anfangs- und Endpunkt
  DrawColour col;                                               // Farbe
  Kante() {}
  Kante(const Vec4D& a, const Vec4D& e, const DrawColour& c)
    : anf(a), end(e), col(c) {}
};
//======================================================================
class Dreieck           // ein Dreieck im Raum mit homogenen Koordinaten
{
public:
  Vec4D ecke[3];                                                // Ecken
  DrawColour col;                                               // Farbe
};
//======================================================================
class ClipQuad                         // enthält clipping Informationen
{
public:
  double uminf;                              // Faktor für umin in [0,1]
  double umaxf;                              // Faktor für umax in [0,1]
  double vminf;                              // Faktor für vmin in [0,1]
  double vmaxf;                              // Faktor für vmax in [0,1]
  double nearf;                 // Faktor für znear, near clipping plane
  double farf;                    // Faktor für zfar, far clipping plane
};
//======================================================================
void dreieckeToKanten(const vector<Dreieck>& dr, vector<Kante>& k)
{
  int nd = dr.size();
  int n = 3 * nd;
  k.resize(n);
  for (int i = 0; i < nd; ++i)
    {
      k[3 * i].anf = dr[i].ecke[0];
      k[3 * i].end = dr[i].ecke[1];
      k[3 * i + 1].anf = dr[i].ecke[1];
      k[3 * i + 1].end = dr[i].ecke[2];
      k[3 * i + 2].anf = dr[i].ecke[2];
      k[3 * i + 2].end = dr[i].ecke[0];
      k[3 * i].col = k[3 * i + 1].col = k[3 * i + 2].col = dr[i].col;
    }
}
//======================================================================
void viewPortEinlesen(Vec3D& cop, Vec3D& tgt)
{
  if((cin >> ws).peek() == -1) return;

  cout << "cop: ";
  cin >> cop.el[0] >> cop.el[1] >> cop.el[2];
  cin.ignore(9999, '\n');
  cout << "tgt: ";
  cin >> tgt.el[0] >> tgt.el[1] >> tgt.el[2];
  cin.ignore(9999, '\n');
  cout << endl;
}
//======================================================================
// Berechnet den kanonischen VRP im Abstand 1 zu COP mit Blick auf TGT.
Vec3D lookAt(const Vec3D& cop, const Vec3D& tgt)
{
  Vec3D ivpn = tgt - cop;
  ivpn = ivpn / norm(ivpn);
  return cop + ivpn;
}
//======================================================================
#define ClipQuadDefault {1.0, 1.0, 1.0, 1.0, 0.0, 9999.0}
//======================================================================
void clipInfoEinlesen(ClipQuad& clip)
{
  if((cin >> ws).peek() == -1){
    clip = ClipQuadDefault;
    return;
  }

  cout << "uminf, umaxf: ";
  cin >> clip.uminf >> clip.umaxf;
  cin.ignore(9999, '\n');
  cout << "vminf, vmaxf: ";
  cin >> clip.vminf >> clip.vmaxf;
  cin.ignore(9999, '\n');
  cout << "nearf, farf: ";
  cin >> clip.nearf >> clip.farf;
  cin.ignore(9999, '\n');
  cout << endl;
}
//======================================================================
void modellEinlesen(vector<Dreieck>& dreiecke, Vec3D& cop, Vec3D& tgt)
{
  int i;
  int r, g, b;              // rot, grün, blau
  int nd;                   // Anzahl der Dreiecke

  cout << "cop: ";
  cin >> cop.el[0] >> cop.el[1] >> cop.el[2];
  cin.ignore(9999, '\n');
  cout << "tgt: ";
  cin >> tgt.el[0] >> tgt.el[1] >> tgt.el[2];
  cin.ignore(9999, '\n');
  cout << "Anzahl der Dreiecke: ";
  cin >> nd;
  cin.ignore(9999, '\n');
  dreiecke.resize(nd);
  cout << "Ecken: ";
  for (i = 0; i < nd; ++i)
    {
      cin >> dreiecke[i].ecke[0].el[0] >> dreiecke[i].ecke[0].el[1]
          >> dreiecke[i].ecke[0].el[2] >> dreiecke[i].ecke[1].el[0]
          >> dreiecke[i].ecke[1].el[1] >> dreiecke[i].ecke[1].el[2]
          >> dreiecke[i].ecke[2].el[0] >> dreiecke[i].ecke[2].el[1]
          >> dreiecke[i].ecke[2].el[2];
      dreiecke[i].ecke[0].el[3] = dreiecke[i].ecke[1].el[3]
                                = dreiecke[i].ecke[2].el[3] = 1;
      cin >> r >> g >> b;
      dreiecke[i].col = DrawColour(r, g, b);
    }
  cout << endl;
}
//======================================================================
