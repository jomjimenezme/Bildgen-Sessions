void maleHermiteKurve( Drawing& pic, DPoint2D p1, DPoint2D p4, DPoint2D r1,
                       DPoint2D r4, int n, bool slow = false )
{
  double cx[4], cy[4];
  DPoint2D anf, end;
  double t;
  double ninv = 1.0 / n;
  int i;

  pic.drawCircle(p1, 3, true, BLUE, BLUE, slow);
  pic.drawCircle(p4, 3, true, BLUE, BLUE, slow);
  pic.drawLine(p1, p1 + r1, RED, slow);
  pic.drawLine(p4, p4 + r4, RED, slow);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


  cx[0] = 2 * p1.x - 2 * p4.x + r1.x + r4.x;
  cx[1] = -3 * p1.x + 3 * p4.x - 2 * r1.x - r4.x;
  cx[2] = r1.x;
  cx[3] = p1.x;
  cy[0] = 2 * p1.y - 2 * p4.y + r1.y + r4.y;
  cy[1] = -3 * p1.y + 3 * p4.y - 2 * r1.y - r4.y;
  cy[2] = r1.y;
  cy[3] = p1.y;

  end.x = cx[3]; //p1.x
  end.y = cy[3]; //p1.y
  for (i = 1; i <= n; i++)
  {
    t = ninv * i;
    anf = end;
    end.x = ((cx[0] * t + cx[1]) * t + cx[2]) * t + cx[3];
    end.y = ((cy[0] * t + cy[1]) * t + cy[2]) * t + cy[3];
    pic.drawLine(round(anf), round(end), 0, slow);
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
