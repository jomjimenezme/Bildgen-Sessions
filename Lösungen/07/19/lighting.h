//======================================================================
// Werte von VecRGB sind als floating point auf [0,1] normiert, nicht
// wie anderswo üblich als integer im Bereich [0,255].
VecRGB berechneBeleuchtung(const Vec3D& ecke, const Vec3D& normale,
                           const Vec3D& auge, const Vec3D& licht,
                           const DrawColour& farbe)
{
  // Explizite Eigenschaften für die Lichtquelle als Vektor
  // weißes Licht für alle drei Arten
  VecRGB lightAmbient(1, 1, 1);
  VecRGB lightDiffuse(1, 1, 1);
  VecRGB lightSpecular(1, 1, 1);

  // Explizite Eigenschaften für das Material als Vektor, normiert auf
  // [0,1]
  VecRGB material(farbe.red() / 255.0, farbe.green() / 255.0,
                  farbe.blue() / 255.0);
  // Etwas dunkler als die vorgegebene Farbe
  VecRGB materialAmbient(0.5 * material);
  // Im Grunde die vorgegebene Farbe
  VecRGB materialDiffuse(0.95 * material);
  // Eine Mischung aus der vorgegebenen Farbe und Weiß
  VecRGB materialSpecular(0.4 * material + 0.3 * VecRGB(1, 1, 1));
  // Exponent für winkelabhängige Reflexion
  double materialSpecularity = 1.2;

  // Konstanten für entfernungsabhängige Dämpfung (attenuation)
  double c0 = 1.0;
  double c1 = 0.00001;
  double c2 = 0.0000004;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  // Blickrichtung
  Vec3D v = auge - ecke;
  Vec3D v1 = v / norm(v);

  // Lichtrichtung
  Vec3D l = licht - ecke;
  double dl = norm(l);
  Vec3D l1 = l / dl;

  // Ambient
  VecRGB ambient = lightAmbient * materialAmbient;

  // Diffus
  double dotNL = skalarprod(normale, l1);
  double lambert = max(0.0, dotNL);
  VecRGB diffuse = lambert * lightDiffuse * materialDiffuse;

  // winkelabhängige Reflexion
  Vec3D s = 2 * (dotNL) * normale - l1;
  Vec3D s1 = s / norm(s);
  double cospsi = max(0.0, skalarprod(v1, s1));
  double cospsiny = pow(cospsi, materialSpecularity);
  VecRGB specular = cospsiny * lightSpecular * materialSpecular;

  // Entfernungsabhängige Dämpfung
  double att = 1.0 / (c0 + c1 * dl + c2 * dl * dl);
  double attenuation = min(1.0, att);

  // Fertige Farbe
  VecRGB total = ambient + attenuation * (diffuse + specular);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // Farbwert auf maximal 1 begrenzen
  total.el[0] = min(total.el[0], 1.0);
  total.el[1] = min(total.el[1], 1.0);
  total.el[2] = min(total.el[2], 1.0);

  return total;
}
//======================================================================
