//======================================================================
bool clip3DPoint(const Vec3D& p, double zmin)
{
  // 3D-Clipping im kanonischen Bildraum der Zentralprojektion
  // hier für einen einzelnen Punkt statt einer Linie.

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
  if (
       p.el[2] < -1            // weiter weg als z=-1
    || p.el[2] > zmin          // näher als z=zmin
    || p.el[0] < p.el[2]       // links der linken Kappungsebene
    || p.el[0] > -p.el[2]      // rechts der rechten Kappungsebene
    || p.el[1] < p.el[2]       // unterhalb der unteren Kappungsebene
    || p.el[1] > -p.el[2]      // oberhalb der oberen Kappungsebene
  )
    return false;
  return true;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
//======================================================================
inline void drawPointZ (Drawing& pic, int x, int y, double z,
                        vector<vector<double> >& zbuf, DrawColour colour)
{
  // Malt ins Bild pic einen Punkt an die Stelle (x, y), der die Tiefe z
  // hat, falls es dort noch keinen Punkt geringerer oder gleicher Tiefe
  // gibt; in diesem Fall wird zusätzlich der Eintrag im z-Buffer
  // aktualisiert.

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  if (   x < 0 || x >= static_cast<int>(zbuf.size())
      || y < 0 || y >= static_cast<int>(zbuf[0].size()))
    return;

  if (z > zbuf[x][y])
  {
    pic.drawPoint(x, y, colour);
    zbuf[x][y] = z;
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
//======================================================================
