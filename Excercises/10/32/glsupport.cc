// Diverse (Hilfs-)Funktionen und -Definitionen für OpenGL
// Autor: Sebastian Birk, Holger Arndt, Martin Galgon
// Datum: 07.12.2015

#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <vector>
#include <signal.h>

#include "glsupport.h"

//======================================================================
cam camera;
GLfloat cBlack[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat cWhite[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
// ClipQuad clip;
bool finished = false;
int windowSize[2] = { 1200, 800 };
//======================================================================
static SDL_Surface* pSurface;
static SDL_Thread* pTimerThread;
static GLint timeStart;
static GLfloat cameraPositionStep = 1.0f;
static GLfloat cameraRotationStep = 5.0f;
static double uminf = 1.0, umaxf = 1.0, vminf = 1.0, vmaxf = 1.0;
static bool clipRectSet = false;
//======================================================================
// Legt fest, wo das Clipping-Rechteck gezeichnet werden soll.
void setzeClipRect(double unf, double uxf, double vnf, double vxf)
{
  uminf = unf;
  umaxf = uxf;
  vminf = vnf;
  vmaxf = vxf;
  clipRectSet = true;
}
//======================================================================
// Erzeugt ein neues Objekt. Alle nachfolgend ausgeführten
// Zeichenoperationen werden dem jeweiligen Objekt zugeordnet.
// Ein Objekt wird abgeschlossen mit beendeObjekt().
void beginneObjekt(std::vector<GLint>& Objekte)
{
  GLint iObjekt = glGenLists( 1 );
  Objekte.push_back( iObjekt );
  glNewList( iObjekt, GL_COMPILE );
}
//======================================================================
// Schließt ein Objekt ab.
void beendeObjekt()
{
  glEndList();
}
//======================================================================
// Gibt alle Objekte frei.
void objekteFreigeben(std::vector<GLint> &Objekte)
{
  for( std::vector<GLint>::const_iterator it = Objekte.begin(); it != Objekte.end(); it++ )
    glDeleteLists( *it, 1 );
  Objekte.clear();
}
//======================================================================
void standardSzene(double copx, double copy, double copz,
                   double tgtx, double tgty, double tgtz)
{
  // Anzeige-Einstellungen
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  glViewport( 0, 0, windowSize[0], windowSize[1] );
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  // Projektionseinstellungen
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  double aspect = static_cast<double>(windowSize[0]) / static_cast<double>(windowSize[1]);
  // Ruft letztendlich glFrustum() auf:
  gluPerspective(90.0, aspect, 0.1, 9999.0);

  // Szeneneinstellungen
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  gluLookAt( copx, copy, copz,
             tgtx, tgty, tgtz,
              0.0,  1.0,  0.0 );
}
//======================================================================
// Löst nach einer festen Anzahl von Millisekunden ein Render-Event aus
int timerThread( void *pDaten )
{
  SDL_Event event;
  static unsigned long int nFrame = 0;
  static unsigned long int nDroppedFrames = 0;

  // Event vorbereiten
  event.type = SDL_USEREVENT;

  // Schleife zum Ausloesen der Render-Events
  while ( !finished )
  {
    nFrame++;

    // Schauen, ob die Message-Queue noch ein Render-Event enthaelt
    if (SDL_PeepEvents( &event, 1, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_USEREVENT))
        != 0 )
    {
      // Den aktuellen Frame verwerfen, da noch ein ungerenderter vorliegt
      nDroppedFrames++;
//       cout << "Frame Nummer " << nFrame
//            << " verworfen - das macht insgesamt " << nDroppedFrames
//            << " Frames" << endl;
    }
    else
      SDL_PushEvent( &event );

    // Schlafen legen fuer 30 ms -> etwa 33 fps
    SDL_Delay( 30 );
  }

  return( 0 );
}
//======================================================================
/* Funktion zum Abfangen von CTRL-C -> Beenden ermoeglichen, falls
 * irgendwas haengt.
 * (Wenn man z.B. Aufruft
 *    ./programm eingabe.in
 *  statt
 *    ./programm < eingabe.in)
 */
void catch_ctrl( int sig )
{
  SDL_Quit();
  exit( 1 );
}
//======================================================================
// Erzeugt ein Fenster via SDL. Erzeug taußerdem einen Thread, der in
// regelmäßigen Abständen Zeichen-Events feuert.
void initialisiereSDL()
{
  (void) signal( SIGINT, catch_ctrl );

  // SDL initialisieren
  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER );

  // Videomodus einstellen und Fenstertitel setzen
  pSurface = SDL_SetVideoMode( windowSize[0], windowSize[1], 24, SDL_OPENGL );
  if ( !pSurface )
  {
    std::cout << "Setzen des Videomodus fehlgeschlagen: " << SDL_GetError();
    SDL_Quit();
    exit( 42 );
  }
  SDL_WM_SetCaption( "Bildgenerierung", "BiGen" );

  // Timer-Thread erstellen
  pTimerThread = SDL_CreateThread( timerThread, NULL );
  if ( !pTimerThread )
  {
    std::cout << "Thread konnte nicht gestartet werden: " << SDL_GetError();
    SDL_Quit();
    exit( 2*42 );
  }

  // Anfangszeit messen
  timeStart = SDL_GetTicks();

  // Tastenwiederholung aktivieren
  SDL_EnableKeyRepeat( 400, 50 );
}
//======================================================================
// Zeichnet das Clipping-Rechteck.
void drawClipRect(double uminf, double umaxf, double vminf, double vmaxf)
{
  glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT);
  glDisable(GL_LIGHTING);

  glViewport( 0, 0, windowSize[0], windowSize[1] );

  glDisable( GL_CLIP_PLANE0   );
  glDisable( GL_CLIP_PLANE0+1 );
  glDisable( GL_CLIP_PLANE0+2 );
  glDisable( GL_CLIP_PLANE0+3 );

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();

  glMatrixMode( GL_MODELVIEW );

  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
    glVertex2f(-uminf, -vminf);
    glVertex2f(-uminf,  vmaxf);
    glVertex2f(-uminf, -vminf);
    glVertex2f( umaxf, -vminf);
  glEnd();

  glTranslatef(0.375 / windowSize[0], 0.375 / windowSize[1], 0);

  glBegin(GL_LINES);
    glVertex2f( umaxf,  vmaxf);
    glVertex2f( umaxf, -vminf);
    glVertex2f(-uminf,  vmaxf);
    glVertex2f( umaxf,  vmaxf);
  glEnd();

  glMatrixMode( GL_PROJECTION );
  glPopMatrix();

  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  glPopAttrib();
}
//======================================================================
// Rendert die im Vektor uebergebenen Objekte. Wird aus dem Event-Loop
// aufgerufen.
void renderScene( const std::vector<GLint> &Objekte, double dfTime )
{
  static double dfLastTime = 0.0f;
  double dfDeltaTime = dfTime - dfLastTime;
  dfLastTime = dfTime;

  // Rotation berechnen
  camera.rotation[0] += camera.deltaRotation[0] * dfDeltaTime;
  camera.rotation[1] += camera.deltaRotation[1] * dfDeltaTime;
  camera.rotation[2] += camera.deltaRotation[2] * dfDeltaTime;

  // Fensterinhalt und Tiefenpuffer zuruecksetzen
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if(clipRectSet) drawClipRect(uminf, umaxf, vminf, vmaxf);

  // Aktuelle Modelview-Matrix speichern und Szene rendern
  glPushMatrix();

    // Bewegen und rotieren
    glTranslatef( -camera.position[0], -camera.position[1], -camera.position[2] );
    glRotatef( camera.rotation[0], 1.0f, 0.0f, 0.0f );
    glRotatef( camera.rotation[1], 0.0f, 1.0f, 0.0f );
    glRotatef( camera.rotation[2], 0.0f, 0.0f, 1.0f );

    // Alle Objekte zeichnen
    for( std::vector<GLint>::const_iterator it = Objekte.begin(); it != Objekte.end(); it++ )
      glCallList( *it );

  glPopMatrix();

  // Frame anzeigen
  SDL_GL_SwapBuffers();
}
//======================================================================
void behandleStandardEvents(const SDL_Event& event, const std::vector<GLint>& vObjekte)
{
  switch ( event.type )
  {
    case SDL_QUIT:
      finished = true;
    case SDL_USEREVENT:
      // zeichnen, falls ein User-Event auftrat
      renderScene( vObjekte, (SDL_GetTicks() - timeStart)/ 1000.0 );
    case SDL_KEYDOWN:
      // Tastatursteuerung
      switch ( event.key.keysym.sym )
        {
         // ESC zum Beenden
         case SDLK_ESCAPE:
           finished = true;
           break;
         default:
           break;
        }
      break;
    default:
      break;
  }
}
//======================================================================
void behandleKeyEvents(int key)
{
  GLdouble mat[16];

  switch ( key )
  {
    // Rotationssteuerung:
    // Die Cursortasten drehen das Objekt, nicht die Kamera
    case SDLK_LEFT:
      camera.deltaRotation[1] -= cameraRotationStep;
      break;
    case SDLK_RIGHT:
      camera.deltaRotation[1] += cameraRotationStep;
      break;
    case SDLK_UP:
      camera.deltaRotation[0] -= cameraRotationStep;
      break;
    case SDLK_DOWN:
      camera.deltaRotation[0] += cameraRotationStep;
      break;
    // Space stoppt die Rotation
    case SDLK_SPACE:
      camera.deltaRotation[0] = 0;
      camera.deltaRotation[1] = 0;
      camera.deltaRotation[2] = 0;
      break;
    // Bewegungssteuerung:
    // w, a, s, d und pageup, pagedown bewegen das Objekt und nicht die Kamera
    case SDLK_w:
      camera.position[2] += cameraPositionStep;
      break;
    case SDLK_s:
      camera.position[2] -= cameraPositionStep;
      break;
    case SDLK_a:
      camera.position[0] += cameraPositionStep;
      break;
    case SDLK_d:
      camera.position[0] -= cameraPositionStep;
      break;
    case SDLK_PAGEUP:
      camera.position[1] += cameraPositionStep;
      break;
    case SDLK_PAGEDOWN:
      camera.position[1] -= cameraPositionStep;
      break;
    // Ausgabe der aktuellen Modelview-Matrix
    case SDLK_m:
      glGetDoublev( GL_MODELVIEW_MATRIX, mat );
      std::cout << "Modelview-Matrix:" << std::endl;
      for( int i = 0; i < 4; i++ ){
        for( int j = 0; j < 4; j++ )
          std::cout << mat[j*4+i] << " ";
        std::cout << std::endl;
      }
      break;
    default:
      break;
  }
}
//======================================================================
void behandleMausEvents(const SDL_Event& event)
{
  switch ( event.type )
  {
    // Maussteuerung:
    // Bei gedrueckter linker Maustaste laesst sich die Rotation steuern
    case SDL_MOUSEMOTION:
      if( event.motion.state & SDL_BUTTON(1) )
      {
        camera.deltaRotation[1] += event.motion.xrel;
        camera.deltaRotation[0] += event.motion.yrel;
      }
      break;
    default:
      break;
  }
}
//======================================================================
// Behandelt alle standard Events.
void behandleEvents(const SDL_Event& event, const std::vector<GLint>& vObjekte)
{
  behandleStandardEvents(event, vObjekte);
  switch ( event.type )
  {
    case SDL_KEYDOWN:
      behandleKeyEvents(event.key.keysym.sym);
      break;
    default:
      break;
  }
  behandleMausEvents(event);
}
//======================================================================
void finalisiereSDL()
{
  // den Timer-Thread und SDL beenden
  SDL_WaitThread( pTimerThread, NULL );
  SDL_Quit();
}
//======================================================================
