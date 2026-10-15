// DOFrigate.h: interface for the CDOFrigate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOFRIGATE_H__8A4F954B_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOFRIGATE_H__8A4F954B_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DOTrack.h"

class CDOFrigate : public CDOTrack  
{
public:
  CDOFrigate();
  virtual ~CDOFrigate();

  virtual bool drawOutline( HDC surfaceDC ) ;

private:

  POINT m_outline[4] ;
};

#endif // !defined(AFX_DOFRIGATE_H__8A4F954B_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
