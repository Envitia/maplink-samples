// TMS747_DO.h: interface for the TMS747_DO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMS747_DO_H__56D381A9_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_TMS747_DO_H__56D381A9_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "aircraftdo.h"

class TMS747_DO : public AircraftDO  
{
public:
  TMS747_DO();
  virtual ~TMS747_DO();
  
  virtual ColourPair getColours(bool col256 = true) const;
  
  virtual bool getSymbolDetails(POINT*& pts, int& num, long& tx, long& ty) const;

private:
  static enum { NP = 12 };
  static POINT m_outline[ NP ];
};

#endif // !defined(AFX_TMS747_DO_H__56D381A9_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
