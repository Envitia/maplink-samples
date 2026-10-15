// TMSCessna_DO.h: interface for the TMSCessna_DO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMSCESSNA_DO_H__56D381D1_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_TMSCESSNA_DO_H__56D381D1_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class TMSCessna_DO : public App6aAircraftDO  
{
public:
  TMSCessna_DO();
  virtual ~TMSCessna_DO();

private:
  static long m_symbolID;
  static TSLAPP6ASymbol::HostilityEnum m_hostility;
  static TSLAPP6ASymbol::TypeEnum m_type;
};

#endif // !defined(AFX_TMSCESSNA_DO_H__56D381D1_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
