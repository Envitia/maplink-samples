// TMS747_DO.h: interface for the TMS747_DO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMS747_DO_H__56D381A9_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_TMS747_DO_H__56D381A9_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class TMS747_DO : public App6aAircraftDO  
{
public:
  TMS747_DO();
  virtual ~TMS747_DO();
private:
  static long m_symbolID;
  static TSLAPP6ASymbol::HostilityEnum m_hostility;
  static TSLAPP6ASymbol::TypeEnum m_type;
};

#endif // !defined(AFX_TMS747_DO_H__56D381A9_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
