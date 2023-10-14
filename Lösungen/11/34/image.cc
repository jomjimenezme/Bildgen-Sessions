// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#include "image.h"

#include <iostream>
#include <utility>

using namespace std;

namespace rtai {

  void Image::saveImage( const string &filename )
  {
    // als PNG speichern
    data.savePNG(filename);
  }

}
