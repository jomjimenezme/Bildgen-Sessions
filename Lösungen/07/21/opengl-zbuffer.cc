// z-Buffer (OpenGL)
// Autor: Sebastian Birk, Martin Galgon
// Datum: 19.01.2009

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>

#include "glsupport.h"
#include "gltypes.h"

using namespace std;

#include "in-reader.h"

void maleDreiecke( vector<Dreieck> &dreiecke )
{
  // Malt die transformierten Kanten
  glBegin( GL_TRIANGLES );
    int n = dreiecke.size();
    for ( int i = 0; i < n; i++ )
    {
      Vec3D ecke1(dreiecke[i].ecke[0].el[0], dreiecke[i].ecke[0].el[1], dreiecke[i].ecke[0].el[2]);
      Vec3D ecke2(dreiecke[i].ecke[1].el[0], dreiecke[i].ecke[1].el[1], dreiecke[i].ecke[1].el[2]);
      Vec3D ecke3(dreiecke[i].ecke[2].el[0], dreiecke[i].ecke[2].el[1], dreiecke[i].ecke[2].el[2]);

      Vec3D ndr = kreuzprod( ecke1 - ecke2, ecke1 - ecke3 );
      Vec3D ndr1 = ndr / norm(ndr);

      glNormal3f(ndr1.el[0], ndr1.el[1], ndr1.el[2]);

      GLfloat mMaterial[4] = { dreiecke[i].col.red, dreiecke[i].col.green, dreiecke[i].col.blue, 1.0f };
      GLfloat mAmbient[4]  = { 0.5f  * mMaterial[0],        0.5f  * mMaterial[1],        0.5f  * mMaterial[2]       , 1.0f };
      GLfloat mDiffuse[4]  = { 0.95f * mMaterial[0],        0.95f * mMaterial[1],        0.95f * mMaterial[2]       , 1.0f };
      GLfloat mSpecular[4] = { 0.4f  * mMaterial[0] + 0.3f, 0.4f  * mMaterial[1] + 0.3f, 0.4f  * mMaterial[2] + 0.3f, 1.0f };

      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  mAmbient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mDiffuse);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mSpecular);
      // OpenGL verwendet Blinn-Phong, statt Phong. Deshalb muss der
      // Faktor etwas angepasst werden. Identisch können die Bilder aber
      // nie sein.
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 4.0f * 1.2f);

      glVertex3f( dreiecke[i].ecke[0].el[0], dreiecke[i].ecke[0].el[1], dreiecke[i].ecke[0].el[2] );
      glVertex3f( dreiecke[i].ecke[1].el[0], dreiecke[i].ecke[1].el[1], dreiecke[i].ecke[1].el[2] );
      glVertex3f( dreiecke[i].ecke[2].el[0], dreiecke[i].ecke[2].el[1], dreiecke[i].ecke[2].el[2] );
    }
  glEnd();
}

/*******************************************************************
 * OpenGL-Funktionen
 */

// initScene initialisiert alles, was von OpenGL benoetigt wird.
void initScene( vector<GLint>& Objekte )
{
  vector<Dreieck> dreiecke;

  Vec3D cop;
  Vec3D tgt;
  Vec3D vup(0, 1, 0);

  // Eingabe der Daten
  modellEinlesen(dreiecke, cop, tgt);

  Vec3D vrp = lookAt(cop, tgt);

    // Wir wollen senkrecht auf den Ursprung der Bildebene schauen
  double fov = 90.0;
  double aspect = static_cast<double>(windowSize[0])
                / static_cast<double>(windowSize[1]);
  // In Opengl sind Projektionsebene und vordere Clippingebene ein- und
  // dasselbe; znear kann deshalb hier nicht null sein.
  // Ein zu kleines znear reduziert die z-Buffer Präzision.
  double znear = 0.5;
  double zfar = 9999.0;
  double vmax = znear * tan(fov * M_PI / 360.0);
  double vmin = -vmax;
  double umin = vmin * aspect;
  double umax = vmax * aspect;

  // Anzeige-Einstellungen
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glClearColor( 0.75f, 0.75f, 0.75f, 1.0f );

  // Projektionseinstellungen
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  glViewport( 0, 0, windowSize[0], windowSize[1] );
  glFrustum(umin, umax, vmin, vmax, znear, zfar);

  // Szeneneinstellungen
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( cop.el[0], cop.el[1], cop.el[2],
             vrp.el[0], vrp.el[1], vrp.el[2],
             vup.el[0], vup.el[1], vup.el[2] );

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_RESCALE_NORMAL);

  Vec3D offset = kreuzprod(vrp - cop, vup);
  offset = offset / norm(offset);
  offset = 0.5 * norm(cop) * (offset + 0.5 * vup);
  Vec3D l = cop + offset; // Position der Lichtquelle

  GLfloat lPosition[4] = { (GLfloat)l.el[0], (GLfloat)l.el[1], (GLfloat)l.el[2], 1.0 };

  glLightfv(GL_LIGHT0, GL_POSITION, lPosition);

  // Opengl verwendet zwei ambiente Terme. Einmal einen globalen
  // ambienten Term für die gesamte Szene und einmal einen ambienten
  // Term für die Lichtquellen, auf den auch die entfernungsabhängige
  // Dämpfung angewendet wird. Unser Modell beinhaltet nur den globalen
  // ambienten Term.
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, cWhite);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  cBlack);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  cWhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cWhite);

  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.00001);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0000004);

  beginneObjekt(Objekte);
    maleDreiecke(dreiecke);
  beendeObjekt();
}

int main( int argc, char *argv[] )
{
  SDL_Event       event;
  vector<GLint>   vObjekte;

  // SDL initialisieren
  initialisiereSDL();

  // Szene initialisieren
  initScene(vObjekte);

  // Ereignis-Schleife starten
  while(!finished)
  {
    // auf events reagieren
    SDL_WaitEvent(&event);
    // Auf events reagieren, Objekte zeichnen
    behandleEvents(event, vObjekte);
  }

  finalisiereSDL();

  return 0;
}
