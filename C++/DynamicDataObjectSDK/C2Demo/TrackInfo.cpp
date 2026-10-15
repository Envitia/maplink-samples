// TrackInfo.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TrackInfo.h"
#include "TrackManager.h"
#include "Track.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackInfo dialog


CTrackInfo::CTrackInfo(CWnd* pParent /*=NULL*/)
  : CDialog(CTrackInfo::IDD, pParent)
{
  //{{AFX_DATA_INIT(CTrackInfo)
  m_altitude = _T("");
  m_heading = _T("");
  m_trackNumber = _T("");
  m_velocity = _T("");
  m_latitude = _T("");
  m_longitude = _T("");
  //}}AFX_DATA_INIT

  m_updatingData = false ;

  m_oldAltitude	= 0.0 ;
  m_oldVelocity	= 0.0 ;
  m_oldHeading	= 0.0 ;
  m_oldTrackType	= (TRACK_TYPE)0 ;
  m_oldHostility	= (HOSTILE_TYPE)0 ;
}


CTrackInfo::~CTrackInfo()
{
}

void CTrackInfo::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CTrackInfo)
  DDX_Control(pDX, IDC_TRACKTYPE_COMBO, m_trackTypeCombo);
  DDX_Control(pDX, IDC_RESET, m_reset);
  DDX_Control(pDX, IDC_APPLY, m_apply);
  DDX_Control(pDX, IDC_HOSTILITY_COMBO, m_hostilityCombo);
  DDX_Text(pDX, IDC_ALTITUDE, m_altitude);
  DDX_Text(pDX, IDC_HEADING, m_heading);
  DDX_Text(pDX, IDC_TRACK_NUMBER, m_trackNumber);
  DDX_Text(pDX, IDC_VELOCITY, m_velocity);
  DDX_Text(pDX, IDC_LATITUDE, m_latitude);
  DDX_Text(pDX, IDC_LONGTITUDE, m_longitude);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackInfo, CDialog)
  //{{AFX_MSG_MAP(CTrackInfo)
  ON_BN_CLICKED(IDC_RESET, OnReset)
  ON_BN_CLICKED(IDC_APPLY, OnApply)
  ON_CBN_DROPDOWN(IDC_HOSTILITY_COMBO, OnDropdownHostilityCombo)
  ON_CBN_DROPDOWN(IDC_TRACKTYPE_COMBO, OnDropdownTracktypeCombo)
  ON_EN_SETFOCUS(IDC_HEADING, OnSetfocusHeading)
  ON_EN_SETFOCUS(IDC_VELOCITY, OnSetfocusVelocity)
  ON_EN_SETFOCUS(IDC_ALTITUDE, OnSetfocusAltitude)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackInfo message handlers

void CTrackInfo::OnReset() 
{
  // Write the stored data back to
  // the dialog controls
  m_altitude.Format(_T("%02.1f"), m_oldAltitude );
  m_velocity.Format(_T("%02.1f"), m_oldVelocity );
  m_heading.Format( _T("%02.1f"), m_oldHeading  );

  m_reset.EnableWindow( false ) ;
  m_apply.EnableWindow( false ) ;

  UpdateData( false ) ;

  m_updatingData = false ;

}

void CTrackInfo::OnApply() 
{
  UpdateData( true ) ;
  // Update the selected track
  CTrackManager::selectedTrack()->altitude( _tstof( m_altitude.GetBuffer(1) ) ) ;
  CTrackManager::selectedTrack()->velocity( _tstof( m_velocity.GetBuffer(1) ) ) ;
  CTrackManager::selectedTrack()->heading( _tstof( m_heading.GetBuffer(1) ) * PI / 180.0 ) ;

  // Set the track's type and hostility
  if ( m_oldTrackType != (TRACK_TYPE)m_trackTypeCombo.GetCurSel() )
  {
    CTrackManager::redesignateSelectedTrack( (TRACK_TYPE)m_trackTypeCombo.GetCurSel() ) ;
  }

  if ( m_oldHostility != (HOSTILE_TYPE)m_hostilityCombo.GetCurSel() ) 
  {
    CTrackManager::selectedTrack()->hostility( (HOSTILE_TYPE)m_hostilityCombo.GetCurSel() ) ;
  }

  m_apply.EnableWindow( false ) ;
  m_reset.EnableWindow( false ) ;

  m_updatingData = false ;

}

//////////////////////////////////////////////
// Get all the required data from the track
// manager for the currently selected track
//////////////////////////////////////////////
bool CTrackInfo::refreshData()
{
  if ( m_updatingData )
    return false ;

  if ( CTrackManager::selectedTrack() )
  {
    char buffer[256] ;
    buffer[0] = '\0' ;

    // Get the values from the selected track
    sprintf( buffer, "%02.1f", CTrackManager::selectedTrack()->altitude() ) ;
    m_altitude = buffer ;
    sprintf( buffer, "%02.1f", CTrackManager::selectedTrack()->velocity() ) ;
    m_velocity = buffer ;
    sprintf( buffer, "%02.1f", CTrackManager::selectedTrack()->heading() ) ;
    m_heading = buffer ;
    sprintf( buffer, "%d", CTrackManager::selectedTrack()->trackNumber() );
    m_trackNumber = buffer ;

    // Get the latitude and longitude as strings from the selected track
    CTrackManager::selectedTrack()->latitude( buffer ) ;
    m_latitude = buffer ;

    CTrackManager::selectedTrack()->longitude( buffer ) ;
    m_longitude = buffer ;

    // Update the combo boxes with the hostility and the track type
    m_hostilityCombo.SetCurSel( CTrackManager::selectedTrack()->hostility() ) ;
    m_trackTypeCombo.SetCurSel( CTrackManager::selectedTrack()->trackType() ) ;

    // If the track type is Unknown allow the user to set the track type
    m_trackTypeCombo.EnableWindow( CTrackManager::selectedTrack()->trackType() == UNKNOWN_TRACK ? TRUE : FALSE) ;
    // Unless this is the own ship, allow the user to change the hostility of the track
    m_hostilityCombo.EnableWindow( CTrackManager::selectedTrack()->trackType() == OWN_SHIP ? FALSE : TRUE ) ;

    UpdateData( false ) ;

    return true ;
  }

  return false ;

}

BOOL CTrackInfo::OnInitDialog() 
{
  CDialog::OnInitDialog();

  // Add the hostility types to the combo box
  int hostility ;
  for ( hostility = 0 ; hostility < NUM_HOSTILE_CODES ; ++hostility )
  {
    switch( hostility )
    {
    case HOSTILE :
      m_hostilityCombo.AddString( _T("Hostile") ) ;
      break ;
    case FRIENDLY :
      m_hostilityCombo.AddString( _T("Friendly") ) ;
      break ;
    case NEUTRAL :
      m_hostilityCombo.AddString( _T("Neutral") ) ;
      break ;
    case UNKNOWN :
      m_hostilityCombo.AddString( _T("Unknown") ) ;
    }
  }

  m_hostilityCombo.SetCurSel( 0 ) ;

  int trackType ;
  // NB: See definitions.h for the TRACK_TYPE enum defintion. The + 1 is 
  // because the last 2 definitions are for specific track types that have only
  // one instance in the application (i.e. OwnShip and RangeRing) therefore we define
  // NUM_TRACK_TYPES not to include them in normal track type processing.  Obviously we
  // can never select the RangeRings in the Tote so we can exclude that type here.
  for ( trackType = 0 ; trackType < NUM_TRACK_TYPES + 1 ; ++trackType )
  {
    switch ( trackType )
    {
    case AIRCRAFT_CARRIER :
      m_trackTypeCombo.AddString( _T("Aircraft Carrier") ) ;
      break ;
    case BOMBER :
      m_trackTypeCombo.AddString( _T("Bomber") ) ;
      break ;
    case DESTROYER :
      m_trackTypeCombo.AddString( _T("Destroyer") ) ;
      break ;
    case FRIGATE :
      m_trackTypeCombo.AddString( _T("Frigate") ) ;
      break ;
    case GENERAL_AIR :
      m_trackTypeCombo.AddString( _T("General Air") ) ;
      break ;
    case HELICOPTER :
      m_trackTypeCombo.AddString( _T("Helicopter") ) ;
      break ;
    case INTERCEPTOR :
      m_trackTypeCombo.AddString( _T("Interceptor") ) ;
      break ;
    case SUBMARINE :
      m_trackTypeCombo.AddString( _T("Submarine") ) ;
      break ;
    case UNKNOWN_TRACK :
      m_trackTypeCombo.AddString( _T("Unknown Track") ) ;
      break ;
    case OWN_SHIP :
      m_trackTypeCombo.AddString( _T("Own Ship") ) ;
      break ;
    }
  }

  m_trackTypeCombo.SetCurSel( 0 ) ;    

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

// Save current values before the user edits anything
void CTrackInfo::saveData()
{
  if ( CTrackManager::selectedTrack() )
  {
    UpdateData( false ) ;

    m_updatingData = true ;

    m_oldVelocity   = CTrackManager::selectedTrack()->velocity() ;
    m_oldAltitude   = CTrackManager::selectedTrack()->altitude() ;
    m_oldHeading    = CTrackManager::selectedTrack()->heading() ;
    m_oldHostility  = CTrackManager::selectedTrack()->hostility() ;
    m_oldTrackType  = CTrackManager::selectedTrack()->trackType() ;

  }
}

void CTrackInfo::OnDropdownHostilityCombo() 
{
  // If the user is editing the value,
  // enable the apply and reset buttons
  if ( !m_updatingData )
  { 
    saveData() ;
    m_apply.EnableWindow( true ) ;
    m_reset.EnableWindow( true ) ;
  }
}

void CTrackInfo::OnDropdownTracktypeCombo() 
{
  // If the user is editing the value,
  // enable the apply and reset buttons
  if ( !m_updatingData )
  { 
    saveData() ;
    m_apply.EnableWindow( true ) ;
    m_reset.EnableWindow( true ) ;
  }
}

void CTrackInfo::OnSetfocusHeading() 
{
  // If the user is editing the value,
  // enable the apply and reset buttons
  if ( !m_updatingData )
  { 
    saveData() ;
    m_apply.EnableWindow( true ) ;
    m_reset.EnableWindow( true ) ;
  }
}

void CTrackInfo::OnSetfocusVelocity() 
{
  // If the user is editing the value,
  // enable the apply and reset buttons
  if ( !m_updatingData )
  { 
    saveData() ;
    m_apply.EnableWindow( true ) ;
    m_reset.EnableWindow( true ) ;
  }
}

void CTrackInfo::OnSetfocusAltitude() 
{
  // If the user is editing the value,
  // enable the apply and reset buttons
  if ( !m_updatingData )
  { 
    saveData() ;
    m_apply.EnableWindow( true ) ;
    m_reset.EnableWindow( true ) ;
  }
}
