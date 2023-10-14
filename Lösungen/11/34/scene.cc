// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 22.12.2015

#include "scene.h"

namespace rtai {

  string Scene::extension = "scn";

  void Scene::loadFromFile(const string &file)
  {
    // TODO
  }


  // Hintergrundfarbe
  void Scene::setBackground( const Vector3d &col )
  {
    rgbBackground = col;
  }

  void Scene::setBackground( double r, double g, double b )
  {
    rgbBackground[0] = r;
    rgbBackground[1] = g;
    rgbBackground[2] = b;
  }


  // Kamera
  Camera& Scene::getCamera()
  {
    return cam;
  }

  const Camera& Scene::getCamera() const
  {
    return cam;
  }


  // Licht
  void Scene::setAmbient( const Vector3d &col )
  {
    rgbAmbient = col;
  }

  void Scene::setAmbient( double r, double g, double b )
  {
    rgbAmbient[0] = r;
    rgbAmbient[1] = g;
    rgbAmbient[2] = b;
  }

  Vector3d Scene::getAmbient() const
  {
    return rgbAmbient;
  }

  void Scene::addLight( const shared_ptr<Light> &light )
  {
    lights.push_back( light );
  }

  void Scene::addObject( const shared_ptr<Object> &object )
  {
    objects.push_back( object );
  }

}
