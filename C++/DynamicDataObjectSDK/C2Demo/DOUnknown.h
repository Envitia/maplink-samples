// DOUnknown.h: interface for the CDOUnknown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOUNKNOWN_H__8A4F954D_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOUNKNOWN_H__8A4F954D_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DOTrack.h"

class CDOUnknown : public CDOTrack 
{
public:
  CDOUnknown();
  virtual ~CDOUnknown();

  virtual bool drawOutline( HDC surfaceDC ) ;

private:

  POINT m_outline[12] ;

};

#endif // !defined(AFX_DOUNKNOWN_H__8A4F954D_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
