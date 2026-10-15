/****************************************************************************
    Copyright (c) 2000-2017 by Envitia Group PLC.
=============================================================================
MODULE          : 
PACKAGE         : MapLink
SCCS            : 
AUTHOR(s)       : D.Dixon
=============================================================================
DESCRIPTION     : 
****************************************************************************/
#ifndef _DOAPP6ATRACK_H_
#define _DOAPP6ATRACK_H_

#include "dotrack.h"

class CTrack ;
class APP6ATrack;

class DOAPP6ATrack : public CDOTrack  
{
public:
  DOAPP6ATrack();
  virtual ~DOAPP6ATrack();

protected:
  virtual bool draw( TSLRenderingInterface *d_surface, TSLEnvelope *d_extent ) ;

  virtual void drawOutline(  HDC surfaceDC ) ;       // Only derived classes know what they look like
  void drawTrackNumber(  HDC surfaceDC, CTrack * track );
  void showAsSelectedTrack( HDC surfaceDC );


  bool m_firstDraw;
  int m_px, m_py;

private:
  HBITMAP     m_bitmap;
  HBITMAP     m_bitmapOld;
  HDC         m_memDC;
  TSLEntity*  m_text;

  bool drawBitmap(HDC hDC, APP6ATrack* track, long x, long y, bool regenerateBitmap);
  bool getSymbolAsBitmap(HDC hDC, APP6ATrack* track);
  void clearBitmap();
};

#endif

