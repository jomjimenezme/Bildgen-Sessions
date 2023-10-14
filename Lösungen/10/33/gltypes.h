// Autor:             Holger Arndt
// Version:           0.4
// Modifiziert durch: Sebastian Birk
// Datum:             25.11.2008

#ifndef BIGEN_GLTYPES_H_
#define BIGEN_GLTYPES_H_

#include <iostream>
#include <iomanip>
#include <cmath>

/*************************************************************
 * Klassen-Deklarationen
 */

/// Punkt in der Ebene.
/// Kann auch fuer Vektoren in der Ebene verwendet werden.
template <class T = int>
class Point2D
{
public:
  /// x-Koordinate
  T x;

  /// y-Koordinate
  T y;

  /// Default-Konstruktor: Punkt (0, 0)
  Point2D() : x(0), y(0) {};

  /// Konstruktor fuer einen Punkt (\p x, \p y)
  Point2D(T xx, T yy) : x(xx), y(yy) {};

  /// Kopier-Konstruktor
  Point2D(const Point2D<T> &p2) : x(p2.x), y(p2.y) {};

  /// Typumwandlung
  template <class U> operator Point2D<U>() const
  {
    return Point2D<U>( static_cast<U>(x), static_cast<U>(y) );
  }
};

/// Ein Punkt mit ganzzahligen Koordinaten
typedef Point2D<int> IPoint2D;

/// Ein Punkt mit double-wertigen Koordinaten
typedef Point2D<double> DPoint2D;

// Koordinaten eines Vektors im R^3
struct Vec3D
{
  double el[3];
  Vec3D() { el[0] = el[1] = el[2] = 0; }
  Vec3D(double x, double y, double z)
  {
    el[0] = x;
    el[1] = y;
    el[2] = z;
  }
  Vec3D& operator+=(const Vec3D& v)
  {
    el[0] += v.el[0];
    el[1] += v.el[1];
    el[2] += v.el[2];
    return *this;
  }
};
     
// homogene Koordinaten eines Vektors im R^3 oder Vektor im R^4
struct Vec4D
{
  double el[4];
  Vec4D() { el[0] = el[1] = el[2] = el[3] = 0; }
  Vec4D(double x, double y, double z)
  {
    el[0] = x;
    el[1] = y;
    el[2] = z;
    el[3] = 1;
  }
  Vec4D(double x, double y, double z, double w)
  {
    el[0] = x;
    el[1] = y;
    el[2] = z;
    el[3] = w;
  }
  Vec4D(const Vec3D &v)
  {
    el[0] = v.el[0];
    el[1] = v.el[1];
    el[2] = v.el[2];
    el[3] = 1;
  }    
};

// 4x4-Matrix
// dient auch als Transformationsmatrix fuer 3D-Vektoren mit homogenen Koord.
struct Matrix4x4
{
  double el[4][4];
  Matrix4x4()
  {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        el[i][j] = 0;
  }
};

// Farbe
struct DrawColour
{
  float red;
  float green;
  float blue;

  DrawColour(int r = 0, int g = 0, int b = 0)
  {
    red   = r / 255.0f;
    green = g / 255.0f;
    blue  = b / 255.0f;
  }
};


/// Ausgabe eines Punktes in der Form (2,3)
template <class T>
std::ostream& operator<<( std::ostream& os, const Point2D<T>& p )
{
  os << '(' << p.x << ',' << p.y << ')';
  return os;
}

/// Eingabe eines Punktes in der Form (2,3)
template <class T>
std::istream& operator>>( std::istream& is, Point2D<T>& p )
{
  is.ignore(255, '(');
  is >> p.x;
  is.ignore(255, ',');
  is >> p.y;
  is.ignore(255, ')');
  return is;
}

/// Rundung auf Integer-Punkte
template <class T>
Point2D<int> round( const Point2D<T>& p )
{
  return Point2D<int>(static_cast<int>(round(p.x)),
                      static_cast<int>(round(p.y)));
}

/// Vektor plus Vektor
template <class T>
Point2D<T> operator+( const Point2D<T>& p1, const Point2D<T>& p2 )
{
  return Point2D<T>(p1.x + p2.x, p1.y + p2.y);
}

/// Vektor minus Vektor
template <class T>
Point2D<T> operator-( const Point2D<T>& p1, const Point2D<T>& p2 )
{
  return Point2D<T>(p1.x - p2.x, p1.y - p2.y);
}

/// Skalar mal Vektor
template <class T>
Point2D<T> operator*( T a, const Point2D<T>& p )
{
  return Point2D<T>(a * p.x, a * p.y);
}

/// Vektor mal Skalar
template <class T>
Point2D<T> operator*( const Point2D<T>& p, T a )
{
  return a * p;
}

/// Vektor durch Skalar
template <class T>
Point2D<T> operator/( const Point2D<T>& p, T a )
{
  return Point2D<T>(p.x / a, p.y / a);
}

/// Vektornorm
template <class T>
double norm( const Point2D<T>& p )
{
  return std::sqrt(p.x * p.x + p.y * p.y);
}

/// Vektornorm, Variante fuer int-Punkte
template <>
inline double norm( const Point2D<int>& p )
{
  return std::sqrt(static_cast<double>(p.x * p.x + p.y * p.y));
}

/*************************************************************
 * Vektor-Operationen
 */

Vec3D operator*( double a, const Vec3D& v )
{
  return Vec3D( a * v.el[0], a * v.el[1], a * v.el[2] );
}

Vec3D operator/( const Vec3D& v, double a )
{
  return ( 1 / a ) * v;
}

Vec3D operator+( const Vec3D &v1, const Vec3D &v2 )
{
  return Vec3D( v1.el[0] + v2.el[0], v1.el[1] + v2.el[1], v1.el[2] + v2.el[2] );
}

Vec3D operator-( const Vec3D& v1, const Vec3D& v2 )
{
  return Vec3D( v1.el[0] - v2.el[0], v1.el[1] - v2.el[1], v1.el[2] - v2.el[2] );
}

std::ostream& operator<<( std::ostream& os, const Vec3D& v )
{
  os << '(' << std::setprecision(4) << v.el[0] 
     << ' ' << std::setprecision(4) << v.el[1]
     << ' ' << std::setprecision(4) << v.el[2] << ')';
  return os;
}

double norm( const Vec3D& v )
{
  return sqrt( v.el[0] * v.el[0] + v.el[1] * v.el[1] + v.el[2] * v.el[2] );
}

Vec3D kreuzprod( const Vec3D& v1, const Vec3D& v2 )
{
  return Vec3D( v1.el[1] * v2.el[2] - v1.el[2] * v2.el[1],
                v1.el[2] * v2.el[0] - v1.el[0] * v2.el[2],
                v1.el[0] * v2.el[1] - v1.el[1] * v2.el[0] );
}

std::ostream& operator<<( std::ostream& os, const Vec4D& v )
{
  os << '(' << std::setprecision(4) << v.el[0] 
     << ' ' << std::setprecision(4) << v.el[1]
     << ' ' << std::setprecision(4) << v.el[2] 
     << ' ' << std::setprecision(4) << v.el[3]
     << ')';
  return os;
}

DPoint2D homTo2D( const Vec4D& v )
{
  // nur sinnvoll, wenn durch Projektion bereits v.el[2] == 0 ist
  return DPoint2D(v.el[0] / v.el[3], v.el[1] / v.el[3]);
}

Vec3D homTo3D( const Vec4D& v )
{
  return Vec3D( v.el[0] / v.el[3], v.el[1] / v.el[3], v.el[2] / v.el[3] );
}


/*************************************************************
 * Matrix-Operationen
 */

std::ostream& operator<<( std::ostream& os, const Matrix4x4& t )
{
  for (int i = 0; i < 4; i++)
  {
    for ( int j = 0; j < 4; j++ )
      os << std::setw(9) << t.el[i][j] << ' ';
    os << std::endl;
  }
  return os;
}

Matrix4x4 operator*( const Matrix4x4& t1, const Matrix4x4& t2 )
{
  Matrix4x4 erg;
  int i, j, k;
  for ( i = 0; i < 4; i++ )
    for ( j = 0; j < 4; j++ )
      for ( k = 0; k < 4; k++ )
        erg.el[i][j] += t1.el[i][k] * t2.el[k][j];
  return erg;
}

Vec4D operator*( const Matrix4x4& t, const Vec4D& v )
{
  Vec4D erg;
  int i, k;
  for ( i = 0; i < 4; i++ )
    for ( k = 0; k < 4; k++ )
      erg.el[i] += t.el[i][k] * v.el[k];
  return erg;
}

#endif
