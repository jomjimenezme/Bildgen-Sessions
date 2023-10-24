#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>

#include "gltypes.h"
#include "glsupport.h"

class Kante              // eine Kante im Raum mit homogenen Koordinaten
{
public:
  Vec4D anf, end;                               // Anfangs- und Endpunkt
  DrawColour col;                                               // Farbe
  Kante() {}
  Kante(const Vec4D& a, const Vec4D& e, const DrawColour& c)
    : anf(a), end(e), col(c) {}
};

const DrawColour RED = DrawColour(255, 0, 0);

using namespace std;

#include "splinesurf.h"

/*******************************************************************
 * Funktionen fuer die Surfaces
 */

void zeichneBezierFlaeche( const vector<vector<Vec3D> >& p,
                           int nMeshSize = 10 )
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  int m = p.size() - 1;
  int n = p[0].size() - 1;

  // Array der Kontrollpunkte fuer OpenGL vorbereiten
  GLfloat *apPoints = new GLfloat[3*4*4];

  // Male die Kurvenabschnitte
  glColor3f( 0.0f, 0.0f, 0.0f );
  for ( int k = 3; k <= m; k += 3 )
    for ( int l = 3; l <= n; l += 3 )
    {
      // Bezierkurve vorbereiten, indem alle 16 Kontrollpunkte des
      // aktuellen Surface-Gebiets an OpenGL uebergeben werden
      for( int i = 0; i < 4; i++ )
        for( int j = 0; j < 4; j++ )
        {
          apPoints[3*(4*i+j)+0] = p[k-3+i][l-3+j].el[0];
          apPoints[3*(4*i+j)+1] = p[k-3+i][l-3+j].el[1];
          apPoints[3*(4*i+j)+2] = p[k-3+i][l-3+j].el[2];
        }

      // glMap2f( target, t1, t2, tstride, torder, s1, s2, sstride, sorder, points )
      glMap2f( GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0.0, 1.0, 3*4, 4, apPoints );
      glEnable( GL_MAP2_VERTEX_3 );

      // glMapGrid2f( tn, t1, t2, sn, s1, s2);
      glMapGrid2f( nMeshSize, 0.0, 1.0, nMeshSize, 0.0, 1.0 );

      // Auswerten des erstellten Meshs
      glColor3f( 0.0f, 0.0f, 0.0f );

      // glEvalMesh2( mode, i1, i2, j1, j2 )
      // Erzeugt glVertex3-Befehle mit entsprechenden Koordinaten.
      glEvalMesh2( GL_LINE, 0, nMeshSize, 0, nMeshSize );

    }

  // Array der Kontrollpunkte wieder freigeben
  delete[] apPoints;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void zeichneKontrollpunkte( const vector<vector<Vec3D> >& p )
{
  glColor3f( 1.0f, 0.0f, 0.0f );

  vector<Kante> vk;
  berechneBezierPunkte(vk, p);

  glBegin( GL_LINES );
  // berechne zusaetzlich Linien zur Markierung der Kontrollpunkte
  for (unsigned int i = 0; i < vk.size(); i++)
    {
      glVertex3f( vk[i].anf.el[0], vk[i].anf.el[1], vk[i].anf.el[2] );
      glVertex3f( vk[i].end.el[0], vk[i].end.el[1], vk[i].end.el[2] );
    }
  glEnd();
}

/*******************************************************************
 * OpenGL-Funktionen
 */

// Sämtliche Zeichenopertaionen hier weden mit der aktuellen
// ModelView-Matrix transformiert.
void objektHinzufuegen(vector<GLint> &Objekte, vector<vector<Vec3D> > p, int nMeshSize)
{
  beginneObjekt(Objekte);

    // Einstellungen
    glLineWidth( 1.0f );

    glScalef(0.01f, 0.01f, 0.01f);

    // Die Bezier-Flaeche zeichnen
    zeichneBezierFlaeche( p, nMeshSize );

    // Die Kontrollpunkte zeichnen
    zeichneKontrollpunkte( p );

  beendeObjekt();
}

int main( int argc, char *argv[] )
{
  SDL_Event     event;
  vector<GLint> vObjekte;

  // SDL initialisieren
  initialisiereSDL();

  // Standard-Surface initialisieren
  int type = 0;
  int nMeshSize = 10;
  vector<vector<Vec3D> > vKontrollpunkte = initSurface( type );

  // Szene initialisieren, Objekte hinzufügen
  standardSzene( 3, 5, 9, 0, 0, 0 );
  objektHinzufuegen(vObjekte, vKontrollpunkte, nMeshSize);

  // Ereignis-Schleife starten
  while ( !finished )
  {
    // auf events reagieren
    SDL_WaitEvent( &event );
    // Auf events reagieren, Objekte zeichnen
    behandleEvents(event, vObjekte);
    // Surface events
    bool changed = false;
    switch ( event.type )
    {
      case SDL_KEYDOWN:
        // Tastatursteuerung
        switch ( event.key.keysym.sym )
        {
          // Auswahl der Surface-Typen
          case SDLK_1:
            type = 0;
            changed = true;
            break;
          case SDLK_2:
            type = 1;
            changed = true;
            break;
          case SDLK_3:
            type = 2;
            changed = true;
            break;
          case SDLK_4:
            type = 3;
            changed = true;
            break;
          // Anpassen des MeshSize-Parameters
          case SDLK_PLUS:
            nMeshSize++;
            changed = true;
            break;
          case SDLK_MINUS:
            nMeshSize--;
            nMeshSize = max( nMeshSize, 1 );
            changed = true;
            break;
          default:
            break;
        }
        break;
    }
    if(changed)
    {
      vKontrollpunkte = initSurface(type);
      objekteFreigeben(vObjekte);
      objektHinzufuegen(vObjekte, vKontrollpunkte, nMeshSize);
    }
  }

  finalisiereSDL();

  return 0;
}
