#include <iostream>
#include <cmath>

#include <cppqt.h>

using namespace std;

int maindraw()
{
  Drawing pic( 400, 300 );
  pic.show();

  IPoint2D m;
  int r;

  bool bWeiter = true;

  while(bWeiter)
  {
    cout << "Eingabe des Mittelpunktes (x,y): ";
    cin >> m;
    cout << "Eingabe des Radius (integer): ";
    cin >> r;

    bWeiter = (m.x >= 0) && (m.y >= 0) && (r >= 0);

    // Durchlaufe das umschliessende Quadrat und faerbe den Punkt dann ein,
    // wenn er sich im inneren des Kreises befindet.
    if( bWeiter )
    for( int x = -r; x <= r; ++x )
      for( int y = -r; y <= r; ++y )
        if( x*x + y*y <= r*r )
          pic.drawPoint( m.x + x, m.y + y, DrawColour(0,0,0) );
  };

  cout << endl;
  IOThread::waitForWindow(60);

  return 0;
}


/* 

(200, 150) 400
(200, 150) 150

(130, 190) 50
(280, 190)  50

(130, 190) 10
(280, 190)  10

(200, 60) 30

 * */
