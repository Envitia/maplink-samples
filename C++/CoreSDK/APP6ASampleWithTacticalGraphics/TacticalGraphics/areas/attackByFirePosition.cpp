/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
*/

#include "attackByFirePosition.h"
#include "../utilities/simpleArrow.h"
#include "../utilities/vector3d.h"

namespace envitia { namespace MapLink { namespace TacticalGraphics {



bool attackByFirePosition::checkParameters( const TSLCoordSet &positions, int& length, int& angle, int& lengthLegs )
{
  // Check lengths within specified range 0 - 100
  if ( length < 0 || length > 100|| lengthLegs < 0 || lengthLegs > 100 )
  {
    // Error lengths outside percentage range
    return false;
  }

  // Check three positions passed
  if ( positions.size() != 3 )
  {
    // Error Three Anchor points required
    return false;
  }
  return true;
}

TSLEntitySet* attackByFirePosition::createArrowheadSet( const TSLCoordSet& coords, const int length, const int angle )
{
  // Create Arrowhead
  simpleArrow* arrow = new simpleArrow();
  if ( !arrow )
  {
    return NULL;
  }

  // Create Arrowhead
  TSLEntitySet* groupArrow = arrow->createArrow( coords, length, angle );
  if ( !groupArrow )
  {
    // Error returned by Arrowhead creation
    return NULL;
  }

  return groupArrow;
}


TSLEntitySet* attackByFirePosition::create( const TSLCoordSet &positions, int lengthArrowhead, int angleArrowhead, int lengthLegs )
{
  if ( !checkParameters( positions, lengthArrowhead, angleArrowhead, lengthLegs ) )
  {
    // Error invalid parameters
    return NULL;
  }

  // Find midpoint of line defined by Pts 2 and 3
  double dx = positions[1].x() - positions[2].x();
  double dy = positions[1].y() - positions[2].y();

  TSLCoord ptMid;
  ptMid.m_x = positions[1].x() - dx/2;
  ptMid.m_y = positions[1].y() - dy/2;

  // Create coordset for arrow
  TSLCoordSet* arrowPts = new TSLCoordSet();
  if ( !arrowPts )
  {
    return NULL;
  }
  arrowPts->add( ptMid, 0 ); // Start = PTm
  arrowPts->add( positions[0], 1 ); // End = PT1

  TSLEntitySet* groupArrow = createArrowheadSet( *arrowPts, lengthArrowhead, angleArrowhead );
  if ( !groupArrow )
  {
    return NULL;
  }

  // Create master Entity Set to contain graphic elements
  TSLEntitySet *groupMaster = TSLEntitySet::create();
  if ( !groupMaster )
  {
    return NULL;
  }

  // Add Arrowhead to master entity set
  if ( !groupMaster->insert( groupArrow ) )
  {
    // Error inserting Arrow group into master group
    return NULL;
  }

  // Add Arrowhead line to master entity set
  TSLPolyline* arrowLine = groupMaster->createPolyline( 0, arrowPts, true );
  if ( !arrowLine )
  {
    // Error adding Arrow line into master group
    return NULL;
  }

  TSLRenderingAttributes atts;
  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_edgeThickness = 1;
  atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  atts.m_edgeStyle = 1;
  arrowLine->setRendering( atts );


  // Calculate Firing Position Line
  TSLPolyline* firingPosnLine = calculateFiringPositionLine( positions, lengthLegs );
  if ( !firingPosnLine || !groupMaster->insert( firingPosnLine ) )
  {
    // Error adding Firing Position line into master group
    return NULL;
  }
  firingPosnLine->setRendering( atts );

  // Success return master entity set 
  return groupMaster;
}


TSLPolyline* attackByFirePosition::calculateFiringPositionLine( const TSLCoordSet &positions, int lengthLegs )
{
  // Create baseline vector
  math::Vector3D pt1( positions[1] );
  math::Vector3D pt2( positions[2] );

  // Calculate vector from PT1 to PT2
  math::Vector3D baseline = ( pt1 - pt2 );

  if ( 0 == lengthLegs )
  {
    lengthLegs = DEFAULT_LEG_LENGTH_PERCENTAGE;
  }

  double legPercentage = lengthLegs / 100.0;
  double legLength = baseline.length() * legPercentage;

  math::Vector3D normal(0,0,1);
  math::Vector3D perpendicular = baseline.crossProduct(normal);
  math::Vector3D parallel = baseline.operator~();
  parallel *= legLength;

  // Attack Point
  math::Vector3D ptAttack( positions[0] ); 

  // Attack Vector
  math::Vector3D attackLine = ( pt1 - ptAttack );
  double tripleProduct = perpendicular.dotProduct( attackLine );
  double sign = (tripleProduct > 0 ) ? -1 : 1;
  perpendicular.normalise();
  perpendicular = perpendicular * ( legLength * sign );

  // Find 1st leg point
  math::Vector3D legPt1 = pt1 - perpendicular;
  legPt1 = legPt1 + parallel;

  // Find 2nd leg point
  perpendicular = baseline.crossProduct( normal );
  perpendicular.normalise();
  perpendicular = perpendicular * ( legLength * sign );
  math::Vector3D legPt2 = pt2 - perpendicular;
  legPt2 = legPt2 - parallel;

  TSLCoordSet* fireLine = new TSLCoordSet();
  if ( !fireLine )
  {
    return NULL;
  }
  fireLine->add( legPt1.m_x, legPt1.m_y );
  fireLine->add( positions[1] );
  fireLine->add( positions[2] );
  fireLine->add( legPt2.m_x, legPt2.m_y );

  TSLPolyline* firingPositionLine = TSLPolyline::create( 0, fireLine, true );

  return firingPositionLine;
}



} // end TacticalGraphics namespace
} // end MapLink namespace
} // end Envitia namespace