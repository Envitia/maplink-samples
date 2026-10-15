#pragma once

class Utility
{
public:

  //	This method calculates the square distance from the search point coordinate, to the
  //	array of coordinates.
  //	If required, the segment containing the nearest segment will be returned in
  //	retSegCoord1 and retSegCoord2
  static bool squareDistanceToPolyline (const TSLCoord& searchPt, 
                                        const TSLCoordSet& coords, 
                                        int numberOfPoints, 
                                        double& sqDistance, 
                                        TSLCoord*  retSegCoord1 = NULL, 
                                        TSLCoord* retSegCoord2 = NULL)
  {
    sqDistance = DBL_MAX ;

    if ( numberOfPoints < 2 )
      return false ;

    double d;

    for ( int pointCounter = 0; pointCounter < numberOfPoints-1; ++pointCounter)
    {
      const TSLCoord* n = &(coords[pointCounter]);
      const TSLCoord* p = &(coords[pointCounter+1]);

      // Need coordinate set to create polyline 
      TSLCoordSet* coSet = new TSLCoordSet();
      if ( coSet )
      {
        // Add p & n
        coSet->addCoords( p, 2 );
      }
      else
      {
        // Error
        return false;
      }

      // Create Polyline
      TSLPolyline* pline = TSLPolyline::create(0, coSet);
      if ( pline )
      {
        d = pline->squareDistance( searchPt, false );
      }
      else
      {
        // Error
        if ( coSet )
        {
          coSet->destroy();
          coSet = NULL;
        }
        return false;
      }
      if ( d < sqDistance )
      {
        sqDistance = d ;
        if ( retSegCoord1 && retSegCoord2 )
        {
          *retSegCoord1 = *p ;
          *retSegCoord2 = *n ;
        }
      }

      // Clean up
      if ( pline )
      {
        pline->destroy();
        pline = NULL;
      }
      if ( coSet )
      {
        coSet->destroy();
        coSet = NULL;
      }
    }

    return true;
  }

  // Returns the angle between the vertical line and the segment [lower upper].
  // Angle returned is in range 0 - 360, clockwise.
  static double baseLineAngle (const TSLCoord &lower, const TSLCoord &upper)
  {
    double dy;
    double dx;
    double lengthSq;
    double length;
    bool yUpper = false;

    if ( upper.y() > lower.y() )
    {
      dy = (double)upper.y() - (double)lower.y() ;
      dx = (double)upper.x() - (double)lower.x() ;
      lengthSq = dx*dx + dy*dy ;
      length   = sqrt( lengthSq ) ;
    }
    else
    {
      dy = (double)lower.y() - (double)upper.y() ;
      dx = (double)lower.x() - (double)upper.x() ;
      lengthSq = dx*dx + dy*dy ;
      length   = sqrt( lengthSq ) ;
      yUpper = true;
    }

    dx -= 1.0 ;								//generates an horizontal line 
    double horizLengthSq = dx*dx + dy*dy ;  

    if (length == 0)
      return 0;

    //Trigonometric formula:
    //A,B,C are the vertices of a triangle, a is the angle <(AB),(AC)>, then
    //sqrt(BC) = sqrt(AC) + sqrt(AB) - 2*(AB)(BC)cos(a)  
    double val =  (lengthSq + 1.0 - horizLengthSq) / (2.0*length) ;

    // Handle rounding 
    if ( val > 1.0 )
      val = 1.0 ;
    else if ( val < -1.0 )
      val = -1.0 ;

    // Get angle & ensure it is converted radians to degrees
    double angle = ( ( acos( val ) ) * 57.2958 );

    if ( angle <= 90.0 && !yUpper )
    {
      // Quadrant 1
      angle = 90.0 - angle;
    }
    else if ( angle > 90.0 && yUpper )
    {
      // Quadrant 2
      angle = 90 + ( 180.0 - angle );
    }
    else if ( angle <= 90.0 && yUpper )
    {
      // Quadrant 3
      angle = 180.0 + ( 90.0 - angle );
    }
    else if ( angle > 90.0 && !yUpper )
    {
      // Quadrant 4
      angle = 270.0 + ( 180.0 - angle );
    }

    return angle ;
  }


  static bool getLinkDistanceOrientation( 
    const TSLCoord& searchPt, 
    const TSLLink& link, 
    double& retSqDistance, 
    double& retLinkOrientation, 
    TSLCoord*  retSegCoord1 = NULL, 
    TSLCoord* retSegCoord2 = NULL )
  {
    bool result = true;

    // Get the links geometry
    TSLCoordSet* points = link.getPoints();
    if ( points )
    {
      TSLCoord segStartPt;
      TSLCoord segEndPt;
      int numberOfPointsInLink = points->size();

      // Get 
      if ( !Utility::squareDistanceToPolyline( 
              searchPt, 
              *points, 
              numberOfPointsInLink, 
              retSqDistance, 
              &segStartPt, 
              &segEndPt ) )
      {
        // Error
        // Clean up coordinate set before exiting
        points->destroy();
        return false;
      }

      // Set up return Points if valid
      if ( retSegCoord1 && retSegCoord2 )
      {
        *retSegCoord1 = segStartPt;
        *retSegCoord2 = segEndPt;
      }

      // Now need to determine Orientation of this segment
      if ( segStartPt.m_y > segEndPt.m_y )
      {
        retLinkOrientation = Utility::baseLineAngle( segEndPt, segStartPt );
        retLinkOrientation += 180;
      }
      else
      {
        retLinkOrientation = Utility::baseLineAngle( segStartPt, segEndPt );
      }
      // Clean up coordinate set before exiting
      points->destroy();
    }

    return result;
  }


  static bool getShortestDistanceToSegmentWithOrientation( 
     const TSLCoord& searchPt, 
     const TSLLink& link, 
     const double& orientation,
     double& retSqDistance, 
     double& retSegOrientation, 
     double tolerance = 90,
     TSLCoord*  retSegStCoord = NULL, 
     TSLCoord* retSegEndCoord = NULL )
  {
    bool result = true;

    double bestSqDistance = DBL_MAX ;
    double segAngle;
    double sqDistance;

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
        if ( segAngle < 0.0 ) segAngle += 360.0;
        if ( segAngle >= 360.0 ) segAngle -= 360.0;
        if ( fabs(orientation - segAngle) < tolerance )
        {
          // Find distance
          // Need coordinate set to create polyline 
          TSLCoordSet* coSet = new TSLCoordSet();
          if ( coSet )
          {
            // Add p & n
            coSet->addCoords( n, 2 );
          }
          else
          {
            // Error
            return false;
          }

          // Create Polyline
          TSLPolyline* pline = TSLPolyline::create(0, coSet);
          if ( pline )
          {
            sqDistance = pline->squareDistance( searchPt, false );
          }
          else
          {
            // Error
            if ( coSet )
            {
              coSet->destroy();
              coSet = NULL;
            }
            return false;
          }
          // Closer ?
          if ( sqDistance < bestSqDistance )
          {
            // yes, so update best value and any potential return values
            bestSqDistance = sqDistance;
            retSqDistance = bestSqDistance;
            retSegOrientation = segAngle;

            if ( retSegStCoord && retSegEndCoord )
            {
              *retSegStCoord = *n ;
              *retSegEndCoord = *p ;
            }
          }

          // Clean up
          if ( pline )
          {
            pline->destroy();
            pline = NULL;
          }
          if ( coSet )
          {
            coSet->destroy();
            coSet = NULL;
          }
        }
      }// End of for loop of Link Points

      // Clean up coordinate set before exiting
      points->destroy();
    }

    return result;
  }

};
