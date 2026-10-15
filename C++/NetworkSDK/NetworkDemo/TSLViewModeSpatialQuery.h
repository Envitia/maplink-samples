//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************

#ifndef _tslviewmodespatialquery_h_
#define _tslviewmodespatialquery_h_ 1

#include "tslviewmode.h"

class CNetworkDemoDoc;


class TSLViewModeSpatialQuery :
  public TSLViewMode
{
public:

  TSLViewModeSpatialQuery(TSLViewModeInterface * iViewMode, unsigned long modeID, CNetworkDemoDoc* doc);
  
  ~TSLViewModeSpatialQuery(void);

  virtual bool OnLButtonDown(long x, long y, bool shift, bool control);
  virtual bool OnLButtonUp(long x, long y, bool shift, bool control);
  virtual bool OnRButtonDown(long x, long y, bool shift, bool control);

  virtual bool OnMouseMove(long x, long y, bool shift, bool control);

  void useSpatialIndex( bool state );

private:

  CNetworkDemoDoc* m_document;

  long m_x;
  long m_y;

  TSLTMC m_searchPtTMCx;
  TSLTMC m_searchPtTMCy;

  bool m_pressed;

  bool m_useSpatialIndex;

};


#endif 
