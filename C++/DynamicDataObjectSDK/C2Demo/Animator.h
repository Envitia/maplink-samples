// Animator.h: interface for the CAnimator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATOR_H__E0F12532_49CE_11D5_BD2E_00104BB05CAE__INCLUDED_)
#define AFX_ANIMATOR_H__E0F12532_49CE_11D5_BD2E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TSLViewModeInterfaceNT ;

class CAnimator  
{
public:
	CAnimator();
	virtual ~CAnimator();
	bool startAnimation( PLAYBACK_RATE playbackRate = REAL_TIME ) ;
	bool stopAnimation() ;
#if !defined(_MSC_VER) || _MSC_VER > 1200
	static void CALLBACK OnTimer( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime ) ;
// VC6 requires a different dignature for this TimerProc callback paramater in SetTimer
#else
	static void CALLBACK OnTimer( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime ) ;
#endif
	PLAYBACK_RATE playbackRate() const ;
    protected:
	PLAYBACK_RATE			m_playbackRate ;
        UINT_PTR			m_timerID;
 
};

#endif // !defined(AFX_ANIMATOR_H__E0F12532_49CE_11D5_BD2E_00104BB05CAE__INCLUDED_)
