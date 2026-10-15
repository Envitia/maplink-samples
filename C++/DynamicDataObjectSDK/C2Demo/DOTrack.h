// DOTrack.h: interface for the CDOTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTRACK_H__8A4F9543_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DOTRACK_H__8A4F9543_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTrack ;

class CDOTrack : public TSLDisplayObject  
{
public:
  CDOTrack();
  virtual ~CDOTrack();

  static HANDLE m_font ;
  static int    m_doReference ;

protected:

  // Generic drawing functions required for displaying track information.
  // The actual track symbols are rendered in the derived classes
  virtual bool setColours( HDC surfaceDC, CTrack * pTrack ) ;

  virtual bool draw( TSLRenderingInterface *d_surface, TSLEnvelope *d_extent ) ;

  virtual bool drawHistoryPoints( TSLRenderingInterface *d_surface, CTrack * pTrack ) ; 

  virtual bool drawVelocityVector( TSLRenderingInterface *d_surface, CTrack * pTrack ) ;

  virtual bool drawPlatformCode( HDC surfaceDC, CTrack * pTrack ) ;

  virtual bool drawTrackNumber( HDC surfaceDC, CTrack * pTrack ) ;

  virtual bool showAsSelectedTrack( HDC surfaceDC ) ;

  virtual bool drawOutline( HDC surfaceDC ) = 0 ;	  // Only derived classes know what they look like

  COLORREF m_fillColour, m_textColour ;	// Colours for rendering the symbol

  double m_xPos, m_yPos ;			// Store for the track position

  long m_textX, m_textY ;			// Used for marking where to draw the Platform code text. 
  // Set in derived classes constructors

};

#endif // !defined(AFX_DOTRACK_H__8A4F9543_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
