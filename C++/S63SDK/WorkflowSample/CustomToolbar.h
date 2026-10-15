#pragma once


#include "zoomscale.h"
#include "scaleband.h"
// CCustomToolbar

class CCustomToolbar : public CToolBar
{
	DECLARE_DYNAMIC(CCustomToolbar)

public:
	CCustomToolbar();
	virtual ~CCustomToolbar();

  bool createComboBox( int toolbarID, int comboID );
  bool createScaleBandComboBox( int toolbarID, int comboID );

  ZoomScale m_zoomCombo;
  ScaleBand m_scaleBandCombo;
  CFont m_font;

protected:
	DECLARE_MESSAGE_MAP()
};


