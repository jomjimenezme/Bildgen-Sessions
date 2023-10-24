// Raytracer
// Autor: Sebastian Birk, Holger Arndt
// Datum: 21.12.2015

#pragma once

#include <algorithm>
#include <string>

#include <Eigen/Core>

#include <drawing.h>

using namespace std;
using namespace Eigen;


namespace rtai {

  class Image
  {
  public:
    Image()
      : Image(1024,768)
    {};
    Image( int w, int h )
      : width(w), height(h), data(w, h)
    {
      initData();
      data.show();
    };

    inline int getWidth() { return width; }
    inline int getHeight() { return height; }

    inline void setPixel( int x, int y, const Vector3d &rgb ) {
      if (inLimits(x,y)) {
        data.drawPoint(x, height - y - 1, DrawColour(255 * rgb[0], 255 * rgb[1],
                                                     255 * rgb[2]));
      }
    }

    void saveImage( const string &filename );


  private:
    int width;
    int height;
    Drawing data;

    void initData() {
      data = 255;
    }

    inline bool inLimits( int x, int y ) {
      return (x >= 0 && y >= 0
              && x < width && y < height);
    }
  };

}
