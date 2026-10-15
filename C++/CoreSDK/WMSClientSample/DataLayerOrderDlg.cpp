// DataLayerOrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WMSClientSample.h"
#include "DataLayerOrderDlg.h"
#include "WMSClientSampleView.h"

// CDataLayerOrderDlg dialog

IMPLEMENT_DYNAMIC(CDataLayerOrderDlg, CDialog)

CDataLayerOrderDlg::CDataLayerOrderDlg(CWnd* pParent)
  : CDialog(CDataLayerOrderDlg::IDD, pParent)
{
}

CDataLayerOrderDlg::~CDataLayerOrderDlg()
{
}

void CDataLayerOrderDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_DATALAYERORDER, m_dataLayerOrderList);
  DDX_Control(pDX, IDC_TRANSPARENCY_SLIDER, m_transparencySlider);
}


BEGIN_MESSAGE_MAP(CDataLayerOrderDlg, CDialog)
  ON_LBN_SELCHANGE(IDC_DATALAYERORDER, &CDataLayerOrderDlg::OnLbnSelchangeDatalayerorder)
  ON_BN_CLICKED(IDC_DATALAYER_TOPBUTTON, &CDataLayerOrderDlg::OnBnClickedDatalayerTopbutton)
  ON_BN_CLICKED(IDC_DATALAYER_UPBUTTON, &CDataLayerOrderDlg::OnBnClickedDatalayerUpbutton)
  ON_BN_CLICKED(IDC_DATALAYER_DOWNBUTTON, &CDataLayerOrderDlg::OnBnClickedDatalayerDownbutton)
  ON_BN_CLICKED(IDC_DATALAYER_BOTTOMBUTTON, &CDataLayerOrderDlg::OnBnClickedDatalayerBottombutton)
  ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TRANSPARENCY_SLIDER, &CDataLayerOrderDlg::OnNMReleasedcaptureTransparencySlider)
END_MESSAGE_MAP()



WMSClientSampleView* getActiveView()
{
  WMSClientSampleApp* app = (WMSClientSampleApp*)AfxGetApp();
  ASSERT(app);
  CFrameWnd* frameWnd = (CFrameWnd*)app->GetMainWnd();
  ASSERT(frameWnd);
  WMSClientSampleView * view = (WMSClientSampleView*)frameWnd->GetActiveView();
  ASSERT(view);
  return view;
}

void CDataLayerOrderDlg::refresh()
{
  m_dataLayerOrderList.ResetContent();

  WMSClientSampleView* view = getActiveView();
  if (TSLNTSurface* drawingSurface = view->drawingSurface())
  {
    int numlayers = drawingSurface->getNumDataLayers();
    for( int i = 0; i < numlayers; ++i )
    {
      TSLDataLayer* layer = 0;
      const char* layerName = 0;
      drawingSurface->getDataLayerInfo( numlayers-i-1, &layer, &layerName );
      ASSERT( layer && layerName );
      m_dataLayerOrderList.InsertString( i, (LPCTSTR)TSLUTF8Decoder(layerName) );
    }
  }

  OnLbnSelchangeDatalayerorder();
}

// CDataLayerOrderDlg message handlers


BOOL CDataLayerOrderDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  HMODULE thisMod = ::GetModuleHandle(0);
  BOOL ok =  m_upButton.SubclassDlgItem(IDC_DATALAYER_UPBUTTON, this) &&
             m_downButton.SubclassDlgItem(IDC_DATALAYER_DOWNBUTTON, this) &&
             m_topButton.SubclassDlgItem(IDC_DATALAYER_TOPBUTTON, this) &&
             m_bottomButton.SubclassDlgItem(IDC_DATALAYER_BOTTOMBUTTON, this);
  ASSERT( ok );
  m_upButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_UP), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                         (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_UPD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_downButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_DOWN), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                            (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_DOWND), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_topButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_TOP), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                          (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_TOPD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));
  m_bottomButton.SetBitmaps( (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_BOTTOM), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS), 0, 0,
                             (HBITMAP)::LoadImage(thisMod, MAKEINTRESOURCE(IDB_BOTTOMD), IMAGE_BITMAP, 16,16, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS));

  m_transparencySlider.SetRange(0, 255);
  m_transparencySlider.SetTicFreq(32);

  refresh();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDataLayerOrderDlg::DestroyWindow()
{
  m_upButton.UnsubclassWindow();
  m_downButton.UnsubclassWindow();
  m_topButton.UnsubclassWindow();
  m_bottomButton.UnsubclassWindow();

  return CDialog::DestroyWindow();
}

void CDataLayerOrderDlg::OnOK()
{
  ShowWindow( SW_HIDE );
}

void CDataLayerOrderDlg::OnCancel()
{
  ShowWindow( SW_HIDE );
}

void CDataLayerOrderDlg::OnLbnSelchangeDatalayerorder()
{
  m_upButton.EnableWindow(FALSE);
  m_downButton.EnableWindow(FALSE);
  m_topButton.EnableWindow(FALSE);
  m_bottomButton.EnableWindow(FALSE);
  m_transparencySlider.EnableWindow(FALSE);
  m_transparencySlider.SetPos(255);

  int selectedIndex = m_dataLayerOrderList.GetCurSel();
  if ( selectedIndex == -1 )
    return;

  int itemCount = m_dataLayerOrderList.GetCount();
  if ( selectedIndex != 0 && itemCount > 1 )
  {
    m_upButton.EnableWindow(TRUE);
    m_topButton.EnableWindow(TRUE);
  }
  if ( selectedIndex != itemCount-1 )
  {
    m_downButton.EnableWindow(TRUE);
    m_bottomButton.EnableWindow(TRUE);
  }

  CString layerName;
  m_dataLayerOrderList.GetText( selectedIndex, layerName );
  WMSClientSampleView* view = getActiveView();
  TSLNTSurface* drawingSurface = view->drawingSurface();
  ASSERT( drawingSurface );
  TSLPropertyValue transparency = 255;
  if (drawingSurface->getDataLayerProps( TSLUTF8Encoder(layerName), TSLPropertyTransparency, &transparency ))
  {
    m_transparencySlider.EnableWindow(TRUE);
    m_transparencySlider.SetPos(transparency);
  }
}


void CDataLayerOrderDlg::OnBnClickedDatalayerTopbutton()
{
  int currentSel = m_dataLayerOrderList.GetCurSel();
  if ( currentSel < 1 )
    return;

  CString layerName;
  m_dataLayerOrderList.GetText( currentSel, layerName );

  WMSClientSampleView* view = getActiveView();
  TSLNTSurface* drawingSurface = view->drawingSurface();
  ASSERT( drawingSurface );
  drawingSurface->bringToFront( TSLUTF8Encoder(layerName) );
  view->Invalidate();

  m_dataLayerOrderList.DeleteString(currentSel);
  m_dataLayerOrderList.InsertString(0, layerName);
  m_dataLayerOrderList.SetCurSel(0);
  OnLbnSelchangeDatalayerorder();
}

void CDataLayerOrderDlg::OnBnClickedDatalayerUpbutton()
{
  int currentSel = m_dataLayerOrderList.GetCurSel();
  if ( currentSel < 1 )
    return;

  CString layerName;
  m_dataLayerOrderList.GetText( currentSel, layerName );
  CString layerAboveName;
  m_dataLayerOrderList.GetText( currentSel-1, layerAboveName );

  WMSClientSampleView* view = getActiveView();
  TSLNTSurface* drawingSurface = view->drawingSurface();
  ASSERT( drawingSurface );
  drawingSurface->bringInFrontof( TSLUTF8Encoder(layerName), TSLUTF8Encoder(layerAboveName) );
  view->Invalidate();

  m_dataLayerOrderList.DeleteString(currentSel);
  m_dataLayerOrderList.InsertString(--currentSel, layerName.GetString());
  m_dataLayerOrderList.SetCurSel(currentSel);
  OnLbnSelchangeDatalayerorder();
}

void CDataLayerOrderDlg::OnBnClickedDatalayerDownbutton()
{
  int currentSel = m_dataLayerOrderList.GetCurSel();
  int itemCount = m_dataLayerOrderList.GetCount();
  if ( currentSel < 0 || currentSel == itemCount-1 )
    return;

  CString layerName;
  m_dataLayerOrderList.GetText( currentSel, layerName );
  CString layerBelowName;
  m_dataLayerOrderList.GetText( currentSel+1, layerBelowName );

  WMSClientSampleView* view = getActiveView();
  TSLNTSurface* drawingSurface = view->drawingSurface();
  ASSERT( drawingSurface );
  drawingSurface->sendToBackOf( TSLUTF8Encoder(layerName), TSLUTF8Encoder(layerBelowName) );
  view->Invalidate();

  m_dataLayerOrderList.DeleteString(currentSel);
  m_dataLayerOrderList.InsertString(++currentSel, layerName.GetString());
  m_dataLayerOrderList.SetCurSel(currentSel);
  OnLbnSelchangeDatalayerorder();
}

void CDataLayerOrderDlg::OnBnClickedDatalayerBottombutton()
{
  int currentSel = m_dataLayerOrderList.GetCurSel();
  int itemCount = m_dataLayerOrderList.GetCount();
  if ( currentSel < 0 || currentSel == itemCount-1 )
    return;

  CString layerName;
  m_dataLayerOrderList.GetText( currentSel, layerName );

  WMSClientSampleView* view = getActiveView();
  TSLNTSurface* drawingSurface = view->drawingSurface();
  ASSERT( drawingSurface );
  drawingSurface->sendToBack( TSLUTF8Encoder(layerName) );
  view->Invalidate();

  m_dataLayerOrderList.DeleteString(currentSel);
  m_dataLayerOrderList.InsertString(--itemCount, layerName.GetString());
  m_dataLayerOrderList.SetCurSel(itemCount);
  OnLbnSelchangeDatalayerorder();
}

void CDataLayerOrderDlg::OnNMReleasedcaptureTransparencySlider(NMHDR *pNMHDR, LRESULT *pResult)
{
  int selectedIndex = m_dataLayerOrderList.GetCurSel();
  if ( selectedIndex == -1 )
    return;

  CString layerName;
  m_dataLayerOrderList.GetText( selectedIndex, layerName );
  WMSClientSampleView* view = getActiveView();
  TSLNTSurface* drawingSurface = view->drawingSurface();
  ASSERT( drawingSurface );

  TSLPropertyValue transparency = m_transparencySlider.GetPos();

  drawingSurface->setDataLayerProps( TSLUTF8Encoder(layerName), TSLPropertyTransparency, transparency );
  view->Invalidate();

  *pResult = 0;
}
