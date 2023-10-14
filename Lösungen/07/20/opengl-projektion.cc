// Projektion und Clipping in OpenGL
// Autor: Sebastian Birk, Martin Galgon
// Datum: 01.12.2008

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

void maleKanten( const vector<Kante>& kanten )
{
  // Malt die transformierten Kanten
  glEnable( GL_COLOR_MATERIAL );
  glBegin( GL_LINES );
    int n = kanten.size();
    for ( int i = 0; i < n; i++ )
    {
      glColor3f(  kanten[i].col.red,   kanten[i].col.green, kanten[i].col.blue  );
      glVertex3f( kanten[i].anf.el[0], kanten[i].anf.el[1], kanten[i].anf.el[2] );
      glVertex3f( kanten[i].end.el[0], kanten[i].end.el[1], kanten[i].end.el[2] );
    }
  glEnd();
  glDisable( GL_COLOR_MATERIAL );
}

/*******************************************************************
 * OpenGL-Funktionen
 */

// initScene initialisiert alles, was von OpenGL benoetigt wird.
void initScene( vector<GLint> &Objekte )
{
  vector<Kante>   kanten;
  vector<Dreieck> dreiecke;

  Vec3D cop;
  Vec3D tgt;
  Vec3D vup(0, 1, 0);

  // Eingabe der Daten
  modellEinlesen(dreiecke, cop, tgt);
  ClipQuad clip;
  clipInfoEinlesen(clip);
  setzeClipRect(clip.uminf, clip.umaxf, clip.vminf, clip.vmaxf);

  Vec3D vrp = lookAt(cop, tgt);

  // Kanten aus den Dreiecken erstellen
  dreieckeToKanten( dreiecke, kanten );

  // Wir wollen senkrecht auf den Ursprung der Bildebene schauen
  Vec3D vpn = cop - vrp;
  double dist = norm(vpn);
  double fov = 90.0;
  double aspect = static_cast<double>(windowSize[0])
                / static_cast<double>(windowSize[1]);
  // In Opengl sind Projektionsebene und vordere Clippingebene ein- und
  // dasselbe; znear kann deshalb hier nicht null sein.
  // Ein zu kleines znear reduziert die z-Buffer Präzision.
  double znear = max(0.5, dist * clip.nearf); //
  double vmax = znear * tan(fov * M_PI / 360.0);
  double vmin = -vmax;
  double umin = vmin * aspect;
  double umax = vmax * aspect;
  double zfar = dist * clip.farf;

  // Anzeige-Einstellungen
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  // Projektionseinstellungen
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();




//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//  Loesung 2:
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  glViewport(floor( 0.5 * windowSize[0] * (1.0 - clip.uminf) ),
             floor( 0.5 * windowSize[1] * (1.0 - clip.vminf) ),
             ceil( windowSize[0] * 0.5 * (clip.uminf + clip.umaxf) ),
             ceil( windowSize[1] * 0.5 * (clip.vminf + clip.vmaxf) ));

  // Frustum (Pyramidenstumpf des Bildraums) berechnen und setzen
  glFrustum(umin * clip.uminf, umax * clip.umaxf,
            vmin * clip.vminf, vmax * clip.vmaxf,
            znear, zfar);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


  // Szeneneinstellungen
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( cop.el[0], cop.el[1], cop.el[2],
             vrp.el[0], vrp.el[1], vrp.el[2],
             vup.el[0], vup.el[1], vup.el[2] );

  // Objekt zeichnen
  beginneObjekt(Objekte);

  glLineWidth( 1.0f );
  maleKanten( kanten );

  beendeObjekt();
}

int main( int argc, char *argv[] )
{
  SDL_Event     event;
  vector<GLint> vObjekte;

  // SDL initialisieren
  initialisiereSDL();

  // Szene initialisieren, Objekte hinzufügen
  initScene(vObjekte);

  // Ereignis-Schleife starten
  while(!finished)
  {
    // Auf event warten
    SDL_WaitEvent(&event);
    // Auf events reagieren, Objekte zeichnen
    behandleEvents(event, vObjekte);
  }

  finalisiereSDL();

  return 0;
}
