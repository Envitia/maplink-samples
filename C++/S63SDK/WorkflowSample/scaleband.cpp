// scaleband.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63WorkFlowView.h"
#include "s63\s63workflow.h"
#include "scaleband.h"
#include <vector>
#include <string>

// ZoomScale

IMPLEMENT_DYNAMIC(ScaleBand, CComboBox)

ScaleBand::ScaleBand()
{
  m_pParent = NULL;
  m_scaleBandChanged = false;
  m_scaleBand = 0;
  m_currentScaleBandAdded = false;
}

ScaleBand::~ScaleBand()
{
}


BEGIN_MESSAGE_MAP(ScaleBand, CComboBox)
  ON_CONTROL_REFLECT(CBN_SELCHANGE, &ScaleBand::OnCbnSelchange)
END_MESSAGE_MAP()


void ScaleBand::setParent(S63WorkFlowView* pParent)
{
  m_pParent = pParent;
}

void ScaleBand::AddScaleBandValues()
{
  const char* scaleBandStrings[] = {
                        "Display Cells within permitted Scale Bands",
                        "Over-scale of Cells Permitted", 
                        "Overview Scale Band", 
                        "General Scale Band",
                        "Coastal Scale Band", 
                        "Approach Scale Band", 
                        "Harbour Scale Band", 
                        "Berthing Scale Band"};

  std::vector<std::string> scaleBands(scaleBandStrings, scaleBandStrings + 8);
  for (size_t i=0; i < scaleBands.size(); i++)
  {
    InsertString(i,(LPCTSTR)scaleBands[i].c_str());
  }
  SetCurSel(0);
}

// ZoomScale message handlers

void ScaleBand::OnCbnSelchange()
{
  m_scaleBand = GetCurSel();

  const TSLS63DataLayerScaleBands * scaleBands = m_pParent->workFlow()->getScaleBands();
  m_pParent->workFlow()->clearScaleBands();

  double lower = 1.0, upper = 9999999999.00;
  switch (m_scaleBand)
  {
    case 0:
      m_pParent->workFlow()->updateScaleBands();
      break;
    case 1: // overscale
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandOverscale, lower, upper, false);
      break;
    case 2:
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandOverview, lower, upper, false);
      break;
    case 3:
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandGeneral, lower, upper, false);
      break;
    case 4:
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandCoastal, lower, upper, false);
      break;
    case 5:
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandApproach, lower, upper, false);
      break;
    case 6:
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandHarbour, lower, upper, false);
      break;
    case 7:
      m_pParent->workFlow()->setScaleBand(TSLS63ScaleBandBerthing, lower, upper, false);
      break;
    default:
      break;
  }

  m_scaleBandChanged = true;
  if(m_pParent)
  {
    ::InvalidateRect( m_pParent->m_hWnd, NULL, FALSE );
  }
}

void ScaleBand::setScaleBand(const char *buffer)
{
  SetCurSel(0);
}