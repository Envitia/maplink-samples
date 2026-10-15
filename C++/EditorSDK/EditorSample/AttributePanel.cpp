// AttributePanel.cpp : implementation file
//

#include "stdafx.h"
#include "EditorSample.h"
#include "AttributePanel.h"
#include "OpMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttributePanel dialog

CAttributePanel::CAttributePanel()
: CDialogBar( ), m_surface(0)
{
  //{{AFX_DATA_INIT(CAttributePanel)
  m_textHeight = _T("");
  //}}AFX_DATA_INIT
}

CAttributePanel::~CAttributePanel()
{
  for( int i=0; i < 2; ++i)
  {
    StylesMapIterator j ;

    for ( j = m_lineStyles[i].begin() ; j != m_lineStyles[i].end() ; ++j )
      ::DeleteObject( (*j).second ) ;

    for ( j = m_fillStyles[i].begin() ; j != m_fillStyles[i].end() ; ++j )
      ::DeleteObject( (*j).second ) ;

    for ( j = m_fontStyles[i].begin() ; j != m_fontStyles[i].end() ; ++j )
      ::DeleteObject( (*j).second ) ;
  }

  if ( m_surface )
    delete m_surface;
}

void CAttributePanel::DoDataExchange(CDataExchange* pDX)
{
  CDialogBar::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAttributePanel)
	DDX_Control(pDX, IDC_TEXT_BACKGROUND_STYLE, m_textBackgroundStyle);
	DDX_Control(pDX, IDC_TEXT_BACKGROUND_MODE, m_textBackgroundMode);
	DDX_Control(pDX, IDC_TEXT_BACKGROUND_EDGE_COLOUR, m_textBackgroundEdgeColour);
	DDX_Control(pDX, IDC_TEXT_BACKGROUND_COLOUR, m_textBackgroundColour);
	DDX_Control(pDX, IDC_TEXT_VALIGN, m_valign);
	DDX_Control(pDX, IDC_TEXT_HALIGN, m_halign);
	DDX_Control(pDX, IDC_TEXT_FONT, m_textFont);
	DDX_Control(pDX, IDC_TEXT_COLOUR, m_textColour);
        DDX_Control(pDX, IDC_FILL_STYLE, m_fillStyle);
        DDX_Control(pDX, IDC_EDGE_STYLE, m_edgeStyle);
        DDX_Control(pDX, IDC_EDGE_COLOUR, m_edgeColour);
        DDX_Control(pDX, IDC_FILL_COLOUR, m_fillColour);
        DDX_Text(pDX, IDC_EDGE_THICKNESS, m_edgeThickness);
        DDV_MinMaxInt(pDX, m_edgeThickness, 0, 25);
	DDX_Text(pDX, IDC_TEXT_HEIGHT, m_textHeight);
	DDV_MaxChars(pDX, m_textHeight, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttributePanel, CDialogBar)
        //{{AFX_MSG_MAP(CAttributePanel)
        ON_CBN_SELCHANGE(IDC_EDGE_COLOUR, OnSelchangeEdgeColour)
        ON_CBN_SELCHANGE(IDC_EDGE_STYLE, OnSelchangeEdgeStyle)
        ON_CBN_SELCHANGE(IDC_FILL_COLOUR, OnSelchangeFillColour)
        ON_WM_DRAWITEM()
        ON_CBN_SELCHANGE(IDC_FILL_STYLE, OnSelchangeFillStyle)
        ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
        ON_EN_CHANGE(IDC_EDGE_THICKNESS, OnChangeEdgeThickness)
	ON_CBN_SELCHANGE(IDC_TEXT_COLOUR, OnSelchangeTextColour)
	ON_EN_CHANGE(IDC_TEXT_HEIGHT, OnChangeTextHeight)
	ON_CBN_SELCHANGE(IDC_TEXT_FONT, OnSelchangeTextFont)
	ON_CBN_SELCHANGE(IDC_TEXT_HALIGN, OnSelchangeTextHalign)
	ON_CBN_SELCHANGE(IDC_TEXT_VALIGN, OnSelchangeTextValign)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_WM_MEASUREITEM()
	ON_CBN_SELCHANGE(IDC_TEXT_BACKGROUND_EDGE_COLOUR, OnSelchangeTextBackgroundEdgeColour)
	ON_CBN_SELCHANGE(IDC_TEXT_BACKGROUND_MODE, OnSelchangeTextBackgroundMode)
	ON_CBN_SELCHANGE(IDC_TEXT_BACKGROUND_STYLE, OnSelchangeTextBackgroundStyle)
	ON_CBN_SELCHANGE(IDC_TEXT_BACKGROUND_COLOUR, OnSelchangeTextBackgroundColour)
	//}}AFX_MSG_MAP
        ON_MESSAGE( WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttributePanel message handlers

LRESULT CAttributePanel::OnInitDialog( WPARAM wParam, LPARAM lParam )
{  
  HandleInitDialog(wParam, lParam);

  if (!UpdateData(FALSE))
  {
    TRACE0( "Warning: UpdateData failed during attribute panel init.\n" ) ;
    return FALSE ;
  }
  

  return TRUE;
}

bool CAttributePanel::initialiseAndCreate( CFrameWnd * frame )
{
  if ( !Create( frame, IDD_ATTRIBUTE_PANEL, WS_CHILD |CBRS_LEFT, IDD_ATTRIBUTE_PANEL ) )
    return false ;
  
  SetWindowText( _T("Attributes") );
  EnableDocking( CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT );
  
  frame->DockControlBar ( this, CBRS_ALIGN_LEFT );
  
  // tell the OpMgr about us
  COpMgr::setAttributePanel(this);
  
  return true;
}

void CAttributePanel::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
  UpdateDialogControls(pTarget, FALSE);
} 

void CAttributePanel::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
  if ( !m_colours.size() )
    return;
  
  CDC dc;
  CRect highlight = lpDrawItemStruct->rcItem ;
  CRect rcItem( highlight);
  rcItem.DeflateRect( 2, 2 );
  
  int index = lpDrawItemStruct->itemID ;
  
  if( index < 0 )
    return;
  
  dc.Attach(lpDrawItemStruct->hDC);
  dc.SetBkMode( TRANSPARENT ) ;
  
  if ( !(lpDrawItemStruct->itemState & ODS_SELECTED) )
  {
    dc.FillSolidRect(highlight, GetSysColor(COLOR_WINDOW) );
  }
  
  switch(nIDCtl)
  {
    case IDC_TEXT_FONT:
    case IDC_FILL_STYLE:
    case IDC_EDGE_STYLE:
    case IDC_TEXT_BACKGROUND_STYLE:
    {
      CBitmap bmp;

      if ( nIDCtl == IDC_EDGE_STYLE )
      {
	int style = m_edgeStyle.GetItemData(index);
	bmp.Attach( m_lineStyles[ (lpDrawItemStruct->itemState & ODS_SELECTED) ? 1 : 0][style] );
      }
      else if ( nIDCtl == IDC_TEXT_FONT )
      {
	int style = m_textFont.GetItemData(index);
	bmp.Attach( m_fontStyles[ (lpDrawItemStruct->itemState & ODS_SELECTED) ? 1 : 0][style] );
      }
      else if ( nIDCtl == IDC_FILL_STYLE )
      {
	int style = m_fillStyle.GetItemData(index);
	bmp.Attach( m_fillStyles[ (lpDrawItemStruct->itemState & ODS_SELECTED) ? 1 : 0][style] );
      }
      else
      {
	int style = m_textBackgroundStyle.GetItemData(index);
	bmp.Attach( m_fillStyles[ (lpDrawItemStruct->itemState & ODS_SELECTED) ? 1 : 0][style] );
      }
      
      CDC memdc;
      memdc.CreateCompatibleDC( &dc );
      CBitmap *poldBmp = memdc.SelectObject(&bmp);
      
      if( nIDCtl == IDC_EDGE_STYLE )
      {
	// nudge the src down so that the line is still centred despite the items being shorter
	// mix of rcItem & highlight is intentional to get it vertically centred but with left & right margin
	dc.BitBlt(rcItem.left, highlight.top, rcItem.Width(), highlight.Height(), &memdc, 0, (m_editControlHeight-m_listBoxItemHeight)/2,SRCCOPY);
      }
      else
      {
	dc.BitBlt(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height(), &memdc, 0,0,SRCCOPY);
      }
      
      memdc.SelectObject(poldBmp);
      
      // make sure we don't delete the bitmap
      bmp.Detach();
    }
    break;
       
  default:  // all the colour combos are handled in the same way
    {    
      dc.FillSolidRect( &rcItem, m_colours[ lpDrawItemStruct->itemID ].second );     
    }
    break;
  }
  
  if ( lpDrawItemStruct->itemState & ODS_SELECTED )
  {
    CPen pen( 0, 1, ::GetSysColor(COLOR_HIGHLIGHT) ) ;
    dc.SelectObject( pen ) ;
    dc.SelectObject( ::GetStockObject( NULL_BRUSH ) ) ;
    dc.Rectangle( &highlight ) ;
    highlight.DeflateRect(1,1);
    dc.Rectangle( &highlight ) ;
  }
  
  // windows gave us the dc so make sure we don't accidently delete it ourselves
  dc.Detach();
}

int CAttributePanel::lookupColour( long val )
{
  int i = m_colours.size() ; 
  
  while ( --i >= 0 )
  {
    if ( m_colours[i].first == val )
      return i ;
  }
  return 0 ;
}

void CAttributePanel::initialiseColours()
{  
  m_surface = new TSLNTSurface(m_hWnd, false);

  COLORREF highlightTextColour = GetSysColor(COLOR_HIGHLIGHTTEXT);
  m_highlightColour = GetSysColor(COLOR_HIGHLIGHT);

  m_highlightTextColourIndex = m_surface->getIDOfNearestColour( GetRValue(highlightTextColour), GetGValue(highlightTextColour), GetBValue(highlightTextColour) );
  m_highlightColourIndex = m_surface->getIDOfNearestColour( GetRValue(m_highlightColour), GetGValue(m_highlightColour), GetBValue(m_highlightColour) );

  COLORREF windowTextColour = GetSysColor(COLOR_WINDOWTEXT);
  m_windowColour = GetSysColor(COLOR_WINDOW);

  m_windowTextColourIndex = m_surface->getIDOfNearestColour( GetRValue(windowTextColour), GetGValue(windowTextColour), GetBValue(windowTextColour) );
  m_windowColourIndex = m_surface->getIDOfNearestColour( GetRValue(m_windowColour), GetGValue(m_windowColour), GetBValue(m_windowColour) );

  int   blackIndex = 1 ;
  int * indices = 0 ;
  int   num = 0 ;
  
  // Create the colours list
  if ( m_surface->getColourIndices( &indices, &num ) )
  {
    // Just take the main colours from the table
    for ( int i = 0 ; i < num ; ++i ) 
    {
      COLORREF rgb = m_surface->colourValue( indices[ i ] ) ;
      unsigned char red   = GetRValue( rgb ) ;
      unsigned char green = GetGValue( rgb ) ;
      unsigned char blue  = GetBValue( rgb ) ;
      
      if (   (red   == 0 || red   == 153 || red   == 255)
	  && (green == 0 || green == 153 || green == 255)
	  && (blue  == 0 || blue  == 153 || blue  == 255) )
      {
	m_colours.push_back( make_pair( indices[i], rgb ) ) ;
	m_edgeColour.AddString( _T("----") ) ;
	m_fillColour.AddString( _T("---") ) ;
	m_textColour.AddString( _T("----") ) ;
        m_textBackgroundColour.AddString( _T("---") ) ; 
        m_textBackgroundEdgeColour.AddString( _T("---") ) ; 
	
	if ( !red && !blue && !green )
	  blackIndex = indices[i] ;
      }
    }
  }
  
  CRect rc;
  m_edgeStyle.GetClientRect(&rc);
  int width = m_edgeStyle.GetDroppedWidth();

  width = max(rc.Width(), width);

  // Assume the edit control is taller than the list box items
  int height = m_editControlHeight;
  
  // Create the edge styles list
  if ( m_surface->getLineStyleIndices( &indices, &num ) )
  {
    // Add in <NONE> style
    m_lineStyles[0][0] = blankBitmap( width, height, m_windowColour )  ;
    m_lineStyles[1][0] = blankBitmap( width, height, m_highlightColour ) ; ;
    m_edgeStyle.AddString( (LPCTSTR)0 ) ;
    m_lineStylesToIndex[ 0 ] = 0;

    for ( int i = 0 ; i < num ; i++ )
    { 
      HBITMAP unselected = blankBitmap( width, height, m_windowColour ) ; 
      HBITMAP selected   = blankBitmap( width, height, m_highlightColour ) ; 

      m_surface->lineStyleValue(indices[i], m_windowTextColourIndex,    1, unselected ) ; 
      m_surface->lineStyleValue(indices[i], m_highlightTextColourIndex, 1, selected ) ;

      m_lineStyles[0][indices[i]] = unselected ;
      m_lineStyles[1][indices[i]] = selected ;
      
      m_edgeStyle.AddString( (LPCTSTR)indices[i] ) ;

      // populate the map we use to look up the initial selection
      m_lineStylesToIndex[indices[i]] = i + 1;
    }
  }
  
  // Create the fill styles list
  if ( m_surface->getFillStyleIndices( &indices, &num ) )
  {
    m_fillStyles[0][0] = blankBitmap( width, height, m_windowColour )  ;
    m_fillStyles[1][0] = blankBitmap( width, height, m_highlightColour ) ; ;
    m_fillStyle.AddString( (LPCTSTR)0 ) ;
    m_textBackgroundStyle.AddString( (LPCTSTR)0 ) ;
    m_fillStylesToIndex[ 0 ] = 0;

    for ( int i = 0 ; i < num ; i++ )
    {
      HBITMAP unselected = blankBitmap( width, height, m_windowColour ) ; 
      HBITMAP selected   = blankBitmap( width, height, m_highlightColour ) ; 

      m_surface->fillStyleValue(indices[i], m_windowTextColourIndex,    unselected ) ;
      m_surface->fillStyleValue(indices[i], m_highlightTextColourIndex, selected ) ;

      m_fillStyles[0][indices[i]] = unselected ; 
      m_fillStyles[1][indices[i]] = selected ;
    
      m_fillStyle.AddString( (LPCTSTR) indices[i]) ;
      m_textBackgroundStyle.AddString( (LPCTSTR) indices[i]) ;

      // populate the map we use to look up the initial selection
      m_fillStylesToIndex[indices[i]] = i + 1;
    }
  }

  // Create the font styles list
  if ( m_surface->getFontStyleIndices( &indices, &num ) )
  {
    for ( int i = 0 ; i < num ; i++ )
    {
      HBITMAP unselected = blankBitmap( width, height - 4, m_windowColour ) ; 
      HBITMAP selected   = blankBitmap( width, height - 4, m_highlightColour ) ; 

      m_surface->fontStyleValue( indices[i], m_windowTextColourIndex,     unselected ) ;
      m_surface->fontStyleValue( indices[i], m_highlightTextColourIndex,  selected   ) ;

      m_fontStyles[0][indices[i]] = unselected ;
      m_fontStyles[1][indices[i]] = selected ;

      m_textFont.AddString( (LPCTSTR) indices[i] ) ;

      m_fontStylesToIndex[indices[i]] = i;
    }
  }
  
  // Query the initial attributes from the config file and select the appropriate entry
  // from the combo boxes.
  int selected ;

  TSLProfileHelper::setDefaultSection( "initialAttributes" ) ;
  
  TSLProfileHelper::lookupProfile( "edgeColour", &selected, 1 ) ;
  m_edgeColour.SetCurSel( lookupColour( selected ) ) ;

  TSLProfileHelper::lookupProfile( "fillColour", &selected, 1 ) ;
  m_fillColour.SetCurSel( lookupColour( selected ) ) ;

  TSLProfileHelper::lookupProfile( "textColour", &selected, 1 ) ;
  m_textColour.SetCurSel( lookupColour( selected ) ) ;

  TSLProfileHelper::lookupProfile( "textFont", &selected, 1 ) ;
  m_textFont.SetCurSel( selected - 1 );

  // Set up the text size
  TSLProfileHelper::lookupProfile( "textHeight", &height, 1 ) ;
  if ( height < 1 )
    height = 1 ;
  else if ( height > 30 )
    height = 30 ;
  
  char buffer[ 12 ] ;
  sprintf( buffer, "%d", height ) ;
  m_textHeight = buffer ;
  
  TSLProfileHelper::lookupProfile( "edgeThickness", &m_edgeThickness, 1 ) ;
  
  TSLProfileHelper::lookupProfile( "edgeStyle", &selected, 1 ) ;
  m_edgeStyle.SetCurSel( m_lineStylesToIndex[selected] ) ;
  
  TSLProfileHelper::lookupProfile( "fillStyle", &selected, 1 ) ;
  m_fillStyle.SetCurSel( m_fillStylesToIndex[selected] ) ;

  int halign = 0 ;
  int valign = 0 ;
  TSLProfileHelper::lookupProfile( "textHorizontalAlignment", &halign, TSLHorizontalAlignmentLeft ) ;
  TSLProfileHelper::lookupProfile( "textVerticalAlignment",   &valign, TSLVerticalAlignmentMiddle ) ;
      
  m_halign.SetCurSel( halign ) ;
  m_valign.SetCurSel( valign ) ;

  TSLProfileHelper::lookupProfile( "textBackgroundMode", &selected, -1 ) ;
  m_textBackgroundMode.SetCurSel( selected ) ;

  TSLProfileHelper::lookupProfile( "textBackgroundStyle", &selected, -1 ) ;
  m_textBackgroundStyle.SetCurSel( m_fillStylesToIndex[selected]  ) ;

  TSLProfileHelper::lookupProfile( "textBackgroundColour", &selected, -1 ) ;
  m_textBackgroundColour.SetCurSel( lookupColour( selected ) ) ;

  TSLProfileHelper::lookupProfile( "textBackgroundEdgeColour", &selected, 1 ) ;
  m_textBackgroundEdgeColour.SetCurSel( lookupColour( selected ) ) ;
 
  UpdateData( FALSE ) ;
}

void CAttributePanel::OnSelchangeEdgeColour() 
{
  int sel = m_edgeColour.GetCurSel();
  
  COpMgr::edgeColour( m_colours[ sel ].first);
}

void CAttributePanel::OnSelchangeEdgeStyle() 
{
  int sel = m_edgeStyle.GetCurSel();
  
  COpMgr::edgeStyle( m_edgeStyle.GetItemData(sel) );
}

void CAttributePanel::OnSelchangeFillColour() 
{
  int sel = m_fillColour.GetCurSel();
  
  COpMgr::fillColour( m_colours[ sel ].first );
}

void CAttributePanel::OnSelchangeFillStyle() 
{
  int sel = m_fillStyle.GetCurSel();
  
  COpMgr::fillStyle( m_fillStyle.GetItemData(sel) );
}

void CAttributePanel::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
  NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
  
  *pResult = 0;
  
  m_edgeThickness -= pNMUpDown->iDelta ;
  
  if ( m_edgeThickness < 0 )
    m_edgeThickness = 0 ;
  else if ( m_edgeThickness > 30 )
    m_edgeThickness = 30 ;
  
  COpMgr::edgeThickness( m_edgeThickness ) ;
  
  // Update data since it may have been limited
  UpdateData(FALSE) ;
}

void CAttributePanel::OnChangeEdgeThickness() 
{
  UpdateData(TRUE) ;
  
  COpMgr::edgeThickness( m_edgeThickness ) ;
}

void CAttributePanel::OnSelchangeTextColour() 
{
  int sel = m_textColour.GetCurSel();
  
  COpMgr::textColour( m_colours[ sel ].first );
}

void CAttributePanel::OnChangeTextHeight() 
{
  UpdateData(TRUE) ;

  int textHeight = _ttoi( m_textHeight ) ;

  if ( textHeight < 0 )
	  textHeight = 0 ;
  else if ( textHeight > 30 )
	  textHeight = 30 ;

  COpMgr::textHeight( (textHeight ? textHeight : 1 ) * MM_SCALE_FACTOR ) ;

  char buffer[ 12 ] ;
  if ( textHeight > 0 )
	  sprintf( buffer, "%d", textHeight ) ;
  else
	  buffer[0] = '\0' ;

  m_textHeight = buffer ;

  // Update data since it may have changed
  UpdateData( FALSE ) ; 
}

void CAttributePanel::OnSelchangeTextFont() 
{
  int sel = m_textFont.GetCurSel();
  
  COpMgr::textFont( m_textFont.GetItemData(sel) );
}

void CAttributePanel::OnSelchangeTextHalign() 
{
  TSLHorizontalAlignment halign = static_cast< TSLHorizontalAlignment>( m_halign.GetCurSel() );
  TSLVerticalAlignment   valign = static_cast< TSLVerticalAlignment>( m_valign.GetCurSel() );
  
  COpMgr::textAlignment( halign, valign ) ;
}

void CAttributePanel::OnSelchangeTextValign() 
{
  TSLHorizontalAlignment halign = static_cast< TSLHorizontalAlignment>( m_halign.GetCurSel() );
  TSLVerticalAlignment   valign = static_cast< TSLVerticalAlignment>( m_valign.GetCurSel() );
  
  COpMgr::textAlignment( halign, valign ) ;
}

void CAttributePanel::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
  // This is the text height spin box
  NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
  
  *pResult = 0;

  int textHeight = _ttoi( m_textHeight ) ;
  textHeight -= pNMUpDown->iDelta ;
  
  if ( textHeight < 1 )
    textHeight = 1 ;
  else if ( textHeight > 30 )
    textHeight = 30 ;
  
  COpMgr::textHeight( textHeight *  MM_SCALE_FACTOR ) ;
  
  char buffer[ 12 ] ;
  sprintf( buffer, "%d", textHeight ) ;
  
  m_textHeight = buffer ;
  
  // Update data since it has changed
  UpdateData(FALSE) ;
}

void CAttributePanel::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
  // we're only handling this so we can find out what the default height is.
  if( lpMeasureItemStruct->itemID == (UINT)-1 )
  {
    // edit control item
    m_editControlHeight = lpMeasureItemStruct->itemHeight;
  }
  else
  {
    m_listBoxItemHeight = lpMeasureItemStruct->itemHeight;
  }	

  CDialogBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

HBITMAP CAttributePanel::blankBitmap( int width, int height, COLORREF backgroundColour)
{
  HDC tdc = ::GetDC( 0 );
  
  // create a memory DC compatible with the screen
  HDC hdcMemory = CreateCompatibleDC( tdc );
  
  // create a bitmap to return
  HBITMAP hBitmap = CreateCompatibleBitmap ( tdc, width, height);
  
  HBITMAP hbmpOld = static_cast< HBITMAP >(::SelectObject(hdcMemory, hBitmap));
  
  ::ReleaseDC( 0, tdc );
  
  // Fill it with the chosen background Colour
  RECT rc = {0,0,width + 1, height + 1};
  
  HBRUSH bkgBrush = ::CreateSolidBrush( backgroundColour );
  ::FillRect( hdcMemory, &rc, bkgBrush );

  // Now tidy up and return
  ::DeleteObject( bkgBrush );

  ::SelectObject(hdcMemory, hbmpOld);
  ::DeleteDC(hdcMemory);

  return hBitmap ;
}

void CAttributePanel::OnSelchangeTextBackgroundColour() 
{
  int sel = m_textBackgroundColour.GetCurSel();
  
  COpMgr::textBackgroundColour( m_colours[ sel ].first );
}

void CAttributePanel::OnSelchangeTextBackgroundEdgeColour() 
{
  int sel = m_textBackgroundEdgeColour.GetCurSel();
  
  COpMgr::textBackgroundEdgeColour( m_colours[ sel ].first );
}

void CAttributePanel::OnSelchangeTextBackgroundMode() 
{
  int sel = m_textBackgroundMode.GetCurSel();
  
  COpMgr::textBackgroundMode( static_cast< TSLTextBackgroundMode >( sel ) ) ;
}

void CAttributePanel::OnSelchangeTextBackgroundStyle() 
{
  int sel = m_textBackgroundStyle.GetCurSel();
  
  COpMgr::textBackgroundStyle( m_textBackgroundStyle.GetItemData(sel) );
}

