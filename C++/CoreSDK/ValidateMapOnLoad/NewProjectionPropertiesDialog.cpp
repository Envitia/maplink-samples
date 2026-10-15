// NewProjectionPropertiesDialog.cpp : implementation file
//

#include "stdafx_tlt.h"
#include "NewProjectionPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NewProjectionPropertiesDialog dialog


NewProjectionPropertiesDialog::NewProjectionPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NewProjectionPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(NewProjectionPropertiesDialog)
	m_maxDist = 3704000.0;  // 2000 nautical miles
	m_initialLat = -15.0;   // nice view of South America
	m_initialLong = -62.0;     // avoid 60.0 dead on as this is likely to be ona tile boundary: 
                                   // effect of greatCircleDistance will not be apparent
	m_projectionChoice = 0;
	//}}AFX_DATA_INIT
}


void NewProjectionPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NewProjectionPropertiesDialog)
	DDX_Text(pDX, IDC_MAXDIST, m_maxDist);
	DDV_MinMaxDouble(pDX, m_maxDist, 0., 99999999.9);
	DDX_Text(pDX, IDC_INITIALLAT, m_initialLat);
	DDV_MinMaxDouble(pDX, m_initialLat, -90., 90.);
	DDX_Text(pDX, IDC_INITIALLONG, m_initialLong);
	DDV_MinMaxDouble(pDX, m_initialLong, -180., 180.);
	DDX_CBIndex(pDX, IDC_PROJECTION, m_projectionChoice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NewProjectionPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(NewProjectionPropertiesDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NewProjectionPropertiesDialog message handlers
