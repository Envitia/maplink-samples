// TMSAirbus_DO.cpp: implementation of the TMSAirbus_DO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app6addo.h"
#include "TMSAirbus_DO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long TMSAirbus_DO::m_symbolID = 7;
TSLAPP6ASymbol::HostilityEnum TMSAirbus_DO::m_hostility = TSLAPP6ASymbol::HostilityFriend;
TSLAPP6ASymbol::TypeEnum TMSAirbus_DO::m_type = TSLAPP6ASymbol::TypeAirSpace;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMSAirbus_DO::TMSAirbus_DO()
    : App6aAircraftDO(m_hostility, m_type, m_symbolID)
{
}

TMSAirbus_DO::~TMSAirbus_DO()
{
}

