// ZoomScale.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63WorkFlowView.h"
#include "zoomScale.h"
#include <vector>
#include <string>

// ZoomScale

IMPLEMENT_DYNAMIC(ZoomScale, CComboBox)

ZoomScale::ZoomScale()
{
  m_pParent = NULL;
  m_scaleChanged = false;
  m_scale = 0;
  m_currentScaleAdded = false;
}

ZoomScale::~ZoomScale()
{
}


BEGIN_MESSAGE_MAP(ZoomScale, CComboBox)
  ON_CONTROL_REFLECT(CBN_SELCHANGE, &ZoomScale::OnCbnSelchange)
END_MESSAGE_MAP()


void ZoomScale::setParent(S63WorkFlowView* pParent)
{
  m_pParent = pParent;
}

void ZoomScale::AddScaleValues()
{
  const char* scaleStrings[] = {"1:10000000", 
                        "1: 5000000", 
                        "1: 2000000",
                        "1: 1000000", 
                        "1:  500000", 
                        "1:  250000", 
                        "1:  100000", 
                        "1:   75000",
                        "1:   50000", 
                        "1:   25000", 
                        "1:   20000", 
                        "1:   15000", 
                        "1:   10000", 
                        "1:    5000", 
                        "1:    2000", 
                        "1:    1000"};

  std::vector<std::string> scales(scaleStrings, scaleStrings + 16); 
  for (size_t i=0; i < scales.size(); i++)
  {
    InsertString(i,(LPCTSTR)TSLUTF8Decoder(scales[i]));
  }
}

// ZoomScale message handlers

void ZoomScale::OnCbnSelchange()
{
  // Get the feature name selected from the list
  CString text;// = new char[ GetLBTextLen( GetCurSel() ) + 1 ];
  GetLBText( GetCurSel(), text );

  std::string str = TSLUTF8Encoder(text);
  std::string str2, str3;
  size_t pos;

  pos = str.find(":");    // position of ":" in str
  str3 = str.substr (pos+1);   // get from ":" to the end

  m_scale = atoi(str3.c_str());

  m_scaleChanged = true;
  if(m_pParent)
  {
    ::InvalidateRect( m_pParent->m_hWnd, NULL, FALSE );
  }
}

void ZoomScale::setScale(const char *buffer)
{
  if (m_currentScaleAdded)
  {
    DeleteString(0);
  }
  else
  {
    m_currentScaleAdded = true;
  }
  InsertString(0, (LPCTSTR)TSLUTF8Decoder(buffer));
  SetCurSel(0);
}