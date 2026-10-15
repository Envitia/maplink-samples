/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
*/

#include "contain.h"
#include "../utilities/simpleArrow.h"
#include "../utilities/vector3d.h"

namespace envitia { namespace MapLink { namespace TacticalGraphics {


bool contain::findPointOnSemicircle( const TSLCoord& startPt, int radius, const double angle, TSLCoord& outPoint )
{
  double dy = radius * sin( angle );
  double dx = radius * cos( angle );

  outPoint.m_x = startPt.m_x + dx;
  outPoint.m_y = startPt.m_y + dy;

  return true;
}


bool contain::findPointOnLine( const TSLCoord& startPt, const TSLCoord& endPoint, TSLCoord& outPoint, int percentage )
{
  /*
  Given the start and endpoints of a line and a percentage of the length, 
  find the point on the line that distance from the start point
  */
  int dx = endPoint.m_x - startPt.m_x;
  int dy = endPoint.m_y - startPt.m_y;

  // Apply percentage to differences
  dx *= percentage;
  dx = dx / 100;
  dy *= percentage;
  dy = dy / 100;

  outPoint.m_x = startPt.m_x + dx;
  outPoint.m_y = startPt.m_y + dy;

  return true;
}

TSLEntitySet* contain::createSpokes( const TSLCoord& centre, const TSLCoord& startOfArc, int radius, double startAngle, double endAngle, int lengthLegs )
{
  /*
  Need to draw NUMBER_OF_SPOKES spokes. Defined as 7.
  From the start of the semicircle, at 30 degree (PI/6) intervals.
  Each spoke is the lengthLegs percentage of the radius.
  */
  
  // Create Entity Set to put spokes in
  TSLEntitySet* spokes = TSLEntitySet::create();
  if ( !spokes )
  {
    return NULL;
  }

  TSLRenderingAttributes atts;
  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_edgeThickness = 1;
  atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  atts.m_edgeStyle = 1;

  double angle = startAngle;
  for ( int numSpokes = 0; numSpokes < NUMBER_OF_SPOKES; ++numSpokes )
  {
    TSLCoord spokeStart;
    TSLCoord spokeEnd;
    // Find point on Semicircle
    findPointOnSemicircle( centre, radius, angle, spokeStart );

    // Then find point along the line defined by the semicircle point and the centre point
    findPointOnLine( spokeStart, centre, spokeEnd, lengthLegs );

    // Create Spoke Polyline
    TSLCoordSet* spokeCoords = new TSLCoordSet();
    spokeCoords->add( spokeStart );
    spokeCoords->add( spokeEnd );
    TSLPolyline* spoke = spokes->createPolyline( 0, spokeCoords, true );
    if ( !spoke )
    {
      return NULL;
    }
	spoke->setRendering( atts );
    // Move to next point along the semicircle
    angle += M_PI_6;
  }

  return spokes;
}


// Quadrant values 1 - 4 CW from Top right
// A return value of zero denotes error
int contain::determineQuadrant( const TSLCoord& pt1, const TSLCoord& pt2 )
{
  int quadrant = 0;

  // Calculate translation values dx, dy for Pt1 to be origin (0,0)
  int dx = -1 * pt1.m_x;
  int dy = -1 * pt1.m_y;

  // Apply translation values to Pt2
  TSLCoord pt2Translated( pt2.m_x, pt2.m_y );
  pt2Translated.translate( dx, dy );

  // Check values of Pt2Translated to determine quadrant 
  if ( pt2Translated.m_x >= 0 && pt2Translated.m_y >= 0 )
  {
    quadrant = 1;
  }
  else if ( pt2Translated.m_x >= 0 && pt2Translated.m_y <= 0 )
  {
    quadrant = 2;
  }
  else if ( pt2Translated.m_x <= 0 && pt2Translated.m_y <= 0 )
  {
    quadrant = 3;
  }
  else if ( pt2Translated.m_x <= 0 && pt2Translated.m_y >= 0 )
  {
    quadrant = 4;
  }

  return quadrant;
}

bool contain::checkParameters( const TSLCoordSet &positions, int& lengthLegs )
{
  // Check lengths within specified range 0 - 100
  if ( lengthLegs < 0 || lengthLegs > 100 )
  {
    // Error lengths outside percentage range
    return false;
  }

  // Check two positions passed
  if ( positions.size() != 2 )
  {
    // Error Two Anchor points required
    return false;
  }
  return true;
}


TSLEntitySet* contain::create( const TSLCoordSet &positions, int lengthLegs )
{
  if ( !checkParameters( positions, lengthLegs ) )
  {
    // Error invalid parameters
    return NULL;
  }

  // Default Spoke leg length
  if ( 0 == lengthLegs )
  {
    lengthLegs = DEFAULT_SPOKE_PERCENTAGE;
  }

  // Determine radius
  TSLCoord pt1 = positions[0]; // Centre
  TSLCoord pt2 = positions[1]; // 
  // X and Y lengths 
  double dx = pt2.x() - pt1.x();
  double dy = pt2.y() - pt1.y();
  double radius = ( sqrt( dx * dx + dy * dy ) );

  // Determine Quadrant
  int quadrant = determineQuadrant( pt1, pt2 );

  // Determine Start & End Angles
  double startAngle = 0.0;
  double endAngle = 0.0;
  double sinStartAngle = 0.0;
  double workAngle = 0.0;

  // Arc implementation uses +ve ACW from the horizontal X-axis
  sinStartAngle = dy / radius;
  workAngle = asin( sinStartAngle );
  switch ( quadrant )
  {
  case 1:
    endAngle = workAngle; 
    startAngle = M_PI + workAngle;
    break;
  case 2:
    endAngle = workAngle;
    startAngle = workAngle - M_PI;
    break;
  case 3:
    endAngle = ( -1.0 * workAngle ) + M_PI;
    startAngle = ( -1.0 * workAngle );
    break;
  case 4:
    endAngle = ( -1.0 * workAngle ) - M_PI;
    startAngle = ( -1.0 * workAngle );
    break;
  default:
    return NULL;
    break;
  }

  // Create Semicircle
  TSLArc* semiCircle = TSLArc::create( 0, startAngle, endAngle, pt1.m_x, pt1.m_y, radius );
  if ( !semiCircle )
  {
    // Error creating semicircle
    return NULL;
  }

  TSLRenderingAttributes atts;
  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_edgeThickness = 1;
  atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  atts.m_edgeStyle = 1;
  semiCircle->setRendering( atts );

  // Now create spokes
  TSLEntitySet* spokes = createSpokes( pt1, pt2, radius, startAngle, endAngle, lengthLegs );
  if ( !spokes )
  {
    // Error creating spokes
    return NULL;
  }

  // Create master Entity Set to contain graphic elements
  TSLEntitySet *groupMaster = TSLEntitySet::create();
  if ( !groupMaster )
  {
    return NULL;
  }

  // Add elements
  if ( !groupMaster->insert( semiCircle ) || !groupMaster->insert( spokes ) )
  {
    return NULL;
  }

  // Success return master entity set 
  return groupMaster;
}





} // end TacticalGraphics namespace
} // end MapLink namespace
} // end Envitia namespace