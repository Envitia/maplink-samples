//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
#include "stdafx.h"
#include "TextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static const char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTextView, CView)

BEGIN_MESSAGE_MAP(CTextView, CScrollView)
  //{{AFX_MSG_MAP(CTextView)
  // NOTE - the ClassWizard will add and remove mapping macros here.
  //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
  // Standard printing commands
  ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

CTextView::CTextView()
: CScrollView()
, m_rectPrint(0, 0, 11520, -100)
, m_logFile(NULL)
, m_scrollToEndPosition(false)
, m_stringList()
{
  // TODO: add construction code here
} 

CTextView::~CTextView()
{
  if (m_logFile)
  {
    fprintf(m_logFile, "Log File Closed\n");
    fclose(m_logFile);
  }
}


BOOL CTextView::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs
  return CScrollView::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CTextView drawing
void CTextView::OnDraw(CDC* pDC)
{
  CGdiObject*font = pDC->SelectStockObject(SYSTEM_FONT);

  TEXTMETRIC tm;
  pDC->GetTextMetrics(&tm);
  const int nHeight = tm.tmHeight + tm.tmExternalLeading;

  const int numLines = m_stringList.size();

  CSize sizeTotal(m_rectPrint.Width(), nHeight * numLines);
  CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);   // page scroll
  CSize sizeLine(sizeTotal.cx / 100, sizeTotal.cy / 100); // line scroll
  SetScrollSizes(MM_TWIPS, sizeTotal, sizePage, sizeLine);

  for (int i = 0; i < numLines; i++) {
    pDC->TextOut(10, -i * nHeight, (LPCTSTR)TSLUTF8Decoder(m_stringList[i]));
  }
  pDC->SelectObject(font);

  if ( m_scrollToEndPosition ) 
  {          
    CPoint point(0, -(sizeTotal.cy));
    ScrollToPosition(point);
    m_scrollToEndPosition = false;
  }

}

void  CTextView::addLine( const std::string &line )
{
  if (m_logFile)
  {
    fprintf(m_logFile, "%s\n", line.c_str());
  }

  m_stringList.push_back(line);

  m_scrollToEndPosition = true;

  InvalidateRect( 0, false ) ;
}

void  CTextView::clear()
{
  m_stringList.clear();
  InvalidateRect( 0, false ) ;
}

bool CTextView::logFile(const std::string &logFile)
{
  if (m_logFile)
  {
    fprintf(m_logFile, "Log File Closed\n");
    fclose(m_logFile);
  }
  TSLSimpleString directory, basename, extension;
  TSLFileHelper::decomposePathname(logFile.c_str(), directory, basename, extension);
  TSLFileHelper::createDirectory(directory.c_str());
  m_logFile = TSLFileHelper::fopen(logFile.c_str(), "a+");
  if (!m_logFile)
  {
    return false;
  }
  fprintf(m_logFile, "Log File Opened\n");
  return true;
}


void CTextView::OnInitialUpdate()
{
  CScrollView::OnInitialUpdate();
  CSize sizeTotal(m_rectPrint.Width(), -m_rectPrint.Height());
  CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);   // page scroll
  CSize sizeLine(sizeTotal.cx / 100, sizeTotal.cy / 100); // line scroll
  SetScrollSizes(MM_TWIPS, sizeTotal, sizePage, sizeLine);
}

/////////////////////////////////////////////////////////////////////////////
// CHexView printing
BOOL CTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

void CTextView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CTextView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}

/////////////////////////////////////////////////////////////////////////////
// CTextView diagnostics
#ifdef _DEBUG
void CTextView::AssertValid() const
{
  CScrollView::AssertValid();
}
void CTextView::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextView message handlers