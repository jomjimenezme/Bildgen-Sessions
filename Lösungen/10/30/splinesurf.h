void berechneZwischenPunkte(vector<vector<Vec3D> >& p)
{
  // berechnet zu p[3i][3j] die fehlenden Zwischenpunkte fuer C^1-Stetigkeit
  int m = p.size() - 1;
  int n = p[0].size() - 1;
  int m3, n3;
  int i, j;
  const double f = 1.0 / 6.0;

  if (m % 3 != 0)
  {
    cerr << "FEHLER: m != 3k+1" << endl;
    exit(1);
  }
  if (n % 3 != 0)
  {
    cerr << "FEHLER: n != 3k+1" << endl;
    exit(1);
  }

  m3 = m / 3;
  n3 = n / 3;

  for (i = 1; i < m3; i++)
    for (j = 0; j <= n3; j++)
    {
      p[3 * i - 1][3 * j] = p[3 * i][3 * j] - f * (p[3 * i + 3][3 * j]
                                                 - p[3 * i - 3][3 * j]);
      p[3 * i + 1][3 * j] = p[3 * i][3 * j] + f * (p[3 * i + 3][3 * j]
                                                 - p[3 * i - 3][3 * j]);
    }
  for (i = 0; i <= m3; i++)
    for (j = 1; j < n3; j++)
    {
      p[3 * i][3 * j - 1] = p[3 * i][3 * j] - f * (p[3 * i][3 * j + 3]
                                                 - p[3 * i][3 * j - 3]);
      p[3 * i][3 * j + 1] = p[3 * i][3 * j] + f * (p[3 * i][3 * j + 3]
                                                 - p[3 * i][3 * j - 3]);
    }
  for (i = 1; i < m3; i++)
    for (j = 1; j < n3; j++)
    {
      p[3 * i - 1][3 * j - 1] =      p[3 * i    ][3 * j - 1]
                              - f * (p[3 * i + 3][3 * j - 1]
                                   - p[3 * i - 3][3 * j - 1]);
      p[3 * i + 1][3 * j - 1] =      p[3 * i    ][3 * j - 1]
                              + f * (p[3 * i + 3][3 * j - 1]
                                   - p[3 * i - 3][3 * j - 1]);
      p[3 * i - 1][3 * j + 1] =      p[3 * i    ][3 * j + 1]
                              - f * (p[3 * i + 3][3 * j + 1]
                                   - p[3 * i - 3][3 * j + 1]);
      p[3 * i + 1][3 * j + 1] =      p[3 * i    ][3 * j + 1]
                              + f * (p[3 * i + 3][3 * j + 1]
                                   - p[3 * i - 3][3 * j + 1]);
    }
  for (i = 0; i <= m; i++)
  {
    p[i][1] = 0.5 * (p[i][0] + p[i][2]);
    p[i][n - 1] = 0.5 * (p[i][n - 2] + p[i][n]);
  }
  for (j = 0; j <= n; j++)
  {
    p[1][j] = 0.5 * (p[0][j] + p[2][j]);
    p[m - 1][j] = 0.5 * (p[m - 2][j] + p[m][j]);
  }
}

vector<vector<Vec3D> > initSurface( int type )
{
  int m, n;
  vector<vector<Vec3D> > p;

  if (type == 0)
  {
    m = n = 12;
    p.resize(m + 1);
    for (int i = 0; i <= m; i++)
      p[i].resize(n + 1);
    for (int i = 0; i <= m; i += 3)
      for (int j = 0; j <= n; j += 3)
        p[i][j] = Vec3D( 100 * i - 600,
                         10 * (i - 6) - 5 * (j - 6),
                         100 * j - 600 );
  }
  else if (type == 1)
  {
    m = n = 12;
    p.resize(m + 1);
    for (int i = 0; i <= m; i++)
      p[i].resize(n + 1);
    for (int i = 0; i <= m; i += 3)
      for (int j = 0; j <= n; j += 3)
        p[i][j] = Vec3D( 100 * i - 600,
                         10 * ((i - 6) * (i - 6) + (j - 6) * (j - 6)) - 400,
                         100 * j - 600 );
  }
  else if (type == 2)
  {
    double r;
    m = n = 18;
    p.resize(m + 1);
    for (int i = 0; i <= m; i++)
      p[i].resize(n + 1);
    for (int i = 0; i <= m; i += 3)
      for (int j = 0; j <= n; j += 3)
      {
        r = sqrt((i - 9.0) * (i - 9.0) + (j - 9.0) * (j - 9.0));
        p[i][j] = Vec3D( 80 * i - 720,
                         4000 / ((r + 4) * (r + 4)) * sin(0.25 - 0.75 * r) + 200,
                         80 * j - 720 );
      }
  }
  else if (type == 3)
  {
    srand48(time(0));
    m = n = 18;
    p.resize(m + 1);
    for (int i = 0; i <= m; i++)
      p[i].resize(n + 1);
    for (int i = 0; i <= m; i += 3)
      for (int j = 0; j <= n; j += 3)
        p[i][j] = Vec3D( 80 * i - 720 + 150 * drand48(),
                         200 * drand48(),
                         80 * j - 720 + 150 * drand48() );
  }
  else
  {
    cerr << "Falsche Eingabe" << endl;
    exit(1);
  }

  // Zwischenpunkte fuer C^1 berechnen
  berechneZwischenPunkte(p);

  return p;
}

void berechneBezierPunkte( vector<Kante>& vk,
                           const vector<vector<Vec3D> >& p)
{
  int m = p.size() - 1;
  int n = p[0].size() - 1;
  int i, j, len;

  // berechne zusatzlich Linien zur Markierung der Kontrollpunkte
  for (i = 0; i <= m; i++)
    for (j = 0; j <= n; j++)
    {
      if (i % 3 == 0 && j % 3 == 0)
        len = 15;
      else
        len = 5;
      vk.push_back(Kante(Vec4D(p[i][j].el[0] - len, p[i][j].el[1],
                               p[i][j].el[2], 1),
                         Vec4D(p[i][j].el[0] + len, p[i][j].el[1],
                               p[i][j].el[2], 1), RED));
      vk.push_back(Kante(Vec4D(p[i][j].el[0], p[i][j].el[1] - len,
                               p[i][j].el[2], 1),
                         Vec4D(p[i][j].el[0], p[i][j].el[1] + len,
                               p[i][j].el[2], 1), RED));
      vk.push_back(Kante(Vec4D(p[i][j].el[0], p[i][j].el[1],
                               p[i][j].el[2] - len, 1),
                         Vec4D(p[i][j].el[0], p[i][j].el[1],
                               p[i][j].el[2] + len, 1), RED));
    }
}
