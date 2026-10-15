//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmoderoute_h_
#define _tslviewmoderoute_h_ 1

#include "tslviewmode.h"

class CNetworkDemoDoc;

// This mode handles routing.

class TSLViewModeRoute : public TSLViewMode
{
public:
  typedef enum
  {
    TSLRoute_Default,
    TSLRoute_AddingStartPin,
    TSLRoute_AddingEndPin,
    TSLRoute_AddingWayptPin,
    TSLRoute_StartPinAdded,
    TSLRoute_WayptPinAdded,
    TSLRoute_AbleToRoute
  } TSLRouteState;

public:
  TSLViewModeRoute(TSLViewModeInterface* iViewMode, unsigned long modeID, CNetworkDemoDoc* doc);
  ~TSLViewModeRoute();
  
  virtual bool OnLButtonDown(long x, long y, bool shift, bool control);
  virtual bool OnLButtonUp(long x, long y, bool shift, bool control);
  virtual bool OnRButtonUp(long x, long y, bool shift, bool control);

  virtual bool OnMouseMove(long x, long y, bool shift, bool control);

  void reset();

  void addStartPin();
  void addEndPin();
  void addWayptPin();

  void clearAllRoutes(bool keepFirstPoint = false);

  TSLRouteState getCurrentState() const
  {
    return m_currentState;
  }

private:
  bool addRoutePin();

private:
  CNetworkDemoDoc* m_document;

  TSLRouteState m_currentState;
  TSLRouteState m_previousState;

  long m_x;
  long m_y;
  
  bool m_pressed;
};

#endif 

