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
  glBegin(GL_QUADS); //A quad is a 4 vertex quadrilateral primitive.
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
}

void drawSnowmanParts(bool outline)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  DrawColour white(255, 255, 255);
  DrawColour black(0, 0, 0);
  DrawColour orange(255, 45, 45);

  if (outline)
    {
      glPolygonMode(GL_FRONT,GL_LINE);
      glPolygonMode(GL_BACK,GL_LINE);
      glPolygonOffset(2, 2);                          // z-buffer offset
      glLineWidth(1.5);
      white = DrawColour(0, 0, 0);
      black = DrawColour(255, 255, 255);
      orange = DrawColour(0, 0, 0);
    }

  // unterer Würfel
  glPushMatrix();
  glTranslatef(0.0f, 1.0f, 0.0f); //y is oben
  drawCube(white);
  glPopMatrix();

  // mittlerer Würfel
  glPushMatrix();
  glTranslatef(0.0f, 2.8f, 0.0f);
  glScalef(0.8f, 0.8f, 0.8f);
  drawCube(white);
  glPopMatrix();

  // oberer Würfel
  glPushMatrix();
  glTranslatef(0.0f, 4.2f, 0.0f);
  glScalef(0.6f, 0.6f, 0.6f);
  drawCube(white);
  glPopMatrix();

  // Hut
  glPushMatrix();
  glTranslatef(0.0f, 4.9f, 0.0f);
  glScalef(0.8f, 0.2f, 0.8f);
  drawCube(black);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.0f, 5.3f, 0.0f);
  glScalef(0.6f, 0.6f, 0.6f);
  drawCube(black);
  glPopMatrix();

  // Nase
  glPushMatrix();
  glTranslatef(0.0f, 4.2f, 0.8f);
  glScalef(0.1f, 0.1f, 0.4f);
  drawCube(orange);
  glPopMatrix();

  // Augen
  glPushMatrix();
  glTranslatef(0.2f, 4.4f, 0.65f);
  glScalef(0.1f, 0.1f, 0.1f);
  drawCube(black);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-0.2f, 4.4f, 0.65f);
  glScalef(0.1f, 0.1f, 0.1f);
  drawCube(black);
  glPopMatrix();

  // Knöpfe
  glPushMatrix();
  glTranslatef(0.0f, 2.8f, 0.85f);
  glScalef(0.1f, 0.1f, 0.1f);
  drawCube(black);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.0f, 3.2f, 0.85f);
  glScalef(0.1f, 0.1f, 0.1f);
  drawCube(black);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.0f, 2.4f, 0.85f);
  glScalef(0.1f, 0.1f, 0.1f);
  drawCube(black);
  glPopMatrix();

  if (outline)
  {
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_FILL);
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void drawSnowman()
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  drawSnowmanParts(false);  //filling.
  drawSnowmanParts(true);  // Just contour.
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void drawSnowmen()
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  glPushMatrix();

  glScalef(1.5f, 1.5f, 1.5f);

  for (int i = 0; i < 9; ++i)
    {
      glPushMatrix();

      glTranslatef(i % 3 - 1, 0.0f, i / 3 - 1);
      glScalef(0.2f, 0.2f, 0.2f);
      drawSnowman();

      glPopMatrix();
    }

  glPopMatrix();
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void initScene()
{
  standardSzene(0, 0, 4, 0, 0, 0);
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
//   glEnable(GL_CULL_FACE);
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
