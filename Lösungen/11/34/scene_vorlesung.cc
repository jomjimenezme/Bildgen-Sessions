// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include <iostream>
#include <string>
#include <fstream>
#include <ios>
#include <vector>
#include <array>
#include <memory>

#include <Eigen/Core>

#include <drawing.h>
#include <maindraw.h>

#include "scene.h"
#include "raytracer.h"
#include "image.h"
#include "objects.h"

using namespace std;

using namespace rtai;


int maindraw()
{
  Scene scene;

  scene.setBackground( Vector3d(.2,.5,.6) );
  scene.setAmbient( Vector3d(.6,.6,.6) );

  scene.getCamera().setPosition( Vector3d(0,2,10) );
  scene.getCamera().lookAt( Vector3d(0,0,-2) );
  scene.getCamera().setHorAngle( 90 );

  shared_ptr<Light> light1( new Light(Vector3d(0,6,5),Vector3d(1,1,1)) );
  light1->setAttenuationCoeff( Vector3d(.5,.001,.004) );
  scene.addLight(light1);

  shared_ptr<Light> light2( new Light(Vector3d(10,2,10),Vector3d(1,1,1)) );
  light2->setAttenuationCoeff( Vector3d(.2,.001,.0004) );
  scene.addLight(light2);

  // Boden
  shared_ptr<Object> plane1( new Plane(Vector3d(0,1,0), 0 ) );
  plane1->setAmbient( Vector3d(0,.2,0) );
  plane1->setDiffuse( Vector3d(.3,.4,.3) );
  plane1->setSpecular( Vector3d(.2,.3,.2) );
  scene.addObject(plane1);

  shared_ptr<Object> sphere1( new Sphere(Vector3d(-1,0,5), 1.5 ) );
  sphere1->setAmbient( Vector3d(.2,0,0) );
  sphere1->setDiffuse( Vector3d(.3,.1,.1) );
  sphere1->setSpecular( Vector3d(.7,.7,.7) );
  scene.addObject(sphere1);

  shared_ptr<Object> sphere3( new Sphere(Vector3d(2,1,5), 1 ) );
  sphere3->setAmbient( Vector3d(0,0,.4) );
  sphere3->setDiffuse( Vector3d(.3,.3,.3) );
  sphere3->setSpecular( Vector3d(.5,.5,.5) );
  sphere3->setSpecularityExponent( 10 );
  sphere3->setReflection( Vector3d(.5,.5,.5) );
  scene.addObject(sphere3);

  shared_ptr<Object> box1( new Box(Vector3d(-2,2,2), Vector3d(11,4,.8), Vector3d(0,30,0) ) );
  box1->setAmbient( Vector3d(.02,.02,.02) );
  box1->setDiffuse( Vector3d(.1,.1,.1) );
  box1->setSpecular( Vector3d(.5,.5,.5) );
  box1->setSpecularityExponent( 10 );
  box1->setReflection( Vector3d(.9,.9,.9) );
  scene.addObject(box1);

  shared_ptr<Object> box2( new Box(Vector3d(3,2,2), Vector3d(11,4,.8), Vector3d(0,-50,0) ) );
  box2->setAmbient( Vector3d(.02,.02,.02) );
  box2->setDiffuse( Vector3d(.1,.1,.1) );
  box2->setSpecular( Vector3d(.5,.5,.5) );
  box2->setSpecularityExponent( 10 );
  box2->setReflection( Vector3d(.9,.9,.9) );
  scene.addObject(box2);

  for (int i = 1; i < 7; ++i) {
    Raytracer rt;
    rt.setDepth(i);
    Image img(1024,768);
    rt.setAntialiasing( true );
    rt.raytrace( scene, img );
    img.saveImage( "scene_vorlesung_d" + std::to_string(i) + ".png" );
  }

  return 0;
}
