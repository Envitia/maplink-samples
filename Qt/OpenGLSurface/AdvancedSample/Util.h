/****************************************************************************
Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef UTIL_H
#define UTIL_H

class Util
{
public:
  static void normaliseAngle( double &x )
  {
    if ( x < -180.0 )
    {
      x += 360.0;
    }
    else if ( x > 180.0 )
    {
      x -= 360.0;
    }
  }

  static void normaliseLatLon( double &lat, double &lon )
  {
    if ( lat > 90.0 )
    {
      lat -= 180.0;
    }
    else if ( lat < -90.0 )
    {
      lat += 180.0;
    }

    if ( lon < -180.0 )
    {
      lon += 360.0;
    }
    else if ( lon > 180.0 )
    {
      lon -= 360.0;
    }
  }

  static double sign( double num )
  {
    return num > 0.0 ? 1.0 : -1.0;
  }

private:
  Util();   // not implemented
  ~Util();  // not implemented
};

#endif
