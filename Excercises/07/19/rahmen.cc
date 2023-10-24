// animiertes Nikolaushaus
// Autor: Holger Arndt
// Datum: 07.12.2015

#include <vector>

#include <cppqt.h>

using namespace std;

int maindraw()
{
  vector<Drawing> pics(9);

  Drawing pic(100, 100, 255);
  pic.show();
  pics[0] = pic;
  pic.drawLine(75, 10, 75, 60);
  pics[1] = pic;
  pic.drawLine(75, 60, 50, 95);
  pics[2] = pic;
  pic.drawLine(50, 95, 25, 60);
  pics[3] = pic;
  pic.drawLine(25, 60, 75, 60);
  pics[4] = pic;
  pic.drawLine(75, 60, 25, 10);
  pics[5] = pic;
  pic.drawLine(25, 10, 75, 10);
  pics[6] = pic;
  pic.drawLine(75, 10, 25, 60);
  pics[7] = pic;
  pic.drawLine(25, 60, 25, 10);
  pics[8] = pic;
  
  Drawing::makeanim(pics, "anim01", "gif", 40);
  Drawing::makeanim(pics, "anim01", "mpg");
  
  return 0;
}
