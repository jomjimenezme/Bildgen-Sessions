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
  scene.setAmbient( Vector3d(1,1,1) );

  scene.getCamera().setPosition( Vector3d(0,2,10) );
  scene.getCamera().lookAt( Vector3d(0,0,-2) );
  scene.getCamera().setHorAngle( 90 );

  shared_ptr<Light> light1( new Light(Vector3d(0,6,3),Vector3d(1,1,1)) );
  light1->setAttenuationCoeff( Vector3d(.5,.001,.004) );
  scene.addLight(light1);

  shared_ptr<Light> light2( new Light(Vector3d(10,2,10),Vector3d(1,1,1)) );
  light2->setAttenuationCoeff( Vector3d(.2,.001,.0004) );
  scene.addLight(light2);

  shared_ptr<Object> plane1( new Plane(Vector3d(0,1,0), 0 ) );
  plane1->setAmbient( Vector3d(0,.2,0) );
  plane1->setDiffuse( Vector3d(.3,.4,.3) );
  plane1->setSpecular( Vector3d(.2,.3,.2) );
  scene.addObject(plane1);

  shared_ptr<Object> sphere1( new Sphere(Vector3d(0,0,0), 2 ) );
  sphere1->setAmbient( Vector3d(.2,0,0) );
  sphere1->setDiffuse( Vector3d(.3,.1,.1) );
  sphere1->setSpecular( Vector3d(.7,.7,.7) );
  scene.addObject(sphere1);

  shared_ptr<Object> sphere3( new Sphere(Vector3d(2,1,5), 1 ) );
  sphere3->setAmbient( Vector3d(0,0,0) );
  sphere3->setDiffuse( Vector3d(.3,.3,.3) );
  sphere3->setSpecular( Vector3d(.5,.5,.5) );
  sphere3->setSpecularityExponent( 10 );
  sphere3->setRefraction( 1.9 );
  scene.addObject(sphere3);

  //  shared_ptr<Object> sphere7( new Sphere(Vector3d(2,.5,5), .5 ) );
  //  sphere7->setAmbient( Vector3d(.05,.05,.05) );
  //  sphere7->setDiffuse( Vector3d(.1,.1,.1) );
  //  sphere7->setSpecular( Vector3d(.5,.5,.5) );
  //  sphere7->setSpecularityExponent( 10 );
  //  sphere7->setRefraction( 1.5 );
  //  sphere7->setPigment( Vector3d(1,1,1) );
  //  scene.addObject(sphere7);

  shared_ptr<Object> box1( new Box(Vector3d(0,5,-10), Vector3d(3,10,3),
                                   Vector3d(0,0,0) ) );
  box1->setAmbient( Vector3d(.02,.02,.02) );
  box1->setDiffuse( Vector3d(.1,.1,.1) );
  box1->setSpecular( Vector3d(.5,.5,.5) );
  box1->setSpecularityExponent( 10 );
  scene.addObject(box1);

  shared_ptr<Object> box2( new Box(Vector3d(0,3,3), Vector3d(7,1.5,.8),
                                   Vector3d(0,30,0) ) );
  box2->setAmbient( Vector3d(.02,.02,.02) );
  box2->setDiffuse( Vector3d(.1,.1,.1) );
  box2->setSpecular( Vector3d(.5,.5,.5) );
  box2->setSpecularityExponent( 10 );
  box2->setRefraction( 1.5 );
  box2->setPigment( Vector3d(.5,.5,.5) );
  scene.addObject(box2);

  shared_ptr<Object> tet1( new Tetraeder(Vector3d(4.5,1,1.5), 3,
                                         Vector3d(0,50,0) ) );
  tet1->setAmbient( Vector3d(.02,.02,.32) );
  tet1->setDiffuse( Vector3d(.1,.1,.1) );
  tet1->setSpecular( Vector3d(.5,.5,.5) );
  tet1->setSpecularityExponent( 10 );
  tet1->setReflection( Vector3d(.6,.6,.6) );
  scene.addObject(tet1);

  Vector3d rot(-.7,2,-.7);
  rot = 80.0*rot/rot.norm();
  shared_ptr<Object> tet2( new Tetraeder(Vector3d(-1,1.3,6), 1.5, rot ) );
  tet2->setAmbient( Vector3d(.0,.0,.0) );
  tet2->setDiffuse( Vector3d(.0,.0,.0) );
  tet2->setSpecular( Vector3d(.5,.5,.5) );
  tet2->setSpecularityExponent( 10 );
  tet2->setReflection( Vector3d(.2,.2,.2) );
  tet2->setRefraction( 1.5 );
  tet2->setPigment( Vector3d(.9,0,0) );
  scene.addObject(tet2);

  Raytracer rt;
  Image img(1024,768);
  rt.setAntialiasing( true );
  rt.raytrace( scene, img );
  img.saveImage( "scene_tests.png" );

  return 0;
}
