void berechneZwischenPunkte(vector<Vec3D>& p)
{
  // berechnet zu p[3i] die fehlenden Zwischenpunkte fuer C^1-Stetigkeit
  int m = p.size() - 1;
  int m3;
  int i;
  const double f = 1.0 / 6.0;

  if (m % 3 != 0)
  {
    cerr << "FEHLER: m != 3k+1" << endl;
    exit(1);
  }

  m3 = m / 3;

  for (i = 1; i < m3; i++)
  {
      p[3 * i - 1] = p[3 * i] - f * (p[3 * i + 3] - p[3 * i - 3]);
      p[3 * i + 1] = p[3 * i] + f * (p[3 * i + 3] - p[3 * i - 3]);
  }
  p[1] = 0.5 * (p[0] + p[2]);
  p[m - 1] = 0.5 * (p[m - 2] + p[m]);
}

vector<Vec3D> initRotationskoerper( int type )
{
  int m;
  vector<Vec3D> p;

  if (type == 0)
  {
    m = 6;
    p.resize(m + 1);
    p[0] = Vec3D(-1200, 900, 0);
    p[3] = Vec3D(0, 900, 0);
    p[6] = Vec3D(1200, 900, 0);
  }
  else if (type == 1)
  {
    m = 9;
    p.resize(m + 1);
    p[0] = Vec3D(-1800, 0, 0);
    p[3] = Vec3D(-1500, 500, 0);
    p[6] = Vec3D(-300, 1000, 0);
    p[9] = Vec3D(1200, 1500, 0);
  }
  else if (type == 2)
  {
    m = 24;
    p.resize(m + 1);
    double theta;
    double R = 1500;
    for (int j = 0; j <= m; j += 3)
    {
      theta = j * M_PI / m;
      p[j] = Vec3D(R * cos(theta), R * sin(theta), 0);
    }
  }
  else if (type == 3)
  {
    srand48(time(0));
    m = 12;
    p.resize(m + 1);
    for (int i = 0; i <= m; i += 3)
      p[i] = Vec3D(300 * i - 2000 + 900 * drand48(), 400 + 900 * drand48(),
                    0);
  }
  else if (type == 4)
  {
    srand48(time(0));
    m = 60;
    p.resize(m + 1);
    for (int i = 0; i <= m; i += 3)
      p[i] = Vec3D(60 * i - 2000 + 300 * drand48(), 400 + 600 * drand48(),
                    0);
  }
  else
  {
    cerr << "Falsche Eingabe" << endl;
    exit(1);
  }

  berechneZwischenPunkte(p);

  return p;
}

void berechneRotationsPunkte( vector<Kante>& vk,
                              const vector<Vec3D>& p)
{
  int m = p.size() - 1;
  int i, len;

  // berechne zusatzlich Linien zur Markierung der Kontrollpunkte sowie
  // die x-Achse
  for (i = 0; i <= m; i++)
  {
    if (i % 3 == 0)
      len = 15;
    else
      len = 5;
    vk.push_back(Kante(Vec4D(p[i].el[0] - len, p[i].el[1], p[i].el[2], 1),
                       Vec4D(p[i].el[0] + len, p[i].el[1], p[i].el[2], 1),
                 RED));
    vk.push_back(Kante(Vec4D(p[i].el[0], p[i].el[1] - len, p[i].el[2], 1),
                       Vec4D(p[i].el[0], p[i].el[1] + len, p[i].el[2], 1),
                 RED));
    vk.push_back(Kante(Vec4D(p[i].el[0], p[i].el[1], p[i].el[2] - len, 1),
                       Vec4D(p[i].el[0], p[i].el[1], p[i].el[2] + len, 1),
                 RED));
  }
  vk.push_back(Kante(Vec4D(p[0].el[0], 0, 0, 1), Vec4D(p[m].el[0], 0, 0, 1),
               RED));
}
