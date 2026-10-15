#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "afxdtctl.h"
#include "afxwin.h"

class S52StateObject;

#include "../s52/statecallbacks.h"

/////////////////////////////////////
// Test Plan Reference: 3.4.1.1
//  - Dialog for configuring the Workflow Sample
/////////////////////////////////////

class StateConfiguration : public CDialog
{
  DECLARE_DYNAMIC(StateConfiguration)

public:

  explicit StateConfiguration(S52StateObject * stateObject, WorkFlowCallback* workFlowCallback, DrawingSurfaceCallback *surfaceCallback, CWnd* pParent = NULL);
  virtual ~StateConfiguration();

  void disables53RenderingButton()
  {
    m_disables52RenderingButton = true;
  }
  //Overrides
  BOOL OnInitDialog();

  //Dialog Check Box Variables 
  BOOL  m_enableS52Rendering;
  BOOL  m_simpleSymbols;
  BOOL  m_simpleAreas;
  BOOL  m_contourLabels;
  BOOL  m_centreSymbols;
  BOOL  m_displayText;
  BOOL  m_displayTextHalos;
  BOOL  m_displaySymbols;

  //Dialog Text Box Variables 
  double m_deepContours;
  double m_shallowContours;
  double m_safetyDepth;
  double m_safetyContours;  
  int m_displayMask;
  int m_display; // colours

  TSLCoord m_ownership;

  //Check Box Controls
  CButton m_enableS52RenderingChkBox;
  CButton m_simpleSymbolsChkBox;
  CButton m_simpleAreasChkBox;
  CButton m_contourLabelsChkBox;
  CButton m_centreSymbolsChkBox;
  CButton m_displayTextChkBox;
  CButton m_displayTextHalosChkBox;
  CButton m_displaySymbolsChkBox;

  //Text Edit labels
  CStatic m_deepContoursLabel;
  CStatic m_shallowContoursLabel;
  CStatic m_safetyDepthLabel;
  CStatic m_safetyContoursLabel;

  //Text Edit Controls
  CEdit m_deepContoursEditBox;
  CEdit m_shallowContoursEditBox;
  CEdit m_safetyDepthEditBox;
  CEdit m_safetyContoursEditBox;
  
  //ID
  enum { IDD = IDD_STATE_CONFIGURATION };

  //Events
  afx_msg void OnBnClickedOk();
  afx_msg void OnEnableS52Rendering();
  afx_msg void OnNoDataToggle();
  afx_msg void OnSimpleSymbols();
  afx_msg void OnSimpleAreas();
  afx_msg void OnContourLabels();
  afx_msg void OnCentreSymbols();
  afx_msg void OnDisplayText();
  afx_msg void OnDisplaySymbols();
  afx_msg void OnDisplayMaskChanged();
  afx_msg void OnDisplayChanged();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  S52StateObject * m_stateObject;
  DrawingSurfaceCallback *m_surfaceCallback;
  WorkFlowCallback *m_workFlowCallback;

  bool m_disables52RenderingButton;

  void enableControls();

  void initialiseStateObject();
public:
  BOOL m_noDataToggle;
  afx_msg void OnDisplayTextHalos();
};
