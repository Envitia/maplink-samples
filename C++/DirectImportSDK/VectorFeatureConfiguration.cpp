// VectorFeatureConfiguration.cpp : implementation file
//

#include "stdafx.h"
#include "DirectImportSimpleViewer.h"
#include "VectorFeatureConfiguration.h"
#include "afxdialogex.h"
#include "AnalysisCallbacks.h"
#include "MyCustomClassifier.h"
#include "ColourHelper.h"

// VectorFeatureConfiguration dialog
IMPLEMENT_DYNAMIC(VectorFeatureConfiguration, CDialogEx)

VectorFeatureConfiguration::VectorFeatureConfiguration( TSLDirectImportDataLayer * dataLayer, TSLDirectImportDataSet * dataSet, TSLFeatureClassConfig * fcc, TSLClientFeatureClassifierCustom * myClassifier, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FEATURE_CONFIG, pParent)
  , m_dataLayer( dataLayer ), m_dataSet( dataSet ), m_fcc(fcc), m_myClassifier( myClassifier )
{
  // Pass the feature class config to the analyser
  m_analysisCallbacks.setFCC(m_fcc);

}

VectorFeatureConfiguration::~VectorFeatureConfiguration()
{
}

void VectorFeatureConfiguration::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_CUSTOM_FCC, m_customButton);
  // If we don't have a custom classifier, then disable the button
  m_customButton.EnableWindow( (m_myClassifier == NULL) ? FALSE : TRUE );
}
 

BEGIN_MESSAGE_MAP(VectorFeatureConfiguration, CDialogEx)
  ON_BN_CLICKED(IDC_GENERATE_FCC, &VectorFeatureConfiguration::OnBnClickedGenerateFcc)
  ON_BN_CLICKED(IDC_CUSTOM_FCC, &VectorFeatureConfiguration::OnBnClickedCustomFcc)
  ON_BN_CLICKED(IDC_LOAD_FCC, &VectorFeatureConfiguration::OnBnClickedLoadFcc)
END_MESSAGE_MAP()

// VectorFeatureConfiguration message handlers
void VectorFeatureConfiguration::OnBnClickedGenerateFcc()
{
  m_dataLayer->setAnalysisCallbacks(&m_analysisCallbacks);

  // Analyse data is asynchronous, so wait until it's finished.
  // By the end, the feature class config 
  m_dataLayer->analyseData(m_dataSet);
  ::WaitForSingleObject(m_analysisCallbacks.m_event->m_hObject, INFINITE);

  // Now set random colours for the features
  for (int i = 0; i < m_fcc->featureList().size(); ++i)
  {
    TSLRenderingAttributes attribs(m_fcc->featureList()[i]->getRendering());
    ColourHelper::setRandomColour(attribs);
    attribs.m_fillStyle = 503; // Translucent fill style
    attribs.m_exteriorEdgeColour = TSLComposeRGB(0x00, 0x00, 0x00); // With black edge
    m_fcc->featureList()[i]->setRendering(attribs);
  }

  m_dataLayer->setAnalysisCallbacks(0);

  OnOK();
}


void VectorFeatureConfiguration::OnBnClickedCustomFcc()
{ 
  // Seeing as we already know the features we can handle, we could just set up the
  // feature class config accordingly, but to get some basic styling for
  // features that we don't handle, just do the basic analysis as per 'Generate'
  // but leave the other features as the default without the random colours

  // Analyse data is asynchronous, so wait until it's finished.
  // By the end, the feature class config 
  m_dataLayer->setAnalysisCallbacks(&m_analysisCallbacks);
  m_dataLayer->analyseData(m_dataSet);
  ::WaitForSingleObject(m_analysisCallbacks.m_event->m_hObject, INFINITE);
  m_dataLayer->setAnalysisCallbacks(0);

  // Now add the classifier for OpenStreetMap ines - this will trigger the addition of any subclasses
  // that we know about.
  m_fcc->addClassifier("lines", TSLFeatureClassifierCustom::create( m_myClassifier ) ) ;

  OnOK();
}


void VectorFeatureConfiguration::OnBnClickedLoadFcc()
{
  CString directoryToOpen( TSLUtilityFunctions::getMapLinkHome() ) ;

  // MapLink Home may or may not have a trailing backslash, since it
  // may be passed on the command line.  Deal with either case
  if (directoryToOpen.GetLength() != 0 && directoryToOpen[directoryToOpen.GetLength() - 1] == '\\')
    directoryToOpen += _T("config\\directimport");
  else
    directoryToOpen += _T("\\config\\directimport");

  CString fileToOpen;

  CFileDialog opendlg(TRUE, _T(".fcc"), fileToOpen, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
    _T("Feature Class Config Files (*.fcc)|*.fcc||"), this, 0, 1);

  opendlg.m_ofn.lpstrInitialDir = directoryToOpen;
  if (opendlg.DoModal() == IDOK)
  {
    CString path = opendlg.GetPathName();
    TSLUTF8Encoder pathEncode(path);
    m_fcc->load(pathEncode);

    OnOK();
  }
}
