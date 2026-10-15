// DORangeRing.h: interface for the CDORangeRing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DORANGERING_H__8A4F954E_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
#define AFX_DORANGERING_H__8A4F954E_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dotrack.h"

class CDORangeRing : public CDOTrack  
{
public:
  CDORangeRing();

  virtual ~CDORangeRing();

  virtual bool draw (TSLRenderingInterface *d_surface, TSLEnvelope *d_extent) ;

  virtual void drawOutline( HDC surfaceDC ) { } 

  static void ringColour( COLORREF colour ) { m_ringColour = colour; }

private :
  TSLEnvelope	    m_drawnExtent ;
  long	    m_lastOwnShipX ;
  long	    m_lastOwnShipY ;
  static COLORREF m_ringColour ;
  static HANDLE   m_RRfont ;
  static int      m_RRReference ;
  static HPEN  m_penDot;
  static HPEN  m_penSolid;

  struct RRing
  {
    Long m_x1 ;
    Long m_y1 ;
    Long m_x2 ;
    Long m_y2 ;
    Long m_x3 ;
    Long m_y3 ;
    Long m_x4 ;
    Long m_y4 ;
    Long m_tx ;
    Long m_ty ;
    string m_label ;
    bool m_arc ;

    RRing( const RRing & r ) : m_x1( r.m_x1 ), m_x2( r.m_x2 ), 
      m_y1( r.m_y1 ), m_y2( r.m_y2 ), m_x3( r.m_x3 ), m_y3( r.m_y3 ),
      m_x4( r.m_x4 ), m_y4( r.m_y4 ), m_tx( r.m_tx ), m_ty( r.m_ty ), m_arc( r.m_arc )
    {
      m_label = r.m_label ;
    }

    RRing( ) : m_x1(0), m_y1(0), m_x2(0), m_y2(0), m_x3(0), m_y3(0), 
      m_x4(0), m_y4(0), m_tx(0), m_ty(0), m_arc( true )
    {
    }

    RRing & operator=( const RRing & r )
    {
      if ( &r != this )
      {
        m_x1 = r.m_x1 ;
        m_x2 = r.m_x2 ;
        m_y1 = r.m_y1 ;
        m_y2 = r.m_y2 ;
        m_x3 = r.m_x3 ;
        m_y3 = r.m_y3 ;
        m_x4 = r.m_x4 ;
        m_y4 = r.m_y4 ;
        m_tx=  r.m_tx ;
        m_ty = r.m_ty ;
        m_arc = r.m_arc ;
        m_label = r.m_label ;
      }
      return *this ;
    }
  } ;

  vector<RRing> m_minorRings ;
  vector<RRing> m_majorRings ;

  void recalculateRings( TSLRenderingInterface *d_surface, int minorDiv, int majorDiv ) ;

  bool getMinMaxDivisor( int & mind, int & maxd, double size ) ;

  void updateDistances (long x, long y, double &minimum, double &maximum)
  {
    double d = (double)x * (double)x + (double)y * (double)y ;

    if ( d < minimum )
      minimum = d ;
    if ( d > maximum )
      maximum = d ;
  }

};

#endif // !defined(AFX_DORANGERING_H__8A4F954E_5B1F_11D5_BD3E_00104BB05CAE__INCLUDED_)
