// TMSAirbus_DO.cpp: implementation of the TMSAirbus_DO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMSAirbus_DO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define TMS_Airbus_SYMCOL	0x000000FF
#define TMS_Airbus_SYMCOL256	0x000000FF

#define TMS_Airbus_TXTCOL	0x00000000
#define TMS_Airbus_TXTCOL256	0x00000000

POINT TMSAirbus_DO::m_outline[NP] = { {   0, -10 },
				      {   2,  -8 },
				      {   3,  -4 },
				      {   8,  -1 },
				      {   9,   1 },
				      {   2,   2 },
				      {   2,   7 },
				      {   5,   8 },
				      {   5,   9 },
				      {   0,  10 },
				      {  -2,   7 },
				      {  -2,   2 },
				      {  -9,   1 },
				      {  -8,  -1 },
				      {  -3,  -4 },
				      {  -2,  -8 }
				    };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMSAirbus_DO::TMSAirbus_DO()
{
}

TMSAirbus_DO::~TMSAirbus_DO()
{
}

ColourPair TMSAirbus_DO::getColours(bool col256) const
{
  return ( col256 ? ColourPair( TMS_Airbus_SYMCOL256, TMS_Airbus_TXTCOL256 ) : ColourPair( TMS_Airbus_SYMCOL, TMS_Airbus_TXTCOL ) );
}

bool TMSAirbus_DO::getSymbolDetails(POINT*& pts, int& num, long& tx, long& ty) const
{
  pts = m_outline;
  num = NP;

  return true;
}

