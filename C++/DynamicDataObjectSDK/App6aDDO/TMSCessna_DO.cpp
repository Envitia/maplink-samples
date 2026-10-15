// TMSCessna_DO.cpp: implementation of the TMSCessna_DO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app6addo.h"
#include "TMSCessna_DO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


long TMSCessna_DO::m_symbolID = 71;
TSLAPP6ASymbol::HostilityEnum TMSCessna_DO::m_hostility = TSLAPP6ASymbol::HostilityNeutral;
TSLAPP6ASymbol::TypeEnum TMSCessna_DO::m_type = TSLAPP6ASymbol::TypeAirSpace;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMSCessna_DO::TMSCessna_DO() 
  : App6aAircraftDO(m_hostility, m_type, m_symbolID)
{
  setSize( 0, 0, 0, 0 );
}

TMSCessna_DO::~TMSCessna_DO()
{
}
