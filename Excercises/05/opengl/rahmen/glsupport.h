// diverse (Hilfs-)Funktionen und -Definitionen für OpenGL
// Autor: Sebastian Birk, Holger Arndt, Martin Galgon
// Datum: 07.12.2015

#ifndef BIGEN_GLSUPPORT_H_
#define BIGEN_GLSUPPORT_H_

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <vector>

struct cam
{
  GLfloat deltaRotation[3] = { 0.0f, 0.0f, 0.0f };
  GLfloat rotation[3] = { 0.0f, 0.0f, 0.0f };
  GLfloat position[3] = { 0.0f, 0.0f, 0.0f };
};

extern cam camera;
extern GLfloat cBlack[4];
extern GLfloat cWhite[4];
extern bool finished;
extern int windowSize[2];
/*******************************************************************
 * Funktionen
 */

void beginneObjekt(std::vector<GLint> &Objekte);
void beendeObjekt();
void objekteFreigeben(std::vector<GLint> &Objekte);
void initialisiereSDL();
void standardSzene(double copx, double copy, double copz,
                   double tgtx, double tgty, double tgtz);
void behandleStandardEvents(const SDL_Event& event, const std::vector<GLint>& vObjekte);
void behandleKeyEvents(int key);
void behandleMausEvents(const SDL_Event& event);
void behandleEvents(const SDL_Event& event, const std::vector<GLint>& Objekte);
void finalisiereSDL();
void setzeClipRect(double unf, double uxf, double vnf, double vxf);

#endif
