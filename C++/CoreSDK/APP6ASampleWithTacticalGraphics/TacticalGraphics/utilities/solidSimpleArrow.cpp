/* -*-c++-*- */
/* Tactical Graphics (APP6C)
*
* Copyright (c) 2017 Envitia PLC
* http://www.envitia.com
*
*/

#include "solidSimpleArrow.h"

namespace envitia { namespace MapLink { namespace TacticalGraphics {


bool solidSimpleArrow::addPoints( TSLCoordSet &arrowPoints )
{
  simpleArrow::addPoints( arrowPoints );
  arrowPoints.add( m_ptH1 ) ;

  return true;
}

bool solidSimpleArrow::createArrowEntity( TSLEntitySet* group, TSLCoordSet& positions )
{
  TSLPolygon* arrowHeadLine = group->createPolygon( 0, &positions, true );
  if ( !arrowHeadLine )
  {
    return false;
  }

  TSLRenderingAttributes atts;
  atts.m_edgeColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_edgeThickness = 1;
  atts.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  atts.m_edgeStyle = 1;
  atts.m_fillColour = TSLComposeRGB( 0, 0, 0 );
  atts.m_fillStyle = 1;

  arrowHeadLine->setRendering( atts );

  return true;
}


} // end TacticalGraphics namespace
} // end MapLink namespace
} // end Envitia namespace