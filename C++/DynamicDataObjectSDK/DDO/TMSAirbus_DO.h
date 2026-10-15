// TMSAirbus_DO.h: interface for the TMSAirbus_DO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMSAIRBUS_DO_H__56D381AA_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_TMSAIRBUS_DO_H__56D381AA_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "aircraftdo.h"

class TMSAirbus_DO : public AircraftDO  
{
public:
  TMSAirbus_DO();
  virtual ~TMSAirbus_DO();
  
  virtual ColourPair getColours(bool col256 = true) const;
  
  virtual bool getSymbolDetails(POINT*& pts, int& num, long& tx, long& ty) const;

private:
  static enum { NP = 16 };
  static POINT m_outline[ NP ];
};

#endif // !defined(AFX_TMSAIRBUS_DO_H__56D381AA_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
