void drawEllipse(Drawing& pic, IPoint2D center, int a, int b, bool filled,
                 int colour = 0)
{
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  int x  = 0;
  int y  = b;
  int a2 = a * a;
  int b2 = b * b;
  int d = 4 * a2 * b - 4 * b2 - a2;

  drawEllipsePoints(pic, x, y, center.x, center.y, filled, colour);

  while (a2 * y > b2 * x)
    {
      if (d < 0)
        {
          --y;
          d += 8 * a2 * y - b2 * (8 * x + 12);
        }
      else
        d -= b2 * (8 * x + 12);
      ++x;
      drawEllipsePoints(pic, x, y, center.x, center.y, filled, colour);
    }

  d = 4*b2*a2 - 4*a2*y*y - 4*b2*x*x - 4*b2*x + 4*a2*(2*y-1) - b2;

  while (y >= 0)
    {
      if (d < 0)
        d += a2 * (8 * y - 12);
      else
        {
          ++x;
          d -= 8 * b2 * x - a2 * (8 * y - 12);
        }
      --y;
      drawEllipsePoints(pic, x, y, center.x, center.y, filled, colour);
    }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
