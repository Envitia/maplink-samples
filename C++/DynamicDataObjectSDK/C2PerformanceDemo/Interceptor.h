// Interceptor.h: interface for the CInterceptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERCEPTOR_H__3CECCE69_4FEC_4E35_BACD_E1BDD82669B5__INCLUDED_)
#define AFX_INTERCEPTOR_H__3CECCE69_4FEC_4E35_BACD_E1BDD82669B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

class CInterceptor : public CTrack  
{
  public:
    CInterceptor();
    CInterceptor( const CTrack & other );
    virtual ~CInterceptor();

    TSLDisplayObject * instantiateDO( TSLDisplayType key, int dsID = 0 ) const ;
    TRACK_TYPE         trackType() const ;      
};

#endif // !defined(AFX_INTERCEPTOR_H__3CECCE69_4FEC_4E35_BACD_E1BDD82669B5__INCLUDED_)
