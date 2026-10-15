/* -*-c++-*- */
/* Simple Spline
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#define _USE_MATH_DEFINES
#include <cmath>

#include "MapLink.h"
#include "vector3d.h"
#include "spline.h"


namespace envitia { namespace MapLink { namespace Geometry {

  struct RIPoint
  {
    double	m_x;
    double	m_y;
  };


  bool Spline::createPolyline ()
  {
    const int size = m_ignoreLastPoint ? m_coords.size() - 1 : m_coords.size();
    if ( size <= 1 )
    {
      return false ;
    }
    const int startIndex = 0;

    // Convert the coordset into a point array
    std::list<TSLCoord>::const_iterator it(m_coords.begin());

    RIPoint * controlPoints = new RIPoint[size] ;
    for (int i = startIndex, j = 0; i < size; ++i, ++j)
    {
      controlPoints[j].m_x = (*it).x() ;
      controlPoints[j].m_y = (*it).y() ;
      ++it;
    }

    // Use spline implementation to get output points
    const int numOutputPoints = size * 12 ;
    RIPoint * outputPoints = new RIPoint[numOutputPoints] ;

    // Call implementation
    // Should make this virtual to allow different implementations
    calcCurve( controlPoints, size, outputPoints, numOutputPoints ) ;

    // Pass the rounded points into a coordset
    if (m_curve)
      m_curve->destroy();
    m_curve = new TSLCoordSet;

    for ( int i = 0; i < numOutputPoints; ++i )
    {
      m_curve->add( math::round(outputPoints[i].m_x),  math::round(outputPoints[i].m_y) ) ;
    }

    // Tidy up
    delete [] controlPoints ;
    delete [] outputPoints ;

    return true;
  }

  void Spline::calcCurve (RIPoint *controlPoints, int numControls, RIPoint *outputPoints, int numOutput)
  {
    ////////////////////////////////////////////////////////////
    // Catmull-Rom Spline

    const double M[4][4] =
    {
      {0  ,   1,   0,   0},
      {-.5,   0,   .5,   0},
      {1  ,-2.5,   2,-0.5},
      {-.5, 1.5,-1.5, 0.5}
    };

    const double a = 0.5;

    const double Mzero[4][4] =
    {
      {0,1,0,0},
      {0,-a,a,0},
      {0,3*a-3,3-2*a,-a},
      {0,2-2*a,a-2,a}
    };

    const double Mnminus1[4][4] =
    {
      {0,1.0,0,0},
      {-a,0,a,0},
      {2.0*a,a-3.0,3.0-3.0*a,0},
      {-a,2.0-a,2.0*a-2.0,0}
    };

    for (int output=0; output < numOutput; output++)
    {
      int numSegments = numControls - 1;
      // figure out which segment of curve we're on and how far along it
      // each segment is 1 Long.
      double position = ((double)numSegments * (double)output) / (double)( numOutput - 1 );

      // so the integer bit gives us the segment number and the fractional bit
      // gives the distance along it

      int segment = (int) position;

      // t parameter for the segment we're on:
      double t = position - (double)segment;

      if( segment != 0 && t == 0.0)
      {
        segment--;
        t = 1.0;
      }


      const double (*m)[4][4];

      RIPoint *P[4];

      if( segment == 0 )
      {
        // initial special case
        m = &Mzero;

        P[0] = &controlPoints[segment];	// doesn't matter what's in here since the matrix multiplication doesn't use it
        P[1] = &controlPoints[segment];
        P[2] = &controlPoints[segment+1];
        P[3] = &controlPoints[segment+2];	
      }
      else if(segment == numSegments -1 )
      {
        // ending special case
        m = &Mnminus1;

        P[0] = &controlPoints[segment-1];
        P[1] = &controlPoints[segment];
        P[2] = &controlPoints[segment+1];
        P[3] = &controlPoints[segment+1];		// doesn't matter what's in here since the matrix multiplication doesn't use it
      }
      else
      {
        // general case
        m = &M;

        P[0] = &controlPoints[segment-1];
        P[1] = &controlPoints[segment];
        P[2] = &controlPoints[segment+1];
        P[3] = &controlPoints[segment+2];	
      }

      double T[4] = { 1, t, t*t, t*t*t};

      // do the matrix multiplication:
      // NB this could be optimized...
      //TODO("optimise matrix multiplication")
      double sumx = 0, sumy = 0;

      for(int i=0;i<4;++i)
      {
        for(int j=0;j<4;++j)
        {
          sumx += T[i] * (*m)[i][j] * P[j]->m_x;
          sumy += T[i] * (*m)[i][j] * P[j]->m_y;
        }
      }

      outputPoints[output].m_x = sumx ; 
      outputPoints[output].m_y = sumy ; 
    }
  }


}; }; };