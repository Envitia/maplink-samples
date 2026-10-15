/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
*/

#include "ambush.h"
#include "../utilities/simpleArrow.h"
#include "../utilities/vector3d.h"

namespace envitia { namespace MapLink { namespace TacticalGraphics {


int ambush::findLengthOfLine( const TSLCoord& pt1, const TSLCoord& pt2 ) const
{
  int length = -1;

  TSLCoordSet* coordSet = new TSLCoordSet();
  if ( !coordSet )
  {
    return length;
  }

  coordSet->add( pt1 );
  coordSet->add( pt2 );
  length = coordSet->length();
  coordSet->destroy();

  return length;
}


bool ambush::findPointOnLine( const TSLCoord& startPt, const TSLCoord& endPoint, TSLCoord& outPoint, int distance )
{
  /*
  Given the start and endpoints of a line and a distance, 
  find the point on the line that distance from the start point
  */
  int dx = endPoint.m_x - startPt.m_x;
  int dy = endPoint.m_y - startPt.m_y;

  // Apply distance to differences
  dx += distance;
  dy += distance;

  // Apply difference to Start point
  outPoint.m_x = startPt.m_x + dx;
  outPoint.m_y = startPt.m_y + dy;

  return true;
}



TSLEntitySet* ambush::create( const TSLCoordSet &positions, int lengthArrowhead, int angleArrowhead, int lengthLegs )
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


  // Create Arc 
//  TSLArc* arc = createArc( positions, ptMid );
  TSLEntitySet* arc = createArc( positions, ptMid, *arrowPts);
  if ( !arc )
  {
    arrowPts->destroy();
    return NULL;
  }

    // Add Arrowhead line to master entity set
  TSLPolyline* arrowLine = groupMaster->createPolyline( 0, arrowPts, true);
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

  // Add Arc to master group
  if ( !groupMaster->insert( arc ) )
  {
    return NULL;
  }


  // Success return master entity set 
  return groupMaster;
}

//TSLArc* ambush::createArc( const TSLCoordSet &anchorPoints, const TSLCoord& midPointAnchorLine )
TSLEntitySet* ambush::createArc( const TSLCoordSet &anchorPoints, const TSLCoord& midPointAnchorLine, TSLCoordSet &arrowPts )
{
  // Find length of line described by anchor points defining arc
  TSLCoord anchorPoint1, anchorPoint2, anchorPoint3;
  anchorPoint1.m_x = anchorPoints[0].m_x;
  anchorPoint1.m_y = anchorPoints[0].m_y;
  anchorPoint2.m_x = anchorPoints[1].m_x;
  anchorPoint2.m_y = anchorPoints[1].m_y;
  anchorPoint3.m_x = anchorPoints[2].m_x;
  anchorPoint3.m_y = anchorPoints[2].m_y;
  int radius = findLengthOfLine( anchorPoint2, anchorPoint3 );
  if ( -1 == radius )
  {
    return NULL;
  }

  // Find centre point for Arc
  // Use length of line between anchor points 2 & 3 as radius
  // Triangle formed by anchor points PT2, PT3 and centre is equilateral
  // This enables us to determine length of PTmid to centre 
  Long64 rad = radius;
  Long64 radius_2 = radius / 2.0;
  Long64 radiusSq = ( rad * rad ); 
  Long64 radius_2Sq = ( radius_2 * radius_2 );
  Long64 lenPtmCtr = sqrt( double(radiusSq - radius_2Sq) );

  // Create baseline vector
  math::Vector3D vPT2( anchorPoint2 );
  math::Vector3D vPT3( anchorPoint3 );
  math::Vector3D vPTMid( midPointAnchorLine );

  // Vector for PT2 to PT3
  math::Vector3D baseline = ( vPT2 - vPT3 );

  // Perpendicular
  math::Vector3D normal(0,0,1);
  math::Vector3D perpendicular = baseline.crossProduct(normal);
  perpendicular.normalise();

  math::Vector3D ptAttack( anchorPoints[0] ); 
  math::Vector3D attackLine = ( vPTMid - ptAttack );
  double tripleProduct = perpendicular.dotProduct( attackLine );
  double sign = (tripleProduct > 0 ) ? -1 : 1;
  perpendicular.normalise();
  perpendicular = perpendicular * sign;
  perpendicular *= lenPtmCtr;
  math::Vector3D ptCtr = ( vPTMid - perpendicular );
  TSLCoord ctr;
  ctr.m_x = ptCtr.m_x;
  ctr.m_y = ptCtr.m_y;

  // Need to determine angle between vector of ptMid - centre and the x-axis
  math::Vector3D plane = ( vPTMid - ptCtr );
  math::Vector3D xAxis(1, 0, 0);
  double anglePlane = plane.angleBetween( xAxis );

  if(plane.m_y < 0 )
    anglePlane *= -1;

  double startAngle = anglePlane - M_PI_6;
  double endAngle = startAngle + M_PI_3;

  TSLArc* arc = TSLArc::create( 0, startAngle, endAngle, ctr.m_x, ctr.m_y, radius );
  if ( !arc )
  {
    return NULL;
  }

  TSLRenderingAttributes atts;
  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_edgeThickness = 1;
  atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  atts.m_edgeStyle = 1;
  arc->setRendering( atts );

  //arrowPts[0] = TSLCoord(p1.m_x, p1.m_x);
  TSLEntitySet* eSet = TSLEntitySet::create();
  if ( !eSet )
  {
    return NULL;
  }

  // lines parallel to PT1 to PT2 facing into circle.
  {
    math::Vector3D vPT2( anchorPoint2 );
    math::Vector3D vPT3( anchorPoint3 );

    // Vector for PT2 to PT3
    math::Vector3D baseline = ( vPT2 - vPT3 );

    // Perpendicular
    math::Vector3D normal(0,0,1);
    math::Vector3D perpendicular = baseline.crossProduct(normal);
    perpendicular.normalise();


    math::Vector3D cpPT2 = ptCtr - vPT2;
    double len = cpPT2.length() / 4.0;

    len *= (-sign);

    double totalAngle = math::radiansToDegrees(endAngle - startAngle);

    double incAngle = totalAngle / 10.0;
    if (incAngle > 5.0)
      incAngle = 5.0;
    incAngle = math::degreesToRadians(incAngle);

    TSLCoordSet *s = new TSLCoordSet;
    double angle = startAngle + incAngle;
    while (angle < (endAngle-incAngle))
    {
      double x = ptCtr.m_x + radius * cos(angle);
      double y = ptCtr.m_y + radius * sin(angle);
      angle += incAngle;

      s->add(TSLCoord(x, y));

      math::Vector3D p1(x, y);

      p1 += perpendicular * len;

      s->add(TSLCoord(p1.m_x, p1.m_y));

      TSLPolyline* line = eSet->createPolyline(0, *s);
	  line->setRendering( atts );
      s->clear();
    }

    // shorten arrow line
    // TBD implement Arc to line intersection
    {
      // NOTE: this is an approximation for intersection with an arc.
      incAngle = math::degreesToRadians(0.1);

      double angle = startAngle + incAngle;
      while (angle < (endAngle-incAngle))
      {
        double x = ptCtr.m_x + radius * cos(angle);
        double y = ptCtr.m_y + radius * sin(angle);
        angle += incAngle;

        s->add(TSLCoord(x, y));

      }
      TSLPolyline *arrow = TSLPolyline::create(0, arrowPts);
      TSLPolyline *line = TSLPolyline::create(0, s, true);

      TSLEntity *intersection = line->intersection(arrow);
      if (intersection)
      {
        if (intersection->type() == TSLGeometryTypeEntitySet)
        {
          TSLEntitySet *group = static_cast<TSLEntitySet*>(intersection);
          if (group->size())
          {
            TSLEntity *entity = (*group)[0];
            if (entity->type() == TSLGeometryTypeSymbol)
            {
              arrowPts[0] = static_cast<TSLSymbol*>(entity)->position();
            }
          }
        }
      }

      line->destroy();
      arrow->destroy();
    }
  }



//  return arc;
// DEBUG
  
  /*
  
  // Create Symbols for anchorPoint2, anchorPoint3
  eSet->createSymbol( 0, anchorPoint2.m_x, anchorPoint2.m_y );
  eSet->createSymbol( 0, anchorPoint3.m_x, anchorPoint3.m_y );
  eSet->createSymbol( 0, midPointAnchorLine.m_x, midPointAnchorLine.m_y );
  eSet->createSymbol( 0, ctr.m_x, ctr.m_y );
  */


  eSet->insert( arc );
  
  return eSet;
}


} // end TacticalGraphics namespace
} // end MapLink namespace
} // end Envitia namespace