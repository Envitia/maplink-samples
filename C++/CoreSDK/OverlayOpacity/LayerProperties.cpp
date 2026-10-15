// LayerProperties.cpp : implementation file
//

#include "stdafx.h"
#include "OverlayOpacity.h"
#include "LayerProperties.h"


// CLayerProperties dialog

IMPLEMENT_DYNAMIC(CLayerProperties, CDialogBar)

CLayerProperties::CLayerProperties()
:m_surface(0)
,m_selectedLayer(0)
{

}

CLayerProperties::~CLayerProperties()
{
}

void CLayerProperties::DoDataExchange(CDataExchange* pDX)
{
  CDialogBar::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LAYER_VISIBLE, m_layerVisible);
  DDX_Control(pDX, IDC_LAYER_BUFFERED, m_layerBuffered);
  DDX_Control(pDX, IDC_OPACITY_SLIDER, m_opacitySlider);
}

BOOL CLayerProperties::Create( CWnd* pParentWnd, UINT nIDTemplate,
                           UINT nStyle, UINT nID, BOOL bChange)
{
  if(!CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID))
    return FALSE;
  
  SetWindowText( _T("Properties") );
  return TRUE;
}

BOOL CLayerProperties::Create( CWnd* pParentWnd,
                           LPCTSTR lpszTemplateName, UINT nStyle,
                           UINT nID, BOOL bChange)
{
  if (!CDialogBar::Create( pParentWnd, lpszTemplateName,
    nStyle, nID))
    return FALSE;
  
  SetWindowText( _T("Properties") );
  return TRUE;
}

BEGIN_MESSAGE_MAP(CLayerProperties, CDialogBar)
  ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
  ON_BN_CLICKED(IDC_LAYER_BUFFERED, &CLayerProperties::OnBnClickedLayerBuffered)
  ON_BN_CLICKED(IDC_LAYER_VISIBLE, &CLayerProperties::OnBnClickedLayerVisible)
  ON_WM_HSCROLL()
END_MESSAGE_MAP()


LRESULT CLayerProperties::OnInitDialog(WPARAM wParam, LPARAM lParam) 
{
  if (!UpdateData(FALSE))
  {
    TRACE0("Warning: Update failed during dialog init.\n");
    return FALSE;
  }

  // Range of Slider is range of TSLPropertyTransparency
  // Between 0 and 255, where 255 is fully opaque and 0 is not visible.
  m_opacitySlider.SetRange(0, 255);
  m_opacitySlider.EnableWindow(FALSE);

  m_layerVisible.EnableWindow(FALSE);
  m_layerBuffered.EnableWindow(FALSE);

  return TRUE;
}

void CLayerProperties::Update(string * layerName, TSLDrawingSurface* surface)
{
  if ( surface )
  {
    m_surface = surface;
  }

  m_selectedLayer = layerName;
  if ( m_selectedLayer && m_surface )
  {
    m_opacitySlider.EnableWindow();
    m_layerVisible.EnableWindow();
    m_layerBuffered.EnableWindow();

    char buffer [256];
    long val;
    m_surface->getDataLayerProps(m_selectedLayer->c_str(), TSLPropertyTransparency, &val );

    sprintf(buffer, "%d", val);
    m_opacitySlider.SetPos( val );

    m_surface->getDataLayerProps(m_selectedLayer->c_str(), TSLPropertyVisible, &val );
    m_layerVisible.SetCheck(val);
    m_surface->getDataLayerProps(m_selectedLayer->c_str(), TSLPropertyBuffered, &val );
    m_layerBuffered.SetCheck(val);
  }
}

// CLayerProperties message handlers


void CLayerProperties::OnBnClickedLayerBuffered()
{
  if ( m_selectedLayer && m_surface )
  {
    m_surface->setDataLayerProps(m_selectedLayer->c_str(), 
                                 TSLPropertyBuffered, 
                                 m_layerBuffered.GetCheck( ) ? 1 : 0);
    ::AfxGetMainWnd()->Invalidate(0);
  }
}

void CLayerProperties::OnBnClickedLayerVisible()
{
  if ( m_selectedLayer && m_surface )
  {
    m_surface->setDataLayerProps(m_selectedLayer->c_str(), 
                                 TSLPropertyVisible, 
                                 m_layerVisible.GetCheck( ) ? 1 : 0); 

    ::AfxGetMainWnd()->Invalidate(0);
  }
}


void CLayerProperties::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  int val = ((CSliderCtrl*)pScrollBar)->GetPos();
  // Ensure slider value is within range
  // i.e. between 0 and 255, where 255 is fully opaque and 0 is not visible. 
  if ( val < 0 ) 
  {
    val = 0;
  }
  if ( val > 255 ) 
  {
    val = 255;
  }

  // Update on change
  long oldval;
  m_surface->getDataLayerProps(m_selectedLayer->c_str(), TSLPropertyTransparency, &oldval );
  if ( oldval != val )
  {
    if ( m_selectedLayer && m_surface )
    {
      m_surface->setDataLayerProps(m_selectedLayer->c_str(), 
                                    TSLPropertyTransparency, val );
    }
    ::AfxGetMainWnd()->Invalidate(0);
  }

  CDialogBar::OnHScroll(nSBCode, nPos, pScrollBar);
}
