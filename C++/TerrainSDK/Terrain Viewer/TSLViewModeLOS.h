// TSLViewModeLOS.h: interface for the TSLViewModeLOS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSLVIEWMODELOS_H__051C52EB_4B84_11D5_93AF_00C04FA10B1D__INCLUDED_)
#define AFX_TSLVIEWMODELOS_H__051C52EB_4B84_11D5_93AF_00C04FA10B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NUMPOINTS 100
#define NUMLOSPOINTS 1000

class CTerrainViewerDoc ;
class CLOSView;

#include "tslviewmode.h"

class CTerrainView;
class CLOSView;

class TSLViewModeLOS : public TSLViewMode 
{
public:

  TSLViewModeLOS( TSLViewModeInterface * iViewMode, unsigned long modeID, CTerrainView * pTerrainView, CTerrainViewerDoc * pDoc );
  TSLViewModeLOS( TSLViewModeInterface * iViewMode, unsigned long modeID, CLOSView * pTerrainLOSView, CTerrainViewerDoc * pDoc );
  virtual ~TSLViewModeLOS();
  virtual bool OnLButtonDown( long x, long y, bool shift, bool control );
  virtual bool OnMouseMove(long x, long y, bool shift, bool control) ;
  virtual bool OnLButtonUp(long x, long y, bool shift, bool control) ;
  void setCentralMeridian( double meridian );

private:

  // Works out the straight line distance between two coordinates.
  double distance(const double x1, const double y1, const double x2, const double y2) const 
  {
    double a_side = ( x2 - x1 ) ;
    double b_side = ( y2 - y1 ) ;
    //c side...
    return sqrt( (a_side*a_side) + (b_side*b_side) ) ;
  }

  void adjust_for_earthcurvature(const double distance, double &height) const
  {
    // Radius of the earth in metres.
    const double radius = 6378137.0 ;
    const double radiusSquared = radius * radius;

    // Correct the heighte
    // Assuming the earth is a sphere, distance is planar
    height += radius - sqrt( radiusSquared + ( distance * distance ) ) ;
  }

  void adjust_for_dateline(const double start_x, const double start_y, const double end_x, const double end_y,
    double &length, double &dist) const ;
  void adjust_for_datelineGreatCircle(const double start_lat, const double start_lon, const double end_lat, const double end_lon,
    double &length, double &dist) const ;

private:
  long   m_startX;
  long   m_startY;
  double m_start_mux  ;
  double m_start_muy  ;
  double m_end_mux;
  double m_end_muy;
  bool   m_pressed ;
  double m_percentage ;
  bool m_convert ;
  bool m_isLatLon;
  double m_central_meridian;
  CTerrainView * m_terrain_view;
  CLOSView *m_los_view;
  TSLStandardDataLayer * m_layer;
  CTerrainViewerDoc * m_doc ;
};

#endif // !defined(AFX_TSLVIEWMODELOS_H__051C52EB_4B84_11D5_93AF_00C04FA10B1D__INCLUDED_)
