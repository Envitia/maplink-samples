// TestSetup.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../StdAfx.h"
#include "../resource.h"       // main symbols
#include "../s52/s52stateobject.h"
#include "s52/tsls52displaycategoryenum.h"
#include "StateConfiguration.h"


// TestConfiguration dialog

IMPLEMENT_DYNAMIC(StateConfiguration, CDialog)

StateConfiguration::StateConfiguration(S52StateObject * stateObject, WorkFlowCallback* workFlowCallback, DrawingSurfaceCallback *surfaceCallback, CWnd* pParent /*=NULL*/)
	: CDialog(StateConfiguration::IDD, pParent)
        , m_stateObject(stateObject)
        , m_enableS52Rendering(TRUE)
        , m_simpleAreas(0)
        , m_simpleSymbols(0)
        , m_contourLabels(0)
        , m_centreSymbols(0)
        , m_deepContours(0.0)
        , m_shallowContours(0.0)
        , m_safetyDepth(0.0)
        , m_safetyContours(0.0)
        , m_displayMask(2)
        , m_display(0)
        , m_surfaceCallback(surfaceCallback)
        , m_noDataToggle(TRUE)
        , m_workFlowCallback(workFlowCallback)
        , m_disables52RenderingButton(false)
        , m_displayTextHalos(FALSE)
{
}

StateConfiguration::~StateConfiguration()
{
}

BOOL StateConfiguration::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  initialiseStateObject();

  enableControls();

  return true;
}

void StateConfiguration::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //Check Boxes
  DDX_Control(pDX, IDC_CHECK_ENABLE_S52_RENDERING, m_enableS52RenderingChkBox);
  DDX_Check(pDX, IDC_CHECK_ENABLE_S52_RENDERING, m_enableS52Rendering);

  DDX_Control(pDX, IDC_CHECK_SIMPLE_SYMBOLS, m_simpleSymbolsChkBox);
  DDX_Check(pDX, IDC_CHECK_SIMPLE_SYMBOLS, m_simpleSymbols);

  DDX_Control(pDX, IDC_CHECK_SIMPLE_AREAS, m_simpleAreasChkBox);
  DDX_Check(pDX, IDC_CHECK_SIMPLE_AREAS, m_simpleAreas);

  DDX_Control(pDX, IDC_CHECK_CONTOUR_LABELS, m_contourLabelsChkBox);
  DDX_Check(pDX, IDC_CHECK_CONTOUR_LABELS, m_contourLabels);

  DDX_Control(pDX, IDC_CHECK_CENTRE_SYMBOLS, m_centreSymbolsChkBox);
  DDX_Check(pDX, IDC_CHECK_CENTRE_SYMBOLS, m_centreSymbols);

  DDX_Control(pDX, IDC_CHECK_DISABLE_TEXT, m_displayTextChkBox);
  DDX_Check(pDX, IDC_CHECK_DISABLE_TEXT, m_displayText);

  DDX_Control(pDX, IDC_CHECK_DISPLAY_TEXT_HALOS, m_displayTextHalosChkBox);
  DDX_Check(pDX, IDC_CHECK_DISPLAY_TEXT_HALOS, m_displayTextHalos);

  DDX_Control(pDX, IDC_CHECK_DISABLE_SYMBOLS, m_displaySymbolsChkBox);
  DDX_Check(pDX, IDC_CHECK_DISABLE_SYMBOLS, m_displaySymbols);

  // Radio buttons
  DDX_Radio(pDX, IDC_DM_DISPLAY_BASE, m_displayMask);
  DDX_Radio(pDX, IDC_DM_DAY_DISPLAY, m_display);

  //Labels
  DDX_Control(pDX, IDC_STATIC_DEEP_CONTOURS, m_deepContoursLabel);
  DDX_Control(pDX, IDC_STATIC_SHALLOW_CONTOURS, m_shallowContoursLabel);
  DDX_Control(pDX, IDC_STATIC_SAFETY_DEPTH, m_safetyDepthLabel);
  DDX_Control(pDX, IDC_STATIC_SAFETY_CONTOURS, m_safetyContoursLabel);

  //Text Edit Boxes
  DDX_Control(pDX, IDC_EDIT_DEEP_CONTOURS, m_deepContoursEditBox);
  DDX_Text(pDX, IDC_EDIT_DEEP_CONTOURS, m_deepContours);

  DDX_Control(pDX, IDC_EDIT_SHALLOW_CONTOURS, m_shallowContoursEditBox);
  DDX_Text(pDX, IDC_EDIT_SHALLOW_CONTOURS, m_shallowContours);

  DDX_Control(pDX, IDC_EDIT_SAFETY_DEPTH, m_safetyDepthEditBox);
  DDX_Text(pDX, IDC_EDIT_SAFETY_DEPTH, m_safetyDepth);

  DDX_Control(pDX, IDC_EDIT_SAFETY_CONTOURS, m_safetyContoursEditBox);
  DDX_Text(pDX, IDC_EDIT_SAFETY_CONTOURS, m_safetyContours);

  DDX_Check(pDX, IDC_CHECK_ENABLE_S52_NO_DATA_PATTERN, m_noDataToggle);
}


BEGIN_MESSAGE_MAP(StateConfiguration, CDialog)

  ON_BN_CLICKED(IDC_CHECK_ENABLE_S52_NO_DATA_PATTERN, OnNoDataToggle)
  ON_BN_CLICKED(IDC_CHECK_ENABLE_S52_RENDERING, OnEnableS52Rendering)
  ON_BN_CLICKED(IDC_CHECK_SIMPLE_SYMBOLS, OnSimpleSymbols)
  ON_BN_CLICKED(IDC_CHECK_SIMPLE_AREAS, OnSimpleAreas)
  ON_BN_CLICKED(IDC_CHECK_CONTOUR_LABELS, OnContourLabels)
  ON_BN_CLICKED(IDC_CHECK_CENTRE_SYMBOLS, OnCentreSymbols)
  ON_BN_CLICKED(IDC_CHECK_DISPLAY_TEXT, OnDisplayText)
  ON_BN_CLICKED(IDC_CHECK_DISPLAY_TEXT_HALOS, OnDisplayTextHalos)
  ON_BN_CLICKED(IDC_CHECK_DISPLAY_SYMBOLS, OnDisplaySymbols)
    
  ON_BN_CLICKED(IDC_DM_DISPLAY_BASE, OnDisplayMaskChanged)
  ON_BN_CLICKED(IDC_DM_STANDARD, OnDisplayMaskChanged)
  ON_BN_CLICKED(IDC_DM_OTHER, OnDisplayMaskChanged)
  ON_BN_CLICKED(IDC_DM_ALL, OnDisplayMaskChanged)

  ON_BN_CLICKED(IDC_DM_DAY_DISPLAY, OnDisplayChanged)
  ON_BN_CLICKED(IDC_DM_DUSK_DISPLAY, OnDisplayChanged)
  ON_BN_CLICKED(IDC_DM_DARK_DISPLAY, OnDisplayChanged)

  ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// TestSetup message handlers

void StateConfiguration::OnEnableS52Rendering()
{
  m_enableS52Rendering = (m_enableS52Rendering + 1) % 2;
  enableControls();
}

void StateConfiguration::OnNoDataToggle()
{
  m_noDataToggle = (m_noDataToggle + 1) % 2;
}

void StateConfiguration::initialiseStateObject()
{
  //check boxes
  m_simpleAreas = m_stateObject->SIMPLE_AREAS();
  m_simpleSymbols = m_stateObject->SIMPLE_SYMBOLS();
  m_contourLabels = m_stateObject->CONTOUR_LABELS();
  m_centreSymbols = m_stateObject->CENTRE_SYMBOLS();
  m_displayText = m_stateObject->SHOW_TEXT();
  m_displayTextHalos = m_stateObject->SHOW_TEXT_HALO();
  m_displaySymbols = m_stateObject->SHOW_SYMBOLS();

  //edit boxes
  m_deepContours = m_stateObject->DEEP_CONTOUR();
  m_shallowContours = m_stateObject->SHALLOW_CONTOUR();
  m_safetyDepth = m_stateObject->SAFETY_DEPTH();
  m_safetyContours = m_stateObject->SAFETY_CONTOUR();

  int displayMask = m_stateObject->DISPLAY_CATEGORY_MASK();

  if ( displayMask == TSLS52DisplayCategoryEnum_DisplayBase )
  {
    m_displayMask = 0;
  }
  else if ( displayMask > TSLS52DisplayCategoryEnum_DisplayBase && displayMask < TSLS52DisplayCategoryEnum_Other )
  {
    m_displayMask = 1;
  }
  else if ( displayMask >= TSLS52DisplayCategoryEnum_Other && displayMask < TSLS52DisplayCategoryEnum_MarinersStandard )
  {
    m_displayMask = 2; 
  }
  else
  {
    m_displayMask = 3;
  }

  m_display = m_stateObject->colourPalette();

  //check boxes
  m_enableS52RenderingChkBox.SetCheck(m_enableS52Rendering);
  m_simpleSymbolsChkBox.SetCheck(m_simpleSymbols);
  m_simpleAreasChkBox.SetCheck(m_simpleAreas);
  m_contourLabelsChkBox.SetCheck(m_contourLabels);
  m_centreSymbolsChkBox.SetCheck(m_centreSymbols);
  m_displayTextChkBox.SetCheck(m_displayText);
  m_displayTextHalosChkBox.SetCheck(m_displayTextHalos);
  m_displaySymbolsChkBox.SetCheck(m_displaySymbols);
  m_noDataToggle = m_stateObject->displayNoDataLayer();
}

void StateConfiguration::enableControls()
{
    //Check Boxes
  m_simpleSymbolsChkBox.EnableWindow(m_enableS52Rendering);
  m_simpleAreasChkBox.EnableWindow(m_enableS52Rendering);
  m_contourLabelsChkBox.EnableWindow(m_enableS52Rendering);
  m_centreSymbolsChkBox.EnableWindow(m_enableS52Rendering);
  m_displayTextChkBox.EnableWindow(m_enableS52Rendering);
  m_displayTextHalosChkBox.EnableWindow(m_enableS52Rendering);
  m_displaySymbolsChkBox.EnableWindow(m_enableS52Rendering);

  //Labels
  m_deepContoursLabel.EnableWindow(m_enableS52Rendering);
  m_shallowContoursLabel.EnableWindow(m_enableS52Rendering);
  m_safetyDepthLabel.EnableWindow(m_enableS52Rendering);
  m_safetyContoursLabel.EnableWindow(m_enableS52Rendering);

  //Text Edit Boxes
  m_deepContoursEditBox.EnableWindow(m_enableS52Rendering);
  m_shallowContoursEditBox.EnableWindow(m_enableS52Rendering);
  m_safetyDepthEditBox.EnableWindow(m_enableS52Rendering);
  m_safetyContoursEditBox.EnableWindow(m_enableS52Rendering);
  
  // Radio buttons - Display Mask
  GetDlgItem(IDC_DM_DISPLAY_BASE)->EnableWindow(m_enableS52Rendering);
  GetDlgItem(IDC_DM_STANDARD)->EnableWindow(m_enableS52Rendering);
  GetDlgItem(IDC_DM_OTHER)->EnableWindow(m_enableS52Rendering);
  GetDlgItem(IDC_DM_ALL)->EnableWindow(m_enableS52Rendering);

  // Radio buttons - Colour palettes
  GetDlgItem(IDC_DM_DAY_DISPLAY)->EnableWindow(m_enableS52Rendering);
  GetDlgItem(IDC_DM_DUSK_DISPLAY)->EnableWindow(m_enableS52Rendering);
  GetDlgItem(IDC_DM_DARK_DISPLAY)->EnableWindow(m_enableS52Rendering);

  if (m_disables52RenderingButton)
    GetDlgItem(IDC_CHECK_ENABLE_S52_RENDERING)->EnableWindow(false);
  //
  GetDlgItem(IDC_CHECK_ENABLE_S52_NO_DATA_PATTERN)->EnableWindow(m_enableS52Rendering);
    
  UpdateData(FALSE);
}

void StateConfiguration::OnBnClickedOk()
{
  CString deepContour, shallowContour, safetyDepth, safetyContour;
    
  m_deepContoursEditBox.GetWindowText(deepContour);
  m_shallowContoursEditBox.GetWindowText(shallowContour);
  m_safetyDepthEditBox.GetWindowText(safetyDepth);
  m_safetyContoursEditBox.GetWindowText(safetyContour);

  m_stateObject->setDeepContours(_ttoi(deepContour));
  m_stateObject->setShallowContours(_ttoi(shallowContour));
  m_stateObject->setSafetyDepth(_ttoi(safetyDepth));
  m_stateObject->setSafetyContours(_ttoi(safetyContour));

  if (m_surfaceCallback)
    m_surfaceCallback->noDataLayerVisible(m_noDataToggle);
  
  if (m_stateObject)
    m_stateObject->displayNoDataLayer((bool)m_noDataToggle);

  if (m_workFlowCallback)
    m_workFlowCallback->saveSettings(m_stateObject);

  CDialog::OnOK();
}

void StateConfiguration::OnSimpleAreas()
{
  m_simpleAreas = (m_simpleAreas + 1) % 2;
  m_stateObject->setSimpleAreas((bool)m_simpleAreas);
}

void StateConfiguration::OnSimpleSymbols()
{
  m_simpleSymbols = (m_simpleSymbols + 1) % 2;
  m_stateObject->setSimpleSymbols((bool)m_simpleSymbols);
}

void StateConfiguration::OnContourLabels()
{
  m_contourLabels = (m_contourLabels + 1) % 2;
  m_stateObject->setContourLabels((bool)m_contourLabels);
}

void StateConfiguration::OnCentreSymbols()
{
  m_centreSymbols = (m_centreSymbols + 1) % 2;
  m_stateObject->setCentreSymbols((bool)m_centreSymbols);
}

void StateConfiguration::OnDisplayText()
{
  m_displayText = (m_displayText + 1) % 2;
  m_stateObject->setShowText((bool)m_displayText);
}


void StateConfiguration::OnDisplayTextHalos()
{
  m_stateObject->setShowTextHalo(!m_stateObject->SHOW_TEXT_HALO());
}

void StateConfiguration::OnDisplaySymbols()
{
  m_displaySymbols = (m_displaySymbols + 1) % 2;
  m_stateObject->setShowSymbols((bool)m_displaySymbols);
}

void StateConfiguration::OnDisplayMaskChanged()
{
  UpdateData();

  Int displayMask = TSLS52DisplayCategoryEnum_DisplayBase;

  switch ( m_displayMask )
  {    
    case 3: // All
    {
      displayMask += TSLS52DisplayCategoryEnum_MarinersStandard;
      displayMask += TSLS52DisplayCategoryEnum_MarinersOther;
    }

    case 2: // Other - Deliberate Fallthrough
    {
      displayMask += TSLS52DisplayCategoryEnum_Other;
    }

    case 1: // Standard - Deliberate Fallthrough
    {
      displayMask += TSLS52DisplayCategoryEnum_Standard;
    }
  }
  
  m_stateObject->setDisplayMask(displayMask);
}

void StateConfiguration::OnDisplayChanged()
{
  UpdateData();

  string mapLinkConfigDir;

  if (!getMapLinkConfigDirectory(mapLinkConfigDir))
  {
    AfxMessageBox(_T("Unable to find the S52 configuration directory."), MB_ICONERROR ) ;
    return ;
  }
  mapLinkConfigDir += "/s52/";

  string palFile(mapLinkConfigDir);
  switch ( m_display )
  {    
    case 0: // day
    palFile += "s52day.pal";
    break;
    case 1: // dusk
    palFile += "s52dusk.pal";
    break;
    case 2: // dark
    palFile += "s52night.pal";
    break;
  }
  if (m_surfaceCallback)
    m_surfaceCallback->setupDynamicColours(palFile.c_str());
  m_stateObject->colourPalette(m_display);  
}
