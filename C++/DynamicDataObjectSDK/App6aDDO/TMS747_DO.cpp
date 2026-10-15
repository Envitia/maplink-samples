// TMS747_DO.cpp: implementation of the TMS747_DO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app6addo.h"
#include "TMS747_DO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long TMS747_DO::m_symbolID = 72;
TSLAPP6ASymbol::HostilityEnum TMS747_DO::m_hostility = TSLAPP6ASymbol::HostilityFriend;
TSLAPP6ASymbol::TypeEnum TMS747_DO::m_type = TSLAPP6ASymbol::TypeAirSpace;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMS747_DO::TMS747_DO()
    : App6aAircraftDO(m_hostility, m_type, m_symbolID)
{
}

TMS747_DO::~TMS747_DO()
{
}

