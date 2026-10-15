// App6aAircraftDO.cpp: implementation of the App6aAircraftDO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app6addo.h"
#include "App6aAircraftDO.h"

#include <cmath>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// pixels size
#define SYMBOL_HEIGHT   50
#define OUTLINE_COLOUR	    0x00f0f0f0

TSLAPP6AHelper* App6aAircraftDO::m_symbolHelper = 0;
int App6aAircraftDO::m_referenceCount = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

App6aAircraftDO::App6aAircraftDO(TSLAPP6ASymbol::HostilityEnum hostility,
                                 TSLAPP6ASymbol::TypeEnum type,
                                 long symbolKey)
                                 : m_px (0)
                                 , m_py (0)
{
  setSize( 0, 0, 0, 0 );
  
  //create the symbol helper if it hasn't been already
  if ( !m_symbolHelper )
    m_symbolHelper = new TSLAPP6AHelper();

  ++m_referenceCount;

  //create the symbol
  TSLAPP6ASymbol symbol;
  symbol.key(symbolKey);
  symbol.height(SYMBOL_HEIGHT);
  symbol.heightType(TSLDimensionUnitsPixels);
  symbol.hostility(hostility);
  symbol.isFramed(true);
  symbol.type(type);

  char buffer[10];
  sprintf(&buffer[0], "%d", m_referenceCount );
  symbol.additionalInformation(&buffer[0]);
  m_symbolEntitySet = m_symbolHelper->getSymbolAsEntitySet(&symbol);

  
}

App6aAircraftDO::~App6aAircraftDO()
{
  //if this is last one, then delete the helper
  if ( !--m_referenceCount )
  {
    m_symbolHelper->destroy();
    m_symbolHelper = 0;
  }
  m_symbolEntitySet->destroy();
  m_symbolEntitySet = 0;
}

bool App6aAircraftDO::draw(TSLRenderingInterface *d_surface, TSLEnvelope *d_extent )
{
  // Retrieve a pointer to the associated track
  App6aAircraft * pAircraft = (App6aAircraft *)m_ddo ;

  // Get the current position of the track in terms of pixels
  TSLCoord coord = pAircraft->position() ;

  long x = coord.x();
  long y = coord.y();

  const long dx = x - m_px;
  const long dy = y - m_py;

  m_symbolEntitySet->translate(dx, dy);
  
  m_py = y;
  m_px = x;

  d_surface->drawEntity(m_symbolEntitySet);
  
  return true;
}

