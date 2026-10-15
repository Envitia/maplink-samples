#pragma once
#include "Utility.h"

class OrientationFilter :
  public TSLNwkFinder
{
public:

  OrientationFilter( const TSLCoord& searchPoint, double orientation, double orientationTolerance = 90 )
    : m_searchPoint( searchPoint ),
      m_orientationTolerance( orientationTolerance )
  {
    constrainAngle( orientation );
    m_orientation = orientation;
  }

  ~OrientationFilter(void)
  {
  }

  // Not implemented for Nodes
  bool operator()(const TSLNode& node) const
  {
    return false;
  }


  // Link implementation
  bool operator()(const TSLLink& link) const
  {
    bool result = false;
    
    double segAngle;

    // Get the links geometry
    TSLCoordSet* points = link.getPoints();
    if ( points )
    {
      int numberOfPointsInLink = points->size();

      // Check we have some points
      if ( numberOfPointsInLink < 2 )
        return false ;

      // Loop thru points
      for ( int pointCounter = 0; pointCounter < numberOfPointsInLink-1; ++pointCounter)
      {
        // Get first two points
        const TSLCoord* n = &((*points)[pointCounter]);
        const TSLCoord* p = &((*points)[pointCounter+1]);

        // Check Orientation of this segment first
        if ( n->m_y > p->m_y )
        {
          segAngle = Utility::baseLineAngle( *p, *n );
          segAngle += 180;
        }
        else
        {
          segAngle = Utility::baseLineAngle( *n, *p );
        }

        // Check within tolerance
        if ( angleWithinTolerance( segAngle ) )
        {
          // Set return value
          result = true;
          break;
        }
      }// End of for loop of Link Points

      // Clean up coordinate set before exiting
      points->destroy();
    }

    return result;
  }

private: // Functions

  bool angleWithinTolerance( double inputAngle ) const
  {
    constrainAngle( inputAngle );
    return fabs(m_orientation - inputAngle) < m_orientationTolerance;
  }

  // Ensures angle is within range =/-180.
  void constrainAngle( double& angle ) const
  {
    if ( angle < 0.0 ) angle += 360.0;
    if ( angle >= 360.0 ) angle -= 360.0;
  }

private: // Data

  // Search Position
  const TSLCoord& m_searchPoint;

  // Orientation of Search Entity
  // Value in degrees 0 - 360.
  double 	m_orientation;

  // Tolerance on Orientation
  // Value in degrees 0 - 180.
  // Default is +/- 90 degrees
  const double m_orientationTolerance;

};
