// RangeRing.cpp: implementation of the CRangeRing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RangeRing.h"
#include "DORangeRing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRangeRing::CRangeRing()
{
  selectable( false ) ;
}

CRangeRing::~CRangeRing () 
{
}

void CRangeRing::recalculatePositionsFromLatLong( TSLDrawingSurface* ds )
{
  TSLTMC	x, y;

  if ( ds->latLongToTMC( m_latitude, m_longitude, &x, &y ) )
  {
    // Don't update the range ring's extent
    move( TSLCoord( x, y ) );
  }
}

TSLDisplayObject * CRangeRing::instantiateDO( TSLDisplayType key, int dsID ) const
{
  TSLDisplayObject * myDO = new CDORangeRing( ) ;
  myDO->setExtent( getExtent() ) ;
  return myDO ;
}

TRACK_TYPE CRangeRing::trackType() const
{
  return RANGE_RINGS ;
}
