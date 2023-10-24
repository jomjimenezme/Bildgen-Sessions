// Rotationskörper (OpenGL)
// Autor: Sebastian Birk, Martin Galgon
// Datum: 15.01.2009

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>

#include "glsupport.h"
#include "gltypes.h"

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

struct RotkDaten
{
  int anzkurv;
  int anzlinku;
  int anzkreis;
  int anzlinkr;
};

#include "splinerotk.h"

/*******************************************************************
 * Funktionen fuer den Rotationskoerper
 */

void zeichneRotationskoerper( const vector<Vec3D>& p,
                              RotkDaten daten )
{
  // HIER ERGAENZEN
}

void zeichneKontrollpunkte( const vector<Vec3D>& p )
{
  glColor3f( 1.0f, 0.0f, 0.0f );

  vector<Kante> vk;
  berechneRotationsPunkte(vk, p);

  glBegin( GL_LINES );
  // berechne zusaetzlich Linien zur Markierung der Kontrollpunkte
  for (unsigned int i = 0; i < vk.size(); i++)
    {
      glVertex3f( vk[i].anf.el[0], vk[i].anf.el[1], vk[i].anf.el[2] );
      glVertex3f( vk[i].end.el[0], vk[i].end.el[1], vk[i].end.el[2] );
    }

  glVertex3f( p[         0].el[0], 0.0f, 0.0f );
  glVertex3f( p[p.size()-1].el[0], 0.0f, 0.0f );

  glEnd();
}

/*******************************************************************
 * OpenGL-Funktionen
 */

void objektHinzufuegen(vector<GLint> &Objekte, const vector<Vec3D> &p, RotkDaten daten)
{
  beginneObjekt(Objekte);

    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.01f, 0.01f, 0.01f);

    // Einstellungen
    glLineWidth( 1.0f );

    // Den Rotationskoerper zeichnen
    zeichneRotationskoerper( p, daten );

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

  // Szene initialisieren
  standardSzene(2, 6, 40, 1, 3, 20);

  // Standard-Surface initialisieren
  int type = 0;
  RotkDaten daten = { 10, 20, 5, 40 };
  vector<Vec3D> vKontrollpunkte = initRotationskoerper( type );

  objektHinzufuegen(vObjekte, vKontrollpunkte, daten);

  // Ereignis-Schleife starten
  while ( !finished )
  {
    // auf events reagieren
    SDL_WaitEvent( &event );
    // Auf events reagieren, Objekte zeichnen
    behandleEvents(event, vObjekte);

    bool changed = false;
    switch ( event.type )
    {
      case SDL_KEYDOWN:
        // Tastatursteuerung
        switch ( event.key.keysym.sym )
        {
          // Auswahl der Rotationskoerper-Typen
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
          case SDLK_5:
            type = 4;
            changed = true;
            break;
          // Anpassen der Parameter
          case SDLK_u:
            daten.anzkurv++;
            changed = true;
            break;
          case SDLK_j:
            daten.anzkurv--;
            daten.anzkurv = max( daten.anzkurv, 1 );
            changed = true;
            break;
          case SDLK_i:
            daten.anzlinku++;
            changed = true;
            break;
          case SDLK_k:
            daten.anzlinku--;
            daten.anzlinku = max( daten.anzlinku, 1 );
            changed = true;
            break;
          case SDLK_o:
            daten.anzkreis++;
            changed = true;
            break;
          case SDLK_l:
            daten.anzkreis--;
            daten.anzkreis = max( daten.anzkreis, 0 );
            changed = true;
            break;
          case SDLK_z:
            daten.anzlinkr++;
            changed = true;
            break;
          case SDLK_h:
            daten.anzlinkr--;
            daten.anzlinkr = max( daten.anzlinkr, 1 );
            changed = true;
            break;
          default:
            break;
        }
        break;
    }
    if(changed)
    {
      vKontrollpunkte = initRotationskoerper(type);
      objekteFreigeben(vObjekte);
      objektHinzufuegen(vObjekte, vKontrollpunkte, daten);
    }
  }

  finalisiereSDL();

  return 0;
}
