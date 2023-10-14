#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

#include <cppqt.h>

using namespace std;

#include "in-reader.h"
#define CPPQT_DUMMY_CLIP3DPOINT
#define CPPQT_DUMMY_DRAWPOINTZ
#define CPPQT_DUMMY_LIGHTING
#include "proj-clip.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<start

bool clip3D1(Vec3D& anf, Vec3D& end, double t1, double t2)
{
  // Clipping-Hilfsfunktion

  Vec3D delta;

  if (t1 > t2) //Annahme
    swap(t1, t2);
  if (t1 > 1 || t2 < 0)   //nichts zu tun
    return false;
  
  // i) iii) iv)
  delta = end - anf;  //P1- P0
  if (t2 < 1)
    end = anf + t2 * delta; //P1 = P0 +t2*delta
  if (t1 > 0)
    anf += t1 * delta;   //P0 = P1 +t1*delta
  return true;
}

bool clip3D2(Vec3D& anf, Vec3D& end, double t1, double t2)
{
  // Clipping-Hilfsfunktion

  Vec3D delta;

  delta = end - anf;
  double z1 = anf.el[2] + t1 * delta.el[2];
  double z2 = anf.el[2] + t2 * delta.el[2];
  if (z1 > 0 && z2 > 0)
    return false;
  if (z1 <= 0 && z2 <= 0)
    return clip3D1(anf, end, t1, t2);
  double tu = min(t1, t2);                                  // unteres t
  if (tu < 0)
    return false;
  if (tu < 1)
    end = anf + tu * delta;
  return true;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>end

bool clip3D(Vec3D& anf, Vec3D& end, double zmin)
{
  // 3D-Clipping im kanonischen Bildraum der Zentralprojektion
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  double t1, t2; 
  //------------------------ 1. z in [-1; zmin]-----------------------------
  if (anf.el[2] == end.el[2])
  {
    // Sonderfall: Kante parallel zur x-y-Ebene
    if (anf.el[2] < -1 || anf.el[2] > zmin)
      return false;
  }
  else
  {
    // allgemeiner Fall
    // z = -1 --> -1 = z0+t(z1-z0) --> t = (z0+1)/-(z1-z0)
    t1 = (anf.el[2] + 1) / (anf.el[2] - end.el[2]);
    // z = zmin --> zmin = z0+t(z1-z0) --> t = (z0-zmin)/-(z1-z0)
    t2 = (anf.el[2] - zmin) / (anf.el[2] - end.el[2]);
    if (!clip3D1(anf, end, t1, t2))
      return false;
  }
  //--------------------------------------------------------------------------
  

  if (anf.el[2] > end.el[2])
    swap(anf, end);


  //------------------------ 2. x in [z; -z]---------------------------
  if (anf.el[2] == end.el[2] && anf.el[0] == end.el[0])
  {
    // Sonderfall: Kante parallel zur y-Achse
    if (anf.el[0] < anf.el[2] || anf.el[0] > -anf.el[2])
      return false;
  }
  else
  {
    // allgemeiner Fall
    // x = z --> x0+t(x1-x0) = z0+t(z1-z0) --> t = (z0-x0)/((x1-x0)-(z1-z0))
    t1 = (anf.el[2] - anf.el[0])
      / (end.el[0] - anf.el[0] + anf.el[2] - end.el[2]);
    // x = -z --> x0+t(x1-x0) = -z0-t(z1-z0) --> t = (x0+z0)/(-(z1-z0)-(x1-x0))
    t2 = (anf.el[2] + anf.el[0])
      / (anf.el[0] - end.el[0] + anf.el[2] - end.el[2]);
    if (!clip3D2(anf, end, t1, t2))
      return false;
  }
  //--------------------------------------------------------------------------


  //------------------------ 3. y in [z; -z]---------------------------
  if (anf.el[2] == end.el[2] && anf.el[1] == end.el[1])
  {
    // Sonderfall: Kante parallel zur x-Achse
    if (anf.el[1] < anf.el[2] || anf.el[1] > -anf.el[2])
      return false;
  }
  else
  {
    // allgemeiner Fall
    // y = z --> y0+t(y1-y0) = z0+t(z1-z0) --> t = (z0-y0)/((y1-y0)-(z1-z0))
    t1 = (anf.el[2] - anf.el[1])
      / (end.el[1] - anf.el[1] + anf.el[2] - end.el[2]);
    // y = -z --> y0+t(x1-x0) = -z0-t(z1-z0) --> t = (y0+z0)/(-(z1-z0)-(y1-y0))
    t2 = (anf.el[2] + anf.el[1])
      / (anf.el[1] - end.el[1] + anf.el[2] - end.el[2]);
    if (!clip3D2(anf, end, t1, t2))
      return false;
  }

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  return true;
}

int maindraw()
{
  Drawing pic(1200, 800);
  pic.show();

  bool doClip = true;                                // clipping ja/nein

  vector<Dreieck> dreiecke;                       // die Dreiecke selbst
  vector<Kante> kanten;                             // die Kanten selbst
  ClipQuad clip;                                 // clipping Information
  Vec3D cop;                     // center of projection = Augenposition
  Vec3D tgt;                              // target = Betrachteter Punkt
  Vec3D vrp;                // view reference point = Bildebenenursprung

  Vec3D vup(0, 1, 0);               // view-up vector = Aufwärtsrichtung
  Matrix4x4 nzen;                   // Transformation zur Normalisierung
                                                   //auf kanon. Bildraum

  modellEinlesen(dreiecke, cop, tgt);
  clipInfoEinlesen(clip);
  dreieckeToKanten(dreiecke, kanten);

  vrp = lookAt(cop, tgt);
  nzen = berechneTransformation(cop, vrp, vup, clip,
                                pic.getWidth(), pic.getHeight());

  maleClipRegion(pic, clip);

  maleLinien(pic, kanten, nzen, clip, doClip);

  pic.savePNG("proj2.png");

  IOThread::waitForWindow(10);

  return 0;
}
