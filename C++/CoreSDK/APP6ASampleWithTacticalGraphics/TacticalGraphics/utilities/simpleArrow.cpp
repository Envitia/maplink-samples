/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
*/

#include "simpleArrow.h"
#include "vector3d.h"

namespace envitia { namespace MapLink { namespace TacticalGraphics {


bool simpleArrow::checkParameters( const TSLCoordSet &positions, int& length, int& angle )
{
  if ( 0 == length )
  {
    length = DEFAULT_ARROWHEAD_LENGTH_PERCENTAGE;
  }

  // Check length within specified range
  if ( length < 0 || length > 100 )
  {
    // Error length outside percentage range
    return false;
  }

  if ( 0 == angle )
  {
    angle = DEFAULT_ARROWHEAD_ANGLE_DEG;
  }

  // Check angle within specified range
  if ( angle < MIN_ARROWHEAD_ANGLE_DEG || angle > MAX_ARROWHEAD_ANGLE_DEG )
  {
    // Error length outside percentage range
    return NULL;
  }

  // Check two positions passed
  if ( positions.size() != 2)
  {
    return NULL;
  }

  TSLCoord start(positions[0]);
  TSLCoord end(positions[1]);
  if ( start == end )
  {
    // Error points are same.
    return false;
  }

  return true;
}

bool simpleArrow::calculatePoints( const TSLCoordSet &positions, int length, int angle )
{
  m_pt1 = positions[0]; // Start 
  m_pt2 = positions[1]; // End


  // X and Y lengths 
  double dx = m_pt2.x() - m_pt1.x();
  double dy = m_pt2.y() - m_pt1.y();

  // Apply length correction here
  dx *= ( 0.01 * (double)length );
  dy *= ( 0.01 * (double)length );

  // Calculate length and angle of arrow
  double lengthArrow = ( sqrt( dx * dx + dy * dy ) );
  double sinArrow = - dx / lengthArrow ; // Negative since rotating other way
  double cosArrow = dy / lengthArrow ;

  // Calculate arrow triangle points when arrow is vertically upwards
  double angleRadians = math::degreesToRadians( angle );
  double sinAngle = sin( angleRadians );
  double cosAngle = cos( angleRadians );

  double rightDx =  lengthArrow * sinAngle ;
  double leftDx  = -rightDx ;
  double headDy  = -lengthArrow * cosAngle ;

  // Rotate head offsets by arrow angle
  m_ptH1.x( (Long)(leftDx * cosArrow - headDy * sinArrow) ) ;
  m_ptH1.y( (Long)(leftDx * sinArrow + headDy * cosArrow) ) ;
  m_ptH1.translate( m_pt2.x(), m_pt2.y() ) ;

  m_ptH2.x( rightDx * cosArrow - headDy * sinArrow ) ;
  m_ptH2.y( rightDx * sinArrow + headDy * cosArrow ) ;
  m_ptH2.translate( m_pt2.x(), m_pt2.y() ) ;

  m_ptHm.x( - headDy * sinArrow ) ;
  m_ptHm.y(   headDy * cosArrow ) ;
  m_ptHm.translate( m_pt2.x(), m_pt2.y() ) ;

  return true;
}


TSLEntitySet* simpleArrow::createArrow( const TSLCoordSet &positions, int length, int angle )
{
  if ( !checkParameters( positions, length, angle ) )
  {
    return NULL;
  }

  if ( !calculatePoints( positions, length, angle ) )
  {
    return NULL;
  }


  // NOTE : Since GeometryUtility::ringIsClockwise cannot handle this if the bottom rightmost
  //        point is start, it will return anti-clockwise. Ensure that this is so.
  TSLCoordSet* coords = new TSLCoordSet();
  if ( !coords || !addPoints( *coords ) )
  {
    return NULL;
  }

  TSLEntitySet* group = TSLEntitySet::create();
  if ( !createArrowEntity( group, *coords ) )
  {
    return NULL;
  }

  return group;
}


bool simpleArrow::createArrowEntity( TSLEntitySet* group, TSLCoordSet& positions )
{
  TSLPolyline* arrowHeadLine = group->createPolyline( 0, &positions, true );
  if ( !arrowHeadLine )
  {
    return false;
  }
  
  TSLRenderingAttributes atts;
  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_edgeThickness = 1;
  atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  atts.m_edgeStyle = 1;

  arrowHeadLine->setRendering( atts );

  return true;
}


bool simpleArrow::addPoints( TSLCoordSet &arrowPoints )
{
  arrowPoints.clear() ;
  arrowPoints.add( m_ptH1 ) ;
  arrowPoints.add( m_pt2 ) ;
  arrowPoints.add( m_ptH2 ) ;

  return true;
}


} // end TacticalGraphics namespace
} // end MapLink namespace
} // end Envitia namespace