//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: /MapLinkWizard/Template/tslviewmodeeditProps.cpp $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#include "stdafx.h"
#include "resource.h"
#include "RectangleProperties.h"
#include "PolygonProperties.h"
#include "PolylineProperties.h"
#include "SymbolProperties.h"
#include "TextProperties.h"
#include "ArcProperties.h"
#include "EllipseProperties.h"

TSLViewModeEditProps::TSLViewModeEditProps( TSLViewModeInterface * iViewMode, unsigned long modeID ) 
  : TSLViewMode( iViewMode, modeID )
{

}

TSLViewModeEditProps::~TSLViewModeEditProps()
{

}

bool TSLViewModeEditProps::OnLButtonDown(long x, long y, bool shift, bool control)
{ 
  // Get the position pressed in user units
  TSLEntity * entity = m_interface->drawingSurface()->findEntityDU( x, y, 5, 0 ) ;
  
  if ( entity )
  {
    // A little dodgy I know, but it works whilst only making this mode application specific.
    // Forgive me.
    TSLViewModeInterfaceNT * realInterface = static_cast< TSLViewModeInterfaceNT *>( m_interface ) ;
    CGeometryCreationDoc * doc = realInterface->document() ;

    if ( doc )
    {
      if ( TSLPolygon::isPolygon(entity) )
      {
        new CPolygonProperties( doc, TSLPolygon::isPolygon(entity), 0 ) ;
      }
      else if ( TSLPolyline::isPolyline(entity) )
      {
        new CPolylineProperties( doc, TSLPolyline::isPolyline(entity), 0 ) ;
      }
      else if ( TSLRectangle::isRectangle(entity) )
      {
        new CRectangleProperties( doc, TSLRectangle::isRectangle(entity), 0 ) ;
      }
      else if ( TSLText::isText(entity) )
      {
        new CTextProperties( doc, TSLText::isText(entity), 0 ) ;
      }
      else if ( TSLSymbol::isSymbol(entity) )
      {
        new CSymbolProperties( doc, TSLSymbol::isSymbol(entity), 0 ) ;
      }
      else if ( TSLArc::isArc(entity) )
      {
        new CArcProperties( doc, TSLArc::isArc(entity), 0 ) ;
      }
      else if ( TSLEllipse::isEllipse(entity) )
      {
        new CEllipseProperties( doc, TSLEllipse::isEllipse(entity), 0 ) ;
      }
    }
  }
  return false ;
}

