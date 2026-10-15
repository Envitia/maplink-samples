/* -*-c++-*- */
/* Standard set of Vector Math operators
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#ifndef vector3d_h_
#define vector3d_h_ 1

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_6
#define M_PI_6     0.52359877559829887307
#endif

#ifndef M_PI_3
#define M_PI_3     1.04719755119659774615
#endif


#define AVERYSMALLNUMBER (1e-10)

namespace envitia { namespace MapLink { namespace math {

inline double degreesToRadians(double deg)
{
  return (deg * M_PI/180.0);
}

inline double radiansToDegrees(double rad)
{
  return (rad * 180.0/M_PI);
}

inline int round (double val)
{
  return (val < 0.0) ? (int)(val - 0.5) : (int)(val + 0.5) ;
}

inline void clampValue(double& value, double minVal, double maxVal)
{
  if ( value < minVal )
  {
    value = minVal;
  }
  else if ( value > maxVal )
  {
    value = maxVal;
  }
}


class Vector3D 
{
public:
  Vector3D (const TSLCoord &coord)
    : m_x(coord.m_x),
    m_y(coord.m_y),
    m_z(0.0)
  {
  }
  Vector3D (double x, double y, double z = 0.0);
  Vector3D (const Vector3D &vec);
  Vector3D ();

  Vector3D  & operator = (const Vector3D &vec);

  Vector3D operator - (Vector3D& arg);

  Vector3D operator + (Vector3D& arg);
  Vector3D operator - ();
  Vector3D operator * (Vector3D& arg);
  Vector3D operator * (double value) const;
  Vector3D operator / (double value);
  //	Dot product
  double operator % (Vector3D& arg);
  //	Cross product
  Vector3D operator ^ (Vector3D& arg);
  //	normalize vector
  Vector3D operator ~ ();
  bool operator != (Vector3D& arg);
  bool operator == (Vector3D& arg) const;
  double modulus () const;
  void getVector (double& x, double& y, double& z) const;
  bool operator == ( const Vector3D& arg) const;
  Vector3D operator - ( const Vector3D& arg) const;
  //	Rotate this vector about the vector passed in.
  //
  //	The angle is supplied in degrees.
  void rotate (double angleDegrees, double x, double y, double z);
  //	Rotate this vector about the vector passed in.
  //
  //	The angle is supplied in degrees.
  void rotate (double angleDegrees, Vector3D const& v);

  //	Rotate this vector about the vector passed in.
  //
  //	The angle is supplied in radians.
  void rotateRadians (double angleRadians, double x, double y, double z);

  //	Rotate this vector about the vector passed in.
  //
  //	The angle is supplied in radians.
  void rotateRadians (double angleRadians, Vector3D const& v);

  //	normalises the vector
  void normalise ();

  double dotProduct (const Vector3D& right) const;

  Vector3D crossProduct (const Vector3D& right) const;

  //	Checks to see if the components are close to 0.  If they are they are set to
  //	0.  This gets rid of innaccurcies when transforming.
  void checkForZero ();

  //	Does exactly what it says on the tin !
  bool isZeroLength () const;

  void operator *= (double right);

  void operator += (const Vector3D& right);

  void operator -= (const Vector3D& right);

  double length () const;

  void operator /= (double right);

  //	Rotate a vector of TMS3DVector's by angle (degrees) around a unit vector.
  //static void rotateVector (const double angle, const double x, const double y, const double z, Vector3DArray &points);
  double distance (const Vector3D& other) const;
  double sqDistance (const Vector3D& other) const;
  Vector3D operator + (const Vector3D& right) const;
  double lengthSquared() const
  {
    return m_x*m_x + m_y*m_y + m_z*m_z;
  }

  bool colinear(const Vector3D &p1, const Vector3D & p2, const Vector3D & p3) const
  {
    double slope = (p1.m_y - p2.m_y)  / (p1.m_x - p2.m_x);
    double yIntercept = p1.m_y - slope * p1.m_x;

    if (((p3.m_y - p2.m_y) / (p3.m_x - p2.m_x)) != slope)
      return  false;
    if ((p3.m_y - slope *p3.m_x) != yIntercept)
      return false;
    return true;
  }

  double angleBetween(const Vector3D &p) const 
  {
    Vector3D v1(m_x, m_y);
    Vector3D v2(p);
    v1.normalise();
    v2.normalise();
    double dot = v1.dotProduct(v2);
    double angle = acos(dot); // radians

    //Vector3D axis = v1.crossProduct(v2);
    //axis.normalise();

    return angle;
  }

  double m_x;
  double m_y;
  double m_z;
};

inline Vector3D::Vector3D (double x, double y, double z)
  : m_x(x),
  m_y(y),
  m_z(z)
{

}

inline Vector3D::Vector3D (const Vector3D &vec)
{
  m_x = vec.m_x;
  m_y = vec.m_y;
  m_z = vec.m_z;
}

inline Vector3D::Vector3D ()
  : m_x(0.0),
  m_y(0.0),
  m_z(0.0)
{
}


inline  Vector3D  & Vector3D::operator = (const Vector3D &vec)
{
  if ( this != &vec )
  {
    m_x = vec.m_x;
    m_y = vec.m_y;
    m_z = vec.m_z;
  }
  return *this;
}

inline  Vector3D Vector3D::operator - (Vector3D& arg)
{
  return Vector3D( m_x - arg.m_x, m_y - arg.m_y, m_z - arg.m_z );
}

inline  Vector3D Vector3D::operator + (Vector3D& arg)
{
  return Vector3D( m_x + arg.m_x, m_y + arg.m_y, m_z + arg.m_z );
}

inline  Vector3D Vector3D::operator - ()
{
  return Vector3D(-m_x, -m_y, -m_z);
}

inline  Vector3D Vector3D::operator * (Vector3D& arg)
{
  return Vector3D(m_x * arg.m_x, m_y * arg.m_y, m_z * arg.m_z);
}

inline  Vector3D Vector3D::operator * (double value) const
{
  return Vector3D( m_x * value, m_y * value, m_z * value );
}

inline  Vector3D Vector3D::operator / (double value)
{
  return this->operator *( 1.0 / value );
}

inline  double Vector3D::operator % (Vector3D& arg)
{
  return m_x * arg.m_x + m_y * arg.m_y + m_z * arg.m_z;
}

inline  Vector3D Vector3D::operator ^ (Vector3D& arg)
{
  return Vector3D(m_y*arg.m_z - m_z*arg.m_y, m_z*arg.m_x - m_x*arg.m_z, m_x*arg.m_y - m_y*arg.m_x);
}

inline  Vector3D Vector3D::operator ~ ()
{
  double   l = *this % *this;
  l = sqrt(l);

  return Vector3D(m_x/l, m_y/l, m_z/l);
}

inline  bool Vector3D::operator != (Vector3D& arg)
{
  return ( m_x - arg.m_x < AVERYSMALLNUMBER ) ||
    ( m_y - arg.m_y < AVERYSMALLNUMBER ) ||
    ( m_z - arg.m_z < AVERYSMALLNUMBER ) ;
}

inline  bool Vector3D::operator == (Vector3D& arg) const
{
  return ((m_x == arg.m_x) && (m_y == arg.m_y) && (m_z == arg.m_z));
}

inline  double Vector3D::modulus () const
{
  return (sqrt(m_x*m_x + m_y*m_y + m_z*m_z));
}

inline  void Vector3D::getVector (double& x, double& y, double& z) const
{
  x = m_x;
  y = m_y;
  z = m_z;
}

inline  bool Vector3D::operator == ( const Vector3D& arg) const
{
  return ((m_x == arg.m_x) && (m_y == arg.m_y) && (m_z == arg.m_z));
}

inline  Vector3D Vector3D::operator - ( const Vector3D& arg) const
{
  return Vector3D( m_x - arg.m_x, m_y - arg.m_y, m_z - arg.m_z );
}

inline  void Vector3D::rotate (double angleDegrees, double x, double y, double z)
{
  rotateRadians( degreesToRadians( angleDegrees ), x, y, z );
}

inline  void Vector3D::rotate (double angleDegrees, Vector3D const& v)
{
  rotateRadians( degreesToRadians( angleDegrees ), v );
}

inline  void Vector3D::rotateRadians (double angleRadians, Vector3D const& v)
{
  rotateRadians( angleRadians, v.m_x, v.m_y, v.m_z );
}

inline  void Vector3D::normalise ()
{
  if ( m_x != 0.0 || m_y != 0.0 || m_z != 0.0 )
  {
    double invLen = 1.0 / sqrt( m_x*m_x + m_y*m_y + m_z*m_z );

    m_x *= invLen;
    m_y *= invLen;
    m_z *= invLen;
  }
}

inline  double Vector3D::dotProduct (const Vector3D& right) const
{
  double dot( m_x*right.m_x + m_y*right.m_y + m_z*right.m_z );
  //clampValue( dot, -1.0, 1.0 );

  return dot;
}

inline Vector3D Vector3D::crossProduct (const Vector3D& right) const
{
  return Vector3D( m_y*right.m_z - right.m_y*m_z, m_z*right.m_x - right.m_z*m_x, m_x*right.m_y - right.m_x*m_y );
}

inline  bool Vector3D::isZeroLength () const
{
  return ( m_x == 0.0 ) && ( m_y == 0.0 ) && ( m_z == 0.0 );
}

inline  void Vector3D::operator *= (double right)
{
  m_x *= right;
  m_y *= right;
  m_z *= right;
}

inline  void Vector3D::operator += (const Vector3D& right)
{
  m_x += right.m_x;
  m_y += right.m_y;
  m_z += right.m_z;
}

inline  void Vector3D::operator -= (const Vector3D& right)
{
  m_x -= right.m_x;
  m_y -= right.m_y;
  m_z -= right.m_z;
}

inline  double Vector3D::length () const
{
  return sqrt( m_x * m_x + m_y * m_y + m_z * m_z );
}

inline  void Vector3D::operator /= (double right)
{
  this->operator *=( 1.0 / right );
}

inline double Vector3D::distance (const Vector3D& other) const
{
  return sqrt( sqDistance( other ) );
}

inline double Vector3D::sqDistance (const Vector3D& other) const
{
  double  dx( other.m_x - m_x );
  double  dy( other.m_y - m_y );
  double  dz( other.m_z - m_z );
  return dx*dx + dy*dy + dz*dz;
}

inline Vector3D Vector3D::operator + (const Vector3D& right) const
{
  return Vector3D( m_x + right.m_x, m_y + right.m_y, m_z + right.m_z );
}

#undef AVERYSMALLNUMBER 

}; }; };

#endif
