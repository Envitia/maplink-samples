/* -*-c++-*- */
/* Simple Spline
*
* Copyright 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <list>


namespace envitia { namespace MapLink { namespace Geometry {

  struct RIPoint;

  class Spline
  {
  public:
    Spline(const std::list<TSLCoord> &coords, bool ignoreLastPoint)
      : m_coords(coords)
      , m_curve(NULL)
      , m_ignoreLastPoint(ignoreLastPoint)
    {
    }
    ~Spline()
    {
      if (m_curve)
        m_curve->destroy();
    }
    
    TSLCoordSet *curve(bool takeOwnerShip)
    {
      if (!m_curve)
      {
        createPolyline();
      }
      if (takeOwnerShip)
      {
        TSLCoordSet *c = m_curve;
        m_curve = NULL;
        return c;
      }
      return m_curve;
    }
  private:
    bool createPolyline ();
    void calcCurve (RIPoint *controlPoints, int numControls, RIPoint *outputPoints, int numOutput);

    TSLCoordSet *m_curve;
    const std::list<TSLCoord> &m_coords;
    bool m_ignoreLastPoint;
  };

}; }; };

#endif