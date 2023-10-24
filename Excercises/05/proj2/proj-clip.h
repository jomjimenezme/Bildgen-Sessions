typedef Vec3D VecRGB;
//======================================================================
extern bool clip3D(Vec3D& anf, Vec3D& end, double zmin);
extern bool clip3DPoint(const Vec3D& p, double zmin);
extern void drawPointZ(Drawing& pic, int x, int y, double z,
                       vector<vector<double> >& zbuf, DrawColour colour);
extern VecRGB berechneBeleuchtung(const Vec3D& ecke, const Vec3D& normale,
                                  const Vec3D& auge, const Vec3D& licht,
                                  const DrawColour& farbe);
//======================================================================
#ifdef CPPQT_DUMMY_CLIP3D
bool clip3D(Vec3D&, Vec3D&, double){ return true; }
#endif
#ifdef CPPQT_DUMMY_CLIP3DPOINT
bool clip3DPoint(const Vec3D&, double){ return true; }
#endif
#ifdef CPPQT_DUMMY_DRAWPOINTZ
void drawPointZ(Drawing&, int, int, double, vector<vector<double> >&, DrawColour){}
#endif
#ifdef CPPQT_DUMMY_LIGHTING
VecRGB berechneBeleuchtung(const Vec3D&, const Vec3D&,
                           const Vec3D&, const Vec3D&,
                           const DrawColour& farbe)
{
  return VecRGB( farbe.red()   / 255.0,
                 farbe.green() / 255.0,
                 farbe.blue()  / 255.0 );
}
#endif
//======================================================================
DPoint2D homTo2D(const Vec4D& v)
{
         // nur sinnvoll, wenn durch Projektion bereits v.el[2] == 0 ist
  return DPoint2D(v.el[0] / v.el[3], v.el[1] / v.el[3]);
}
//======================================================================
Vec3D homTo3D(const Vec4D& v)
{
  return Vec3D(v.el[0] / v.el[3], v.el[1] / v.el[3], v.el[2] / v.el[3]);
}
//======================================================================
Matrix4x4 berechneTransformation(const Vec3D& cop, const Vec3D& vrp,
                                 const Vec3D& vup, ClipQuad clip,
                                 int w, int h)
{
  // Berechnet die normalisierende Transformation nzen, die die Projektion
  // in den kanonischen Bildraum bewirkt für:
  // IN:  Projektionszentrum cop (Augenposition)
  // IN:  Bildebenenursprung vrp
  // IN:  Aufwärtsvektor vup
  // IN:  Projektionsfenster Definition clip
  // IN:  w, h Fenstergröße

  Matrix4x4 nzen;                                      // fertige Matrix
  Matrix4x4 tvrp;                                     // Translation vrp
  Matrix4x4 rot;                                         // Rotation uvn
  Matrix4x4 tprp; // Translation des transformierten cop in den Ursprung
  Matrix4x4 sh;                                              // Scherung
  Matrix4x4 szen;                 // Skalierung auf kanonischen Bildraum

  Vec3D u, v, n;
  Vec3D cw;                        // Bildmittelpunkt (center of window)
  Vec3D dop;            // Projektionsrichtung (direction of projection)
  Vec3D vpn;                                          // Viewport normal
  int i;

  // Da wir (zumindest hier) immer senkrecht auf die Projektionsebene
  // schauen wollen, legen wir die Normale fest wie folgt.
  vpn = cop - vrp;
  double dist = norm(vpn);           // Der Abstand zur Projektionsebene

  double fov = 90.0;                                 // 90° Sichtbereich
  double aspect = static_cast<double>(w)             // Seitenverhältnis
                / static_cast<double>(h);
  double vmax = dist * tan(fov * M_PI / 360.0);
  double vmin = -vmax;
  double umin = vmin * aspect;
  double umax = vmax * aspect;
  umin *= clip.uminf;
  umax *= clip.umaxf;
  vmin *= clip.vminf;
  vmax *= clip.vmaxf;

  // Hintere clipping Ebene als vielfaches des Abstandes zur
  // Projektionsebene; diese Wahl von zfar und znear = dist * clip.nearf
  // erlaubt, das in den kanonischen Bildraum transformierte dist als
  // (1.0 / clip.farf) und das transformierte znear als
  // (clip.nearf / clip.farf) zu berechnen.
  double zfar = dist * clip.farf;

  // Verschiebung von vrp in den Ursprung
  tvrp.el[0][0] = tvrp.el[1][1] = tvrp.el[2][2] = tvrp.el[3][3] = 1;
  tvrp.el[0][3] = -vrp.el[0];
  tvrp.el[1][3] = -vrp.el[1];
  tvrp.el[2][3] = -vrp.el[2];

  // Bestimme (u,v,n)-Koordinatensystem
  n = vpn / norm(vpn);
  v = vup - skalarprod(vup, n) * n;
  v = v / norm(v);
  u = kreuzprod(v, n);

  // Rotation: (x, y, z) ⇝ (u, v, n)
  for (i = 0; i < 3; ++i)
    {
      rot.el[0][i] = u.el[i];
      rot.el[1][i] = v.el[i];
      rot.el[2][i] = n.el[i];
    }
  rot.el[3][3] = 1;

  // Verschiebung um -dist
  tprp.el[0][0] = tprp.el[1][1] = tprp.el[2][2] = tprp.el[3][3] = 1;
  tprp.el[2][3] = -dist;

  // Scherung
  cw.el[0] = (umax + umin) / 2;
  cw.el[1] = (vmax + vmin) / 2;
  dop = cw - Vec3D(0, 0, dist);
  sh.el[0][0] = sh.el[1][1] = sh.el[2][2] = sh.el[3][3] = 1;
  sh.el[0][2] = -dop.el[0] / dop.el[2];
  sh.el[1][2] = -dop.el[1] / dop.el[2];

  // Skalierung auf kanonischen Bildraum
  // Eigentlich zwei Skalierungen,
  // eine in x,y,z:
  //    (-1/zfar, -1/zfar, -1/zfar)
  // und eine in x,y:
  //    (-2*dist/du, -2*dist/dv, 1)
  double du = umax - umin;
  double dv = vmax - vmin;

  szen.el[0][0] = 2.0 * dist / (du * zfar);
  szen.el[1][1] = 2.0 * dist / (dv * zfar);
  szen.el[2][2] = 1.0 / zfar;
  szen.el[3][3] = 1;

  // Multiplikation der einzelnen Transformationen
  nzen = szen * sh * tprp * rot * tvrp;

  return nzen;
}
//======================================================================
Matrix4x4 matrixMzen(double dist, ClipQuad clip, int w, int h)
{
  Matrix4x4 mzen, t1, t2, s;

  // Zentralprojektion
  mzen.el[0][0] = mzen.el[1][1] = dist;
  mzen.el[3][2] = -1;

  // Verschiebung in positiven quadranten
  t1.el[0][0] = t1.el[1][1] = t1.el[3][3] = 1;
  t1.el[0][3] = dist;
  t1.el[1][3] = dist;

  // Skalierung auf fenster
  s.el[0][0] = w * 0.5 * (clip.uminf + clip.umaxf) / (2.0 * dist);
  s.el[1][1] = h * 0.5 * (clip.vminf + clip.vmaxf) / (2.0 * dist);
  s.el[3][3] = 1;

  // Translation an Zielposition
  t2.el[0][0] = t2.el[1][1] = t2.el[3][3] = 1;
  t2.el[0][3] = (1.0 - clip.uminf) * 0.5 * w;
  t2.el[1][3] = (1.0 - clip.vminf) * 0.5 * h;

  return  t2 * s * t1 * mzen;
}
//======================================================================
// Malt die transformierten Kanten ins Bild pic
void maleLinien(Drawing& pic, const vector<Kante>& kanten,
                const Matrix4x4& nzen, ClipQuad clip, bool doClip)
{
  Vec4D nanf, nend;         // Anfangs- und Endpunkt nach Normalisierung
  Vec3D canf, cend;               // Anfangs- und Endpunkt nach Clipping
  Vec4D zanf, zend;      // Anfangs- und Endpunkt nach Zentralprojektion
  IPoint2D panf, pend;              // Anfangs- und Endpunkt im Bild pic

  Matrix4x4 mzen = matrixMzen(1.0 / clip.farf, clip, pic.getWidth(), pic.getHeight());

  int n = kanten.size();
  int m = n;
  if(m < 500) m = 500;
  for (int i = 0; i < n; ++i)
    {
      // Normalisiere Anfangs- und Endpunkt der Kante
      // d.h. Tranformation in kanonischen Bildraum
      nanf = nzen * kanten[i].anf;
      nend = nzen * kanten[i].end;

      // Clipping
      canf = homTo3D(nanf);
      cend = homTo3D(nend);
      if (doClip)
        if (!clip3D(canf, cend, -clip.nearf / clip.farf)) // ändert canf und cend
          continue; // wenn Kante komplett unsichtbar, weiter mit nächster Kante

      // Führe Zentralprojektion aus
      zanf = mzen * Vec4D(canf);
      zend = mzen * Vec4D(cend);

      // Umwandlung von 3D homogen nach 2D
      panf = homTo2D(zanf);
      pend = homTo2D(zend);

      // Male Linien
      pic.drawLine(panf, pend, kanten[i].col, n < 100 || i % (m / 500) == 0);
    }
}
//======================================================================
// Nur x/y
void maleClipRegion(Drawing& pic, ClipQuad clip)
{
  int w = pic.getWidth();
  int h = pic.getHeight();
  int lx = floor( (1.0 - clip.uminf) * 0.5 * w ) - 1;
  int ly = floor( (1.0 - clip.vminf) * 0.5 * h ) - 1;
  int ux = ceil( (1.0 + clip.umaxf) * 0.5 * w );
  int uy = ceil( (1.0 + clip.vmaxf) * 0.5 * h );
  DrawColour d(255, 0, 0);
  pic.drawLine(lx, ly, lx, uy, d);
  pic.drawLine(lx, ly, ux, ly, d);
  pic.drawLine(ux, uy, lx, uy, d);
  pic.drawLine(ux, uy, ux, ly, d);
}
//======================================================================
// Berechnet den Flächeninhalt eines Dreiecks
double inline dreiecksFlaeche(IPoint2D p1, IPoint2D p2, IPoint2D p3)
{
  return abs(0.5 * ((p1.x - p2.x) * (p3.y - p2.y)
                  - (p3.x - p2.x) * (p1.y - p2.y)));
}
//======================================================================
Vec3D inline baryzentrisch1(DPoint2D p, DPoint2D dreieck[3], double n)
{
  Vec3D b(
    dreiecksFlaeche(p, dreieck[1], dreieck[2]) / n,
    dreiecksFlaeche(p, dreieck[0], dreieck[2]) / n,
    0 // dreiecksFlaeche(p, dreieck[0], dreieck[1]) / n
  );
  b.el[2] = 1.0 - b.el[0] - b.el[1];
  return b;
}
//======================================================================
double inline baryzentrisch1(DPoint2D dreieck[3])
{
  return dreiecksFlaeche(dreieck[0], dreieck[1], dreieck[2]);
}
//======================================================================
Vec3D inline baryzentrisch1(DPoint2D p, DPoint2D dreieck[3])
{
  double n = baryzentrisch1(dreieck);
  return baryzentrisch1(p, dreieck, n);
}
//======================================================================
Vec3D inline baryzentrisch2(DPoint2D p, DPoint2D d[3], double n)
{
  Vec3D b(
    (   (d[1].y - d[2].y) * (p.x - d[2].x)
      + (d[2].x - d[1].x) * (p.y - d[2].y) ) / n,
    (   (d[2].y - d[0].y) * (p.x - d[2].x)
      + (d[0].x - d[2].x) * (p.y - d[2].y) ) / n,
    0 // (d[0].y - d[1].y) * (p.x - d[0].x) + (d[1].x - d[0].x) * (p.y - d[0].y) ) / n
  );
  b.el[2] = 1.0 - b.el[0] - b.el[1];
  return b;
}
//======================================================================
double inline baryzentrisch2(DPoint2D d[3])
{
  return (d[1].y - d[2].y) * (d[0].x - d[2].x) + (d[2].x - d[1].x) * (d[0].y - d[2].y);
}
//======================================================================
Vec3D inline baryzentrisch2(DPoint2D p, DPoint2D dreieck[3])
{
  double n = baryzentrisch2(dreieck);
  return baryzentrisch2(p, dreieck, n);
}
//======================================================================
double inline edgefun(IPoint2D p, IPoint2D a, IPoint2D b)
{
  return (a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - a.y * b.x;
}
//======================================================================
Vec3D inline baryzentrisch3(IPoint2D p, IPoint2D d[3], double w)
{
  Vec3D b(
    edgefun(p, d[1], d[2]) / w,
    edgefun(p, d[2], d[0]) / w,
    0 // edgefun(p, d[0], d[1]) / w
  );
  b.el[2] = 1.0 - b.el[0] - b.el[1];
  return b;
}
//======================================================================
double inline baryzentrisch3(IPoint2D d[3])
{
  return (d[2].x - d[1].x) * (d[0].y - d[1].y) - (d[0].x - d[1].x) * (d[2].y - d[1].y);
}
//======================================================================
Vec3D inline baryzentrisch3(IPoint2D p, IPoint2D d[3])
{
  double w = baryzentrisch3(d);
  return baryzentrisch3(p, d, w);
}
//======================================================================
void inline baryzentrisch3(IPoint2D d[3], Vec3D& a, Vec3D& b)
{
  a = Vec3D( (d[1].y - d[2].y),
             (d[2].y - d[0].y),
             (d[0].y - d[1].y) );
  b = Vec3D( (d[2].x - d[1].x),
             (d[0].x - d[2].x),
             (d[1].x - d[0].x) );
}
//======================================================================
bool inline barycheck(Vec3D b)
{
  double checklim = -1e-10;
  return (    b.el[0] >= checklim
           && b.el[1] >= checklim
           && b.el[2] >= checklim
           && b.el[0] + b.el[1] + b.el[2] - 1.0 < 1e-11 );
}
//======================================================================
double inline abstand(IPoint2D p, IPoint2D a, IPoint2D b)
{
  double dx = b.x - a.x;
  double dy = b.y - a.y;
  return abs(dy * p.x - dx * p.y + b.x * a.y - b.y * a.x) / sqrt(dy * dy + dx * dx);
}
//======================================================================
void inline abstaende(IPoint2D ecken[3], double& d1, double& d2, double& d3)
{
  double onethird = 1.0 / 3.0;

  DPoint2D center = DPoint2D(
    onethird * ecken[0].x + onethird * ecken[1].x + onethird * ecken[2].x,
    onethird * ecken[0].y + onethird * ecken[1].y + onethird * ecken[2].y
  );

  d1 = abstand(center, ecken[1], ecken[2]);
  d2 = abstand(center, ecken[0], ecken[2]);
  d3 = abstand(center, ecken[0], ecken[1]);
}
//======================================================================
// f: Zu interpolierende Größe an den Dreieckseckpunkten.
// w: Homogene Koordinate der Dreieckseckpunkte.
// b: Baryzentrische Koordinaten im Rasterisierungsdreieck.
double inline perspektivenInterpolation(Vec3D b, double f[3], double w[3])
{
  double c = b.el[0] * w[0];
  double d = b.el[1] * w[1];
  double e = b.el[2] * w[2];
  return (f[0] * c + f[1] * d * f[2] * e) / (c + d + e);
}
//======================================================================
Vec3D inline perspektivenInterpolation(Vec3D b, Vec3D f[3], double w[3])
{
  double c = b.el[0] * w[0];
  double d = b.el[1] * w[1];
  double e = b.el[2] * w[2];
  double s = (c + d + e);
  return Vec3D(
    (f[0].el[0] * c + f[1].el[0] * d + f[2].el[0] * e) / s,
    (f[0].el[1] * c + f[1].el[1] * d + f[2].el[1] * e) / s,
    (f[0].el[2] * c + f[1].el[2] * d + f[2].el[2] * e) / s
  );
}
//======================================================================
void inline initZBuffer(vector<vector<double>>& zbuf, int w, int h)
{
  zbuf.resize(w);
  for (int i = 0; i < w; ++i)
  {
    zbuf[i].resize(h);
    for (int j = 0; j < h; ++j)
      zbuf[i][j] = numeric_limits<int>::min();               // statt −∞
  }
}
//======================================================================
void rasterizer_simple(Drawing& pic, IPoint2D iEcken2D[3], Vec3D ecken3D[3],
                       VecRGB eckenFarbe[3], int ymin, int ymax, double w[3],
                       double zmin, bool doClip, bool outline,
                       vector<vector<double>>& zbuf)
{
  int xmin = min(min(iEcken2D[0].x, iEcken2D[1].x), iEcken2D[2].x);
  int xmax = max(max(iEcken2D[0].x, iEcken2D[1].x), iEcken2D[2].x);

  double f = baryzentrisch3(iEcken2D);

  // Umrandung des Dreiecks
  double d1 = 0, d2 = 0, d3 = 0, lim = 0.1;
  if(outline) abstaende(iEcken2D, d1, d2, d3);

  Vec3D a, b;
  baryzentrisch3(iEcken2D, a, b);
  a = a / f;
  b = b / f;

  int guardband = 5;
  Vec3D base = baryzentrisch3(DPoint2D(xmin-guardband, ymin-guardband), iEcken2D, f);

  for (int y = ymin-guardband; y <= ymax+guardband; ++y){
    Vec3D bary = base;
    for (int ix = xmin-guardband; ix <= xmax+guardband; ++ix){
      bary = baryzentrisch3(IPoint2D(ix, y), iEcken2D, f);
      if (barycheck(bary)){
        Vec3D point = perspektivenInterpolation(bary, ecken3D, w);
        if ((!doClip || clip3DPoint(point, zmin))){

          Vec3D vc = perspektivenInterpolation(bary, eckenFarbe, w);
          DrawColour c = DrawColour(vc.el[0]*255,vc.el[1]*255,vc.el[2]*255);

          if(outline && ( bary.el[0] * d1 < lim
                       || bary.el[1] * d2 < lim
                       || bary.el[2] * d3 < lim ))
            c = DrawColour();
          drawPointZ(pic, ix, y, point.el[2], zbuf, c);
        }
      }
      bary += a;
    }
    base += b;
  }
}
//======================================================================
// Licht irgendwo rechts oben neben dem Auge
Vec3D standardLichtQuelle(const Vec3D& vrp, const Vec3D& cop, const Vec3D& vup)
{
  Vec3D offset = kreuzprod(vrp - cop, vup);
  offset = offset / norm(offset);
  offset = 0.5 * norm(cop) * (offset + 0.5 * vup);
  return cop + offset; // Position der Lichtquelle
}
//======================================================================
// Malt die transformierten Dreiecke ins Bild pic
void maleDreiecke(Drawing& pic, const vector<Dreieck>& dreiecke,
                  const Matrix4x4& nzen, ClipQuad clip, bool doClip,
                  const Vec3D& cop = Vec3D(), const Vec3D& l = Vec3D(),
                  bool outline = true, bool slow = true,
                  bool phong = false)
{
  int n = dreiecke.size();
  vector<vector<double>> zbuf;                                       // z-Buffer
  IPoint2D iEcken2D[3];
  IPoint2D e1, e2;                                          // zwei dieser Ecken
  int ymin, ymax;                                     // y-Bereich in Projektion
  vector<int> linkerrand(pic.getHeight());         // x-Bereich für ein festes y
  vector<int> rechterrand(pic.getHeight());
  int y;
  double x;
  double einsdurchm;
  int i, j;
  int k;
  int wid = pic.getWidth();
  int hei = pic.getHeight();

  Matrix4x4 mzen = matrixMzen(1.0 / clip.farf, clip, wid, hei);
  double zmin = -clip.nearf / clip.farf;

  // Initialisiere z-Buffer
  initZBuffer(zbuf, wid, hei);

  // Schleife über die Dreiecke
  for (i = 0; i < n; ++i)
  {
    // Drei Ecken in Weltkoordinaten:
    Vec3D eckenWelt[3];
    eckenWelt[0] = homTo3D(dreiecke[i].ecke[0]);
    eckenWelt[1] = homTo3D(dreiecke[i].ecke[1]);
    eckenWelt[2] = homTo3D(dreiecke[i].ecke[2]);

    // Normalisiere die Eckpunkte des Dreiecks
    // d.h. Tranformation in kanonischen Bildraum
    Vec4D eckenNorm[3];
    eckenNorm[0] = nzen * dreiecke[i].ecke[0];
    eckenNorm[1] = nzen * dreiecke[i].ecke[1];
    eckenNorm[2] = nzen * dreiecke[i].ecke[2];

    // Ecken in clip-Koordinaten
    Vec3D ecken3D[3];
    ecken3D[0] = homTo3D(eckenNorm[0]);
    ecken3D[1] = homTo3D(eckenNorm[1]);
    ecken3D[2] = homTo3D(eckenNorm[2]);

    // Bestimme Normale auf das Dreieck (in Weltkoordinaten), n1 normalisiert
    Vec3D ndr = kreuzprod( eckenWelt[0] - eckenWelt[1],
                           eckenWelt[0] - eckenWelt[2] );
    Vec3D ndr1 = ndr / norm(ndr);

    // Berechne Beleuchtung in Weltkoordinaten für alle Eckpunkte
    VecRGB eckenFarbe[3];
    if(!phong){
      eckenFarbe[0] = berechneBeleuchtung(eckenWelt[0], ndr1, cop, l,
                                          dreiecke[i].col);
      eckenFarbe[1] = berechneBeleuchtung(eckenWelt[1], ndr1, cop, l,
                                          dreiecke[i].col);
      eckenFarbe[2] = berechneBeleuchtung(eckenWelt[2], ndr1, cop, l,
                                          dreiecke[i].col);
    }

    // Führe Zentralprojektion aus
    Vec4D eckenZenP[3];
    eckenZenP[0] = mzen * eckenNorm[0];
    eckenZenP[1] = mzen * eckenNorm[1];
    eckenZenP[2] = mzen * eckenNorm[2];

    // Inverse der homogenen Komponente des projezierten Dreiecks
    double w[] = { 1.0 / eckenZenP[0].el[3],
                   1.0 / eckenZenP[1].el[3],
                   1.0 / eckenZenP[2].el[3] };

    // Umwandlung von 3D homogen nach 2D
    iEcken2D[0] = homTo2D(eckenZenP[0]);
    iEcken2D[1] = homTo2D(eckenZenP[1]);
    iEcken2D[2] = homTo2D(eckenZenP[2]);

    // Dreiecke, die zu klein sind oder zu einer Gerade degeneriert sind,
    // werden verworfen
//     if (dreiecksFlaeche(iEcken2D[0], iEcken2D[1], iEcken2D[2]) < 1)
//       continue;

    // Male Dreiecke
    // Bestimme y-Intervall
    ymin = min(min(iEcken2D[0].y, iEcken2D[1].y), iEcken2D[2].y);
    ymax = max(max(iEcken2D[0].y, iEcken2D[1].y), iEcken2D[2].y);

    // Falls Dreiecke nicht ganz im Bild liegen
    ymin = max(0, ymin);
    ymax = min(hei-1, ymax);

//     rasterizer_simple(pic, iEcken2D, ecken3D, eckenFarbe, ymin, ymax, w,
//                       zmin, doClip, outline, zbuf);

    // Vorberechnung für baryzentrische Koordinaten
    double f = baryzentrisch3(iEcken2D);

    // Umrandung des Dreiecks
    double d1 = 0, d2 = 0, d3 = 0, lim = 0.2;
    if(outline) abstaende(iEcken2D, d1, d2, d3);

    // Inkrementelle baryzentrische Koordinaten; machen u.U. die
    // Berechnung etwas ungenau, wodurch einzelne Pixel verloren gehen
    // können. Bei Bedarf Neuberechnung in jeder Zeile oder sogar für
    // jedes Pixel.
    Vec3D a, b;
    baryzentrisch3(iEcken2D, a, b);
    a = a / f;
    b = b / f;

    // Bestimme nun anhand der 3 Seiten für jede Bildzeile den x-Bereich
    for (y = ymin; y <= ymax; ++y)
    {
      linkerrand[y] = numeric_limits<int>::max();
      rechterrand[y] = -1;
    }

    for (j = 0; j < 3; ++j)
    {
      k = (j + 1) % 3;

      if (iEcken2D[j].y < iEcken2D[k].y){
        e1 = iEcken2D[j];
        e2 = iEcken2D[k];
      }else{
        e1 = iEcken2D[k];
        e2 = iEcken2D[j];
      }

      if (e1.y != e2.y)
      {
        einsdurchm = static_cast<double>(e1.x - e2.x) / (e1.y - e2.y);
        x = e1.x;
        for (y = e1.y; y <= e2.y; ++y)
        {
          if(y >= 0 && y < hei){ // Sonst crash bei Zeilen außerhalb
            if (x < linkerrand[y])  linkerrand[y]  = static_cast<int>(x);
            if (x > rechterrand[y]) rechterrand[y] = static_cast<int>(x);
          }
          x += einsdurchm;
        }
      }
    }

    // Nicht unbedingt nötig, drawPoint prüft das auch
    for (y = ymin; y <= ymax; ++y)
    {
      linkerrand[y] = max(0, linkerrand[y]);
      rechterrand[y] = min(wid-1, rechterrand[y]);
    }

    // Zeichne das Dreieck
    int ix = linkerrand[ymin];
    Vec3D bary = baryzentrisch3(IPoint2D(ix, ymin), iEcken2D, f);

    for (y = ymin; y <= ymax; ++y)
    {
      bary += (linkerrand[y] - ix) * a;

      for (ix = linkerrand[y]; ix <= rechterrand[y]; ++ix)
      {
        // Interpolation nur für Pixel im Dreieck
        if(barycheck(bary)){
          // Koordinaten des Originalpunktes im kanonischen Bildraum
          Vec3D point = perspektivenInterpolation(bary, ecken3D, w);

          // Clipping
          if( !doClip || clip3DPoint(point, zmin) ){

            // Farbe
            VecRGB vc;
            if(phong){
              // Normalen nicht interpoliert
              Vec3D pwc = perspektivenInterpolation(bary, eckenWelt, w);
              vc = berechneBeleuchtung(pwc, ndr1, cop, l, dreiecke[i].col);
            }else{
              vc = perspektivenInterpolation(bary, eckenFarbe, w);
            }

            DrawColour c = DrawColour( vc.el[0] * 255,
                                       vc.el[1] * 255,
                                       vc.el[2] * 255 );

            // Liegt das Pixel auf dem Rand des Dreiecks?
            if(outline && ( bary.el[0] * d1 < lim
                         || bary.el[1] * d2 < lim
                         || bary.el[2] * d3 < lim ))
              c = DrawColour();

            drawPointZ(pic, ix, y, point.el[2], zbuf, c);
          }
        }

        bary += a;
      }

      bary += b;
    }

    if(slow){
      if (n < 30)
        IOThread::msleep(300);
      else if (i % max((n / 1000), 2) == 0)
        IOThread::msleep(10);
    }
  }
}
//======================================================================
