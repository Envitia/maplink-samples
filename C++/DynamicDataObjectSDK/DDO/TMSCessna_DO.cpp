// TMSCessna_DO.cpp: implementation of the TMSCessna_DO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMSCessna_DO.h"
#include "Aircraft.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


int TMSCessna_DO::m_symbolID = 6002;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMSCessna_DO::TMSCessna_DO()
{
  setSize( 0, 0, 0, 0 );
}

TMSCessna_DO::~TMSCessna_DO()
{
}

bool TMSCessna_DO::draw(TSLRenderingInterface *d_surface, TSLEnvelope *d_extent )
{
  TSLCoord pos = m_ddo->position();

  TSLTMC wndX = 0;
  TSLTMC wndY = 0;
  
  d_surface->TMCToDU( pos.x(), pos.y(), &wndX, &wndY );

  // TMC units / pixel.
  double resolutionX, resolutionY ;
  d_surface->screenResolution(resolutionX, resolutionY);
  double size = 20.0*resolutionX;

  Aircraft* plane = (Aircraft*)( m_ddo );
  double hdg = plane->heading() ;

  hdg *= -1;

  d_surface->setupSymbolAttributes(m_symbolID, 4, size, TSLDimensionUnitsUndefined, 1, 2000, hdg );
  d_surface->drawSymbol( pos, (long)size );

  return true;
}

