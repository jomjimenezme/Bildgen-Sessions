// Modellierung mit OpenGL
// Autor: Sebastian Birk, Holger Arndt, Martin Galgon
// Datum: 07.12.2015

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>

#include "gltypes.h"
#include "glsupport.h"

using namespace std;

#ifndef M_PI
const double M_PI = 3.14159265358979323846;
#endif

/*******************************************************************
 * OpenGL-Funktionen
 */

// Male einen Würfel in der angegebenen Farbe
void drawCube(const DrawColour& col)
{
  glEnable(GL_COLOR_MATERIAL);
  glBegin(GL_QUADS);
  glColor3f(col.red, col.green, col.blue);
  glVertex3f(-1.0f,-1.0f,1.0f);  // Vorderseite
  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);  // Rückseite
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);  // links
  glVertex3f(-1.0f,-1.0f,1.0f);
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(-1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,-1.0f);  // rechts
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(1.0f,1.0f,1.0f);
  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(-1.0f,1.0f,-1.0f);  // oben
  glVertex3f(-1.0f,1.0f,1.0f);
  glVertex3f(1.0f,1.0f,1.0f);
  glVertex3f(1.0f,1.0f,-1.0f);
  glVertex3f(-1.0f,-1.0f,-1.0f);  // unten
  glVertex3f(1.0f,-1.0f,-1.0f);
  glVertex3f(1.0f,-1.0f,1.0f);
  glVertex3f(-1.0f,-1.0f,1.0f);
  glEnd();
  glDisable(GL_COLOR_MATERIAL);
}

void drawSnowman()
{
  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}


void drawSnowmen()
{
  // HIER ERGÄNZEN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void initScene()
{
  standardSzene(0, 0, 4, 0, 0, 0);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
}

void objektHinzufuegen(vector<GLint> &Objekte)
{
  beginneObjekt(Objekte);
  drawSnowmen();
  beendeObjekt();
}

int main(int argc, char* argv[])
{
  SDL_Event    event;
  vector<GLint> vObjekte;

  initialisiereSDL();

  // Szene initialisieren
  initScene();
  objektHinzufuegen(vObjekte);

  // Ereignis-Schleife starten
  while (!finished)
    {
      // auf Events reagieren
      SDL_WaitEvent(&event);
      // Auf events reagieren, Objekte zeichnen
      behandleEvents(event, vObjekte);
    }

  finalisiereSDL();

  return 0;
}
