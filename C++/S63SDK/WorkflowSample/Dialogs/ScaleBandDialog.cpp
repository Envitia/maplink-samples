// TestSetup.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "ScaleBandDialog.h"


// ScaleBandDialog dialog

IMPLEMENT_DYNAMIC(ScaleBandDialog, CDialog)

ScaleBandDialog::ScaleBandDialog(S63Workflow * workflow, CWnd* pParent /*=NULL*/)
	: CDialog(ScaleBandDialog::IDD, pParent)
        , m_workFlow(workflow)
        , m_overviewLowerInput()
        , m_overviewUpperInput()
        , m_generalLowerInput()
        , m_generalUpperInput()
        , m_coastalLowerInput()
        , m_coastalUpperInput()
        , m_approachLowerInput()
        , m_approachUpperInput()
        , m_harbourLowerInput()
        , m_harbourUpperInput()
        , m_berthingLowerInput()
        , m_berthingUpperInput()
{

}

ScaleBandDialog::~ScaleBandDialog()
{
  m_workFlow = NULL;
}

BOOL ScaleBandDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  setScaleBandValues();
  return TRUE;
}

void ScaleBandDialog::setScaleBandValues()
{
  const TSLS63DataLayerScaleBands * scaleBands = m_workFlow->getScaleBands();
  
  double lower;
  double upper;

  CString lowerStr;
  CString upperStr;

  // Overview
  scaleBands->getBand(TSLS63ScaleBandOverview, lower, upper);

  // Convert the double to a string.
  lowerStr.Format( _T("%.2f"), lower );
  upperStr.Format( _T("%.2f"), upper );

  // Setup the controls on the dialog.
  m_overviewLowerInput.SetWindowText(lowerStr);
  m_overviewUpperInput.SetWindowText(upperStr);

  // General
  scaleBands->getBand(TSLS63ScaleBandGeneral, lower, upper);

  // Convert the double to a string.
  lowerStr.Format( _T("%.2f"), lower );
  upperStr.Format( _T("%.2f"), upper );

  // Setup the controls on the dialog.
  m_generalLowerInput.SetWindowText(lowerStr);
  m_generalUpperInput.SetWindowText(upperStr);

  // Coastal
  scaleBands->getBand(TSLS63ScaleBandCoastal, lower, upper);

  // Convert the double to a string.
  lowerStr.Format( _T("%.2f"), lower );
  upperStr.Format( _T("%.2f"), upper );

  // Setup the controls on the dialog.
  m_coastalLowerInput.SetWindowText(lowerStr);
  m_coastalUpperInput.SetWindowText(upperStr);

  // Approach
  scaleBands->getBand(TSLS63ScaleBandApproach, lower, upper);

  // Convert the double to a string.
  lowerStr.Format( _T("%.2f"), lower );
  upperStr.Format( _T("%.2f"), upper );

  // Setup the controls on the dialog.
  m_approachLowerInput.SetWindowText(lowerStr);
  m_approachUpperInput.SetWindowText(upperStr);

  // Harbour
  scaleBands->getBand(TSLS63ScaleBandHarbour, lower, upper);

  // Convert the double to a string.
  lowerStr.Format( _T("%.2f"), lower );
  upperStr.Format( _T("%.2f"), upper );

  // Setup the controls on the dialog.
  m_harbourLowerInput.SetWindowText(lowerStr);
  m_harbourUpperInput.SetWindowText(upperStr);

  // Berthing
  scaleBands->getBand(TSLS63ScaleBandBerthing, lower, upper);

  // Convert the double to a string.
  lowerStr.Format( _T("%.2f"), lower );
  upperStr.Format( _T("%.2f"), upper );

  // Setup the controls on the dialog.
  m_berthingLowerInput.SetWindowText(lowerStr);
  m_berthingUpperInput.SetWindowText(upperStr);
}

void ScaleBandDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_OVERVIEW_LOWER, m_overviewLowerInput);
  DDX_Control(pDX, IDC_OVERVIEW_UPPER, m_overviewUpperInput);
  DDX_Control(pDX, IDC_GENERAL_LOWER, m_generalLowerInput);
  DDX_Control(pDX, IDC_GENERAL_UPPER, m_generalUpperInput);
  DDX_Control(pDX, IDC_COASTAL_LOWER, m_coastalLowerInput);
  DDX_Control(pDX, IDC_COASTAL_UPPER, m_coastalUpperInput);
  DDX_Control(pDX, IDC_APPROACH_LOWER, m_approachLowerInput);
  DDX_Control(pDX, IDC_APPROACH_UPPER, m_approachUpperInput);
  DDX_Control(pDX, IDC_HARBOUR_LOWER, m_harbourLowerInput);
  DDX_Control(pDX, IDC_HARBOUR_UPPER, m_harbourUpperInput);
  DDX_Control(pDX, IDC_BERTHING_LOWER, m_berthingLowerInput);
  DDX_Control(pDX, IDC_BERTHING_UPPER, m_berthingUpperInput);
}


BEGIN_MESSAGE_MAP(ScaleBandDialog, CDialog)
  ON_BN_CLICKED(IDOK, &ScaleBandDialog::OnBnClickedOk)
  ON_BN_CLICKED(ID_RESET_SCALE_BANDS, &ScaleBandDialog::OnBnResetScaleBandsClicked)
END_MESSAGE_MAP()

void ScaleBandDialog::OnBnClickedOk()
{
  CString upperStr, lowerStr;
  double upper, lower;

  m_workFlow->clearScaleBands();

  // Overview
  m_overviewLowerInput.GetWindowText(lowerStr);
  m_overviewUpperInput.GetWindowText(upperStr);

  lower = _tstof(lowerStr);
  upper = _tstof(upperStr);

  m_workFlow->setScaleBand(TSLS63ScaleBandOverview, lower, upper);

  // General
  m_generalLowerInput.GetWindowText(lowerStr);
  m_generalUpperInput.GetWindowText(upperStr);

  lower = _tstof(lowerStr);
  upper = _tstof(upperStr);

  m_workFlow->setScaleBand(TSLS63ScaleBandGeneral, lower, upper);

  // Coastal
  m_coastalLowerInput.GetWindowText(lowerStr);
  m_coastalUpperInput.GetWindowText(upperStr);

  lower = _tstof(lowerStr);
  upper = _tstof(upperStr);

  m_workFlow->setScaleBand(TSLS63ScaleBandCoastal, lower, upper);

  // Approach
  m_approachLowerInput.GetWindowText(lowerStr);
  m_approachUpperInput.GetWindowText(upperStr);

  lower = _tstof(lowerStr);
  upper = _tstof(upperStr);

  m_workFlow->setScaleBand(TSLS63ScaleBandApproach, lower, upper);

  // Harbour
  m_harbourLowerInput.GetWindowText(lowerStr);
  m_harbourUpperInput.GetWindowText(upperStr);

  lower = _tstof(lowerStr);
  upper = _tstof(upperStr);

  m_workFlow->setScaleBand(TSLS63ScaleBandHarbour, lower, upper);

  // Berthing
  m_berthingLowerInput.GetWindowText(lowerStr);
  m_berthingUpperInput.GetWindowText(upperStr);

  lower = _tstof(lowerStr);
  upper = _tstof(upperStr);

  m_workFlow->setScaleBand(TSLS63ScaleBandBerthing, lower, upper);

  CDialog::OnOK();
}

void ScaleBandDialog::resetScaleBandValues()
{
  m_workFlow->setScaleBand(TSLS63ScaleBandOverview, OVERVIEW_LOWER, OVERVIEW_UPPER);
  m_workFlow->setScaleBand(TSLS63ScaleBandGeneral, GENERAL_LOWER, GENERAL_UPPER);
  m_workFlow->setScaleBand(TSLS63ScaleBandCoastal, COASTAL_LOWER, COASTAL_UPPER);
  m_workFlow->setScaleBand(TSLS63ScaleBandApproach, APPROACH_LOWER, APPROACH_UPPER);
  m_workFlow->setScaleBand(TSLS63ScaleBandHarbour, HARBOUR_LOWER, HARBOUR_UPPER);
  m_workFlow->setScaleBand(TSLS63ScaleBandBerthing, BERTHING_LOWER, BERTHING_UPPER);

  setScaleBandValues();
}

void ScaleBandDialog::OnBnResetScaleBandsClicked()
{
  resetScaleBandValues();
}
