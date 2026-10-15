// S63WorkFlowView.cpp : implementation of the S63WorkFlowView class
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63WorkFlowSample.h"

#include "Mainfrm.h"
#include "S63WorkFlowDoc.h"
#include "S63WorkFlowView.h"
#include <sstream>

#include "s63\s63workflow.h"
#include "s63\s63workflowclient.h"

#include "dialogs\LoadS57Dialog.h"
#include "dialogs\LoadS63Dialog.h"
#include "dialogs\LargeMediaDialog.h"
#include "dialogs\LoadS57MediaDialog.h"
#include "dialogs\LoadMediaDialog.h"
#include "dialogs\TestSetup.h"
#include "dialogs\FileDialog.h"
#include "dialogs\TestConfiguration.h"
#include "dialogs\StateConfiguration.h"
#include "dialogs\IngestCell.h"
#include "dialogs\UserPermitDialog.h"
#include "dialogs\CertificateDialog.h"
#include "dialogs\ManageCellPermitsDialog.h"
#include "dialogs\CancelledCellDialog.h"
#include "dialogs\ScaleBandDialog.h"

#include "s52\s52stateobject.h"
#include "s52\s52userrenderingprocedure.h"

#include "S63/Mandated Tests/edition_1_1/s63userpermittests.h"
#include "S63/Mandated Tests/edition_1_1/s63cellpermittests.h"
#include "S63/Mandated Tests/edition_1_1/s63certificatetests.h"
#include "S63/Mandated Tests/edition_1_1/s63compressiontests.h"
#include "S63/Mandated Tests/edition_1_1/s63digitalsignaturetests.h"
#include "S63/Mandated Tests/edition_1_1/s63encryptiondecryptiontests.h"
#include "S63/Mandated Tests/edition_1_1/s63multipleservertests.h"

#include "S63/Mandated Tests/edition_1_2/s63ENCLicencingtests.h"
#include "S63/Mandated Tests/edition_1_2/s63ENCAuthenticationTests_Part1.h"
#include "S63/Mandated Tests/edition_1_2/s63ENCAuthenticationTests_Part2.h"
#include "S63/Mandated Tests/edition_1_2/s63ENCDecryptiontests.h"
#include "S63/Mandated Tests/edition_1_2/s63ENCDataManagementtests.h"
#include "S63/Mandated Tests/edition_1_2/s63ENCDataExchangeMediaTests.h"

using namespace S63OEMTests;

#include "tslatomic.h"
#include "tslcoord.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern S63WorkFlowApp theApp;

#define ELAPSED_TIME (5 * 1000)

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowView

IMPLEMENT_DYNCREATE(S63WorkFlowView, CView)

BEGIN_MESSAGE_MAP(S63WorkFlowView, CView)
//{{AFX_MSG_MAP(S63WorkFlowView)
  ON_COMMAND(ID_TOOLS_ZOOMIN, OnToolsZoomin)
  ON_COMMAND(ID_TOOLS_ZOOMOUT, OnToolsZoomout)
  ON_COMMAND(ID_TOOLS_RESET, OnToolsReset)
  ON_WM_SIZE()
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,    OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_RESET,   OnUpdateToolsAction)
  ON_COMMAND(ID_TOOLS_PAN, OnToolsPan)
  ON_COMMAND(ID_TOOLS_ZOOM, OnToolsZoom)
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_TOOLS_GRAB, OnToolsGrab)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMIN,  OnUpdateToolsAction)
  ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)

  //}}AFX_MSG_MAP

  // Disable printing
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT,    OnUpdateNeverEnabled)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW,    OnUpdateNeverEnabled)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_SETUP,    OnUpdateNeverEnabled)

  // Our common handlers can update multiple items.
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY,          OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOMOUT,      OnUpdateToolsAction)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM_TO_DATA, OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_PAN,          OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_ZOOM,         OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_GRAB,         OnUpdateToolsMode)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_MAGNIFY,      OnUpdateToolsMode)

  // Add the middle button handlers since DevStudio doesn't 
  // give us the option.
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MBUTTONUP()
  
  ON_COMMAND(ID_TOOLS_ZOOM_TO_DATA, OnZoomToData)
  ON_COMMAND(ID_TOOLS_MAGNIFY, OnToolsMagnify)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO1, OnSavedviewsGoto1)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO2, OnSavedviewsGoto2)
  ON_COMMAND(ID_SAVEDVIEWS_GOTO3, OnSavedviewsGoto3)
  ON_COMMAND(ID_SAVEDVIEWS_RESET, OnSavedviewsReset)
  ON_COMMAND(ID_SAVEDVIEWS_SET1, OnSavedviewsSet1)
  ON_COMMAND(ID_SAVEDVIEWS_SET2, OnSavedviewsSet2)
  ON_COMMAND(ID_SAVEDVIEWS_SET3, OnSavedviewsSet3)
  ON_COMMAND(ID_VIEWSTACK_NEXTVIEW, OnViewstackNextview)
  ON_COMMAND(ID_VIEWSTACK_PREVIOUSVIEW, OnViewstackPreviousview)

  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_RESET, OnUpdateSavedviewsReset)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO3, OnUpdateSavedviewsGoto3)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO2, OnUpdateSavedviewsGoto2)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_GOTO1, OnUpdateSavedviewsGoto1)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_SET1, OnUpdateSavedviewsSet1)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_SET2, OnUpdateSavedviewsSet2)
  ON_UPDATE_COMMAND_UI(ID_SAVEDVIEWS_SET3, OnUpdateSavedviewsSet3)
  ON_UPDATE_COMMAND_UI(ID_VIEWSTACK_NEXTVIEW, OnUpdateViewstackNextview)
  ON_UPDATE_COMMAND_UI(ID_VIEWSTACK_PREVIOUSVIEW, OnUpdateViewstackPreviousview)

  ON_WM_MOUSEWHEEL()

  //Configuration Dialog
  ON_UPDATE_COMMAND_UI(ID_CONFIGURATION, OnUpdateConfigurationDialog)

  ON_COMMAND(ID_CONFIGURATION, OnConfiguration)
  ON_COMMAND(ID_STATECONFIGURATION, OnStateConfiguration)

  //User Permits
  ON_UPDATE_COMMAND_UI(ID_USERPERMITGENERATION, OnUpdateAlwaysEnabled)

  ON_COMMAND(ID_USERPERMITGENERATION, OnGenerateUserPermit)

  //Certificates
  ON_UPDATE_COMMAND_UI(ID_WORKFLOW_INSTALLSACERTIFICATE, OnUpdateInstallSACertificate)
  ON_UPDATE_COMMAND_UI(ID_WORKFLOW_INSTALLDATASERVERCERTIFICATE, OnUpdateInstallDSCertificate)
  ON_UPDATE_COMMAND_UI(ID_WORKFLOW_REMOVESACERTIFICATE, OnUpdateRemoveSACertificate)
  ON_UPDATE_COMMAND_UI(ID_WORKFLOW_REMOVEDATASERVERCERTIFICATE, OnUpdateRemoveDSCertificate)

  ON_COMMAND(ID_WORKFLOW_INSTALLSACERTIFICATE, OnInstallSACertificate)
  ON_COMMAND(ID_WORKFLOW_INSTALLDATASERVERCERTIFICATE, OnInstallDSCertificate)
  ON_COMMAND(ID_WORKFLOW_REMOVESACERTIFICATE, OnRemoveSACertificate)
  ON_COMMAND(ID_WORKFLOW_REMOVEDATASERVERCERTIFICATE, OnRemoveDSCertificate)

  //Media Ingest
  ON_UPDATE_COMMAND_UI(ID_INSTALLUSERPERMIT, OnUpdateAlwaysEnabled)
  ON_UPDATE_COMMAND_UI(ID_INSTALLCELLPERMIT, OnUpdateAlwaysEnabled)
  ON_UPDATE_COMMAND_UI(ID_LOADMEDIA, OnUpdateLoadMedia)
  ON_UPDATE_COMMAND_UI(ID_LARGEMEDIAINGEST, OnUpdateLoadMedia)

  ON_COMMAND(ID_INSTALLUSERPERMIT, OnInstallUserPermit)
  ON_COMMAND(ID_INSTALLCELLPERMIT, OnInstallCellPermit)
  ON_COMMAND(ID_MEDIAINGEST_INGESTS57DATA, OnLoadS57Media)  
  ON_COMMAND(ID_LOADMEDIA, OnLoadMedia)
  ON_COMMAND(ID_LARGEMEDIAINGEST, OnLoadLargeMedia)

  //TUT
  ON_UPDATE_COMMAND_UI(ID_S63TESTCASES_RUN, OnUpdateTestSetup)
  ON_UPDATE_COMMAND_UI(ID_S63TESTCASES_SETUP, OnUpdateAlwaysEnabled)

  ON_COMMAND(ID_S63TESTCASES_SETUP, OnS63TestSetup)
  ON_COMMAND(ID_S63TESTCASES_RUN_EDITION_1_1, OnS63TestRunEdition_1_1)
  ON_COMMAND(ID_S63TESTCASES_RUN_EDITION_1_2, OnS63TestRunEdition_1_2)

  //S63 DATA
  ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_S57, OnUpdateAlwaysEnabled)
  ON_UPDATE_COMMAND_UI(ID_FILE_OPEN_S63, OnUpdateViewerEnabled)
  ON_UPDATE_COMMAND_UI(ID_CONFIGURE_SCALE_BANDS, OnUpdateAlwaysEnabled)

  ON_COMMAND(ID_FILE_OPEN_S63, OnLoadS63)
  ON_COMMAND(ID_FILE_OPEN_S57, OnLoadS57)
  ON_COMMAND(ID_CONFIGURE_SCALE_BANDS, OnConfigureScaleBands)  

  ON_UPDATE_COMMAND_UI(ID_CURRENT_SCALE, OnUpdateCurrentScale)

  ON_COMMAND(ID_VIEW_DEFAULTSCALEBANDDISPLAY, &S63WorkFlowView::OnViewScaleBandFixDefault)
  ON_COMMAND(ID_VIEW_SCALEBANDOVER, &S63WorkFlowView::OnViewScaleBandFixIgnore)
  ON_COMMAND(ID_VIEW_OVERVIEWSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixOverview)
  ON_COMMAND(ID_VIEW_GENERALSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixGeneral)
  ON_COMMAND(ID_VIEW_COASTALSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixCoastal)
  ON_COMMAND(ID_VIEW_APPROACHSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixApproach)
  ON_COMMAND(ID_VIEW_HARBOURSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixHarbour)
  ON_COMMAND(ID_VIEW_BERTHINGSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixBerthing)

  ON_UPDATE_COMMAND_UI(ID_VIEW_DEFAULTSCALEBANDDISPLAY, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SCALEBANDOVER, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_OVERVIEWSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_GENERALSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_COASTALSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_APPROACHSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_HARBOURSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixUpdate)
  ON_UPDATE_COMMAND_UI(ID_VIEW_BERTHINGSCALEBAND, &S63WorkFlowView::OnViewScaleBandFixUpdate)

  // Lat/Long grid
  ON_COMMAND(ID_VIEW_SHOWLAT, &S63WorkFlowView::OnViewShowLat)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWLAT, &S63WorkFlowView::OnUpdateViewShowLat)

    // MGRS grid
  ON_COMMAND( ID_VIEW_SHOWMGRSGRID, &S63WorkFlowView::OnViewShowMGRS )
  ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWMGRSGRID, &S63WorkFlowView::OnUpdateViewShowMGRS)

  ON_WM_ENTERIDLE()
  ON_COMMAND(IDC_REMOVE_S57_STORES, &S63WorkFlowView::OnRemoveS57Stores)
  END_MESSAGE_MAP()

bool S63WorkFlowView::m_disableWarnings = false;
bool S63WorkFlowView::m_useOpenGLSurface = false;
ProgressDialog *S63WorkFlowView::m_ingestProgressDialog = NULL;
TestProgressDialog S63WorkFlowView::m_testProgressDialog;

// These define the IDs of messages passed from background worker threads to the
// user interface thread of the workflow sample
#define WM_UPDATE_INGEST_PROGRESS_DIALOG (WM_USER + 1)
#define WM_CLOSE_INGEST_PROGRESS_DIALOG  (WM_USER + 2)
#define WM_UPDATE_TEST_PROGRESS_DIALOG   (WM_USER + 3)
#define WM_CLOSE_TEST_PROGRESS_DIALOG    (WM_USER + 4)
#define WM_LOG_MESSAGE                   (WM_USER + 5)

// This defines the number of tests that will be executed when the IHO tests are run
#define NUM_IHO_TESTS_1_1 19
#define NUM_IHO_TESTS_1_2 35

/////////////////////////////////////////////////////////////////////////////
// Structure to store arguments for the thread function
typedef struct IngestParameters
{
  IngestParameters( S63Workflow *workflow, const string &dataStore, const string &productsFile,
                    const string &serialFile, HWND &uiWindow )
  : m_workflow( workflow )
  , m_dataStore( dataStore )
  , m_productFile( productsFile )
  , m_serialFile( serialFile )
  , m_uiWindow( uiWindow )
  {
  }

  S63Workflow *m_workflow;
  string m_dataStore;
  string m_productFile;
  string m_serialFile;
  HWND m_uiWindow;

} IngestParameters;

// Structure to hold information to pass to the progress dialog during an ingest
typedef struct ProgressAmount
{
  double percentTotal;
  double percentOfCell;
} ProgressAmount;

// Structure to hold information relating to the IHO tests
typedef struct TestInformation
{
  TestInformation( const S63ConfigurationObject &configuration, HWND &uiWindow )
  : m_configuration( configuration )
  , m_uiWindow( uiWindow )
  {
  }

  S63ConfigurationObject m_configuration;
  HWND m_uiWindow;
} TestInformation;

/////////////////////////////////////////////////////////////////////////////
// Thread function to allow the user interface to remain responsive
// during an ingest. This function performs the actual media ingest and
// reports it's status to the UI thread via messages.

UINT _cdecl performIngest( void* arg )
{
  const IngestParameters *parameters = reinterpret_cast< IngestParameters* >( arg );
  const bool retval = parameters->m_workflow->startIngest( parameters->m_dataStore, parameters->m_productFile,
                                                     parameters->m_serialFile ) ? 1 : 0;

  // Send a message to the user interface indicating that it can close the progress dialog now.
  ::PostMessage( parameters->m_uiWindow, WM_CLOSE_INGEST_PROGRESS_DIALOG, 0, 0 );

  return retval;
}


UINT _cdecl performS57Ingest( void* arg )
{
  const IngestParameters *parameters = reinterpret_cast< IngestParameters* >( arg );
  const bool retval = parameters->m_workflow->startS57Ingest( parameters->m_dataStore, parameters->m_productFile ) ? 1 : 0;

  // Send a message to the user interface indicating that it can close the progress dialog now.
  ::PostMessage( parameters->m_uiWindow, WM_CLOSE_INGEST_PROGRESS_DIALOG, 0, 0 );

  return retval;
}

/////////////////////////////////////////////////////////////////////////////
// Thread function to allow the user interface to remain responsive
// during execution of the IHO tests. This function performs the actual
// tests and reports it's status to the UI thread via messages.

UINT _cdecl performMandatedTestsEdition_1_1( void *arg )
{
  // Set up the configuration object
  TestInformation *testInfo = reinterpret_cast< TestInformation* >( arg );
    
  /////////////////////////////////////
  // Test Plan Reference: 3.4.1 & 3.6
  //  - Automated running of the IHO mandated tests.
  /////////////////////////////////////

  // The recipient of the message is responsible for deleting the string we allocate. This
  // is to ensure the string still exists should we terminate before the message comes to
  // be processed.

  // ** User Permit Tests **
  string *message = new string( "---------------- ** User Permit Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63UserPermitTests userPermitTests(testInfo->m_configuration);

  // Test 1.1
  message = new string( "Test 1.1 - User Permit Generation" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( userPermitTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  ProgressAmount *progress = new ProgressAmount;
  progress->percentTotal = 1;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // ** Cell Permit Tests **
  message = new string( "---------------- ** Cell Permit Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63CellPermitTests cellPermitTests(testInfo->m_configuration);

  // Test 2.2 – Decrypt Cell Permit
  message = new string( "Test 2.2 – Decrypt Cell Permit" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( cellPermitTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 2;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 2.3 – Decrypt Erroneous Cell Permit
  message = new string( "Test 2.3 – Decrypt Erroneous Cell Permit" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( cellPermitTests.test2() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 3;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 2.4 – Check Expiry Date in Cell Permit
  message = new string( "Test 2.4 – Check Expiry Date in Cell Permit" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( cellPermitTests.test3() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 4;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // ** Certificate Tests **
  message = new string( "---------------- ** Certificate Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63CertificateTests certificateTests(testInfo->m_configuration);

  // Test 3.2 - Data Client Uses Correct SA Public Key File
  message = new string( "Test 3.2 - Data Client Uses Correct SA Public Key File" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( certificateTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 5;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 3.3 – Data Client Authenticates SA Public Key on CD Against Installed SA Public Key
  message = new string( "Test 3.3 – Data Client Authenticates SA Public Key on CD Against Installed SA Public Key" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( certificateTests.test2() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 6;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 3.4 – Data Client Identifies Erroneous SA Public Key
  message = new string( "Test 3.4 – Data Client Identifies Erroneous SA Public Key" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( certificateTests.test3() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 7;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 3.6 – Authenticate Data Server Certificate in ENC Signature File
  message = new string( "Test 3.6 – Authenticate Data Server Certificate in ENC Signature File" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( certificateTests.test4() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 8;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 3.7 - Identify Erroneous Data Server Certificate in ENC Signature File
  message = new string( "Test 3.7 - Identify Erroneous Data Server Certificate in ENC Signature File" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( certificateTests.test5() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 9;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // ** Digital Signature Tests **
  message = new string( "---------------- ** Digital Signature Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63DigitalSignatureTests digitalSignatureTests(testInfo->m_configuration);

  // Test 4.2 – Authenticate ENC signature
  message = new string( "Test 4.2 – Authenticate ENC signature" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( digitalSignatureTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 10;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 4.3 – Identify Erroneous ENC signature
  message = new string( "Test 4.3 – Identify Erroneous ENC signature" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( digitalSignatureTests.test2() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 11;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // ** Encryption & Decryption Tests **
  message = new string( "---------------- ** Encryption & Decryption Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63EncryptionDecryptionTests encTests(testInfo->m_configuration);

  // Test 5.2 - Decrypt ENC Base Cells and Update Files (Part 1)
  message = new string( "Test 5.2 - Decrypt ENC Base Cells and Update Files (Part 1)" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( encTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 12;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 5.2 - Decrypt ENC Base Cells and Update Files (Part 2)
  message = new string( "Test 5.2 - Decrypt ENC Base Cells and Update Files (Part 2)" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( encTests.test2() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 13;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 5.4 - Decrypt Next Edition of ENC with Next Cell Key
  message = new string( "Test 5.4 - Decrypt Next Edition of ENC with Next Cell Key" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( encTests.test3() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 14;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 5.5 - Decrypt Erroneous ENC
  message = new string( "Test 5.5 - Decrypt Erroneous ENC" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( encTests.test4() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 15;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // ** Compression Tests **
  message = new string( "---------------- ** Compression Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63CompressionTests compressionTests(testInfo->m_configuration);

  // Test 6.2 - Decompress ENC
  message = new string( "Test 6.2 - Decompress ENC" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( compressionTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 16;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 6.3 – Decompress Corrupted ENC
  message = new string( "Test 6.3 – Decompress Corrupted ENC" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( compressionTests.test2() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 17;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // ** Multiple Data Server Tests **
  message = new string( "---------------- ** Multiple Data Server Tests ** ----------------" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  Edition_1_1::S63MultipleServerTests multipleServerTests(testInfo->m_configuration);

  // Test 7.1 – Test installation of cells from multiple data servers
  message = new string( "Test 7.1 – Test installation of cells from multiple data servers" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( multipleServerTests.test1() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 18;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  // Test 7.2 – Test installation of cells from multiple data servers
  message = new string( "Test 7.2 – Test installation of cells from multiple data servers" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  message = new string( multipleServerTests.test2() ? "PASSED" : "FAILED" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
  progress = new ProgressAmount;
  progress->percentTotal = 19;
  ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );

  message = new string( "All tests completed" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );

  // Close the test progress dialog
  ::PostMessage( testInfo->m_uiWindow, WM_CLOSE_TEST_PROGRESS_DIALOG, 0, 0 );

  delete testInfo;

  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Thread function to allow the user interface to remain responsive
// during execution of the IHO tests. This function performs the actual
// tests and reports it's status to the UI thread via messages.

UINT _cdecl performMandatedTestsEdition_1_2( void *arg )
{
  // Set up the configuration object
  TestInformation *testInfo = reinterpret_cast< TestInformation* >( arg );
    
  /////////////////////////////////////
  // Test Plan Reference: 3.4.1 & 3.6
  //  - Automated running of the IHO mandated tests.
  /////////////////////////////////////

  // The recipient of the message is responsible for deleting the string we allocate. This
  // is to ensure the string still exists should we terminate before the message comes to
  // be processed.
  ProgressAmount* progress(NULL);
  {
    string *message = new string( "---------------- ** Section 2 ENC Licencing Tests ** ----------------" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    Edition_1_2::S63ENCLicencingTests* ENCLicencingTester(NULL);
  
    // Test 2a
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2a - Check permit string is availability" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2a() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 1;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2b
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2b - ENC cell permit string incorrect format" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2b() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 2;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2c
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2c - Validate permit CRC" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2c() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 3;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2d
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2d - Check remaining permit expiry period" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2d() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 4;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2e
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2e - Check for expired permits" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2e() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 5;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2f
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2f - Permit installation and reporting" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2f() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 6;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2g
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2g - Management of permits from multiple data servers" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2g() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 7;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;

    // Test 2h
    ENCLicencingTester = new Edition_1_2::S63ENCLicencingTests(testInfo->m_configuration);
    message = new string( "Test 2h - Management of installed permits" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCLicencingTester->test2h() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 8;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCLicencingTester;
  }

  {
    string *message = new string( "---------------- ** Section 4 ENC Authentication tests part 1 ** ----------------" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    Edition_1_2::S63ENCAuthenticationTests_Part1* ENCAuthenticationTester1(NULL);
  
    // Test 4a
    ENCAuthenticationTester1 = new Edition_1_2::S63ENCAuthenticationTests_Part1(testInfo->m_configuration);
    message = new string( "Test 4a - Install and validate the SA certificate and/or public key" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester1->test4a() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 9;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester1;

    // Test 4b
    ENCAuthenticationTester1 = new Edition_1_2::S63ENCAuthenticationTests_Part1(testInfo->m_configuration);
    message = new string( "Test 4b - Change and update installed certificate" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester1->test4b() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 10;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester1;

    // Test 4c
    ENCAuthenticationTester1 = new Edition_1_2::S63ENCAuthenticationTests_Part1(testInfo->m_configuration);
    message = new string( "Test 4c - No pre-installed certificate/public key on the system" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester1->test4c() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 11;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester1;

    // Test 4d
    ENCAuthenticationTester1 = new Edition_1_2::S63ENCAuthenticationTests_Part1(testInfo->m_configuration);
    message = new string( "Test 4d - Check SA Certificate Expiry Date" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester1->test4d() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 12;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester1;

    // Test 4e
    ENCAuthenticationTester1 = new Edition_1_2::S63ENCAuthenticationTests_Part1(testInfo->m_configuration);
    message = new string( "Test 4e - Incorrectly formatted certificate and public key files" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester1->test4e() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 13;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester1;

    // Test 4f
    ENCAuthenticationTester1 = new Edition_1_2::S63ENCAuthenticationTests_Part1(testInfo->m_configuration);
    message = new string( "Test 4f - Check certificate parameter values" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester1->test4f() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 14;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester1;
  }
  
  {
    string *message = new string( "---------------- ** Section 5 ENC Authentication tests part 2 ** ----------------" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    Edition_1_2::S63ENCAuthenticationTests_Part2* ENCAuthenticationTester2(NULL);
  
    // Test 5a
    ENCAuthenticationTester2 = new Edition_1_2::S63ENCAuthenticationTests_Part2(testInfo->m_configuration);
    message = new string( "Test 5a - Invalid SA signature in the ENC Signature File" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester2->test5a() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 15;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester2;

    // Test 5b
    ENCAuthenticationTester2 = new Edition_1_2::S63ENCAuthenticationTests_Part2(testInfo->m_configuration);
    message = new string( "Test 5b - Authentication against a non SA certificate/public key" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester2->test5b() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 16;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester2;

    // Test 5c
    ENCAuthenticationTester2 = new Edition_1_2::S63ENCAuthenticationTests_Part2(testInfo->m_configuration);
    message = new string( "Test 5c - ENC signature validation" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester2->test5c() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 17;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester2;

    // Test 5d
    ENCAuthenticationTester2 = new Edition_1_2::S63ENCAuthenticationTests_Part2(testInfo->m_configuration);
    message = new string( "Test 5d - ENC signature format validation" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester2->test5d() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 18;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester2;

    // Test 5e
    ENCAuthenticationTester2 = new Edition_1_2::S63ENCAuthenticationTests_Part2(testInfo->m_configuration);
    message = new string( "Test 5e - Check authentication is continuous and complete" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester2->test5e() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 19;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester2;

    // Test 5f
    ENCAuthenticationTester2 = new Edition_1_2::S63ENCAuthenticationTests_Part2(testInfo->m_configuration);
    message = new string( "Test 5f - Single exchange set with ENC signature files signed by multiple data servers" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCAuthenticationTester2->test5f() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 20;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCAuthenticationTester2;
  }

  {
    string *message = new string( "---------------- ** Section 6 ENC Decryption ** ----------------" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    Edition_1_2::S63ENCDecryptionTests* ENCDecryptionTester(NULL);
  
    // Test 6a
    ENCDecryptionTester = new Edition_1_2::S63ENCDecryptionTests(testInfo->m_configuration);
    message = new string( "Test 6a - Install ENCs when pre-installed permits have expired" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDecryptionTester->test6a() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 21;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDecryptionTester;
  
    // Test 6b
    ENCDecryptionTester = new Edition_1_2::S63ENCDecryptionTests(testInfo->m_configuration);
    message = new string( "Test 6b - Permit expiry within 30 days" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDecryptionTester->test6b() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 22;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDecryptionTester;

    // Test 6c
    ENCDecryptionTester = new Edition_1_2::S63ENCDecryptionTests(testInfo->m_configuration);
    message = new string( "Test 6c - Incorrect cell keys encrypted in the ENC permits" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDecryptionTester->test6c() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 23;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDecryptionTester;

    // Test 6d
    ENCDecryptionTester = new Edition_1_2::S63ENCDecryptionTests(testInfo->m_configuration);
    message = new string( "Test 6d - Validate ENC data file integrity" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDecryptionTester->test6d() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 24;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDecryptionTester;
  }

  {
    string *message = new string( "---------------- ** Section 7 ENC Data Management ** ----------------" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    Edition_1_2::S63ENCDataManagementTests* ENCDataManagementTester(NULL);
  
    // Test 7a
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7a - Encrypted ENCs supplied by different Data Servers" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7a() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 25;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;

    // Test 7b
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7b - Loading additional ENC cell permits and cells from a different data provider" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7b() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 26;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;

    // Test 7c
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7c - Test that the system operates correctly in a multiple data provider environment" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7c() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 27;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;

    // Test 7d
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7d - ECDIS management of cancelled cells" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7d() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 28;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;
    
    // Test 7e
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7e - ECDIS Display of Replacement ENC Cells" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7e() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 29;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;

    // Test 7f
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7f - ECDIS management of ENC re-issued cells" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7f() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 30;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;

    // Test 7g
    ENCDataManagementTester = new Edition_1_2::S63ENCDataManagementTests(testInfo->m_configuration);
    message = new string( "Test 7g - ECDIS management of Base and Update Exchange Sets" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataManagementTester->test7g() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 31;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataManagementTester;

    // Test 7h is optional
    
    // Test 7i is optional

    // Test 7j is optional
  }

  {
    string *message = new string( "---------------- ** Section 8 Data Exchange Media ** ----------------" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    Edition_1_2::S63ENCDataExchangeMediaTests* ENCDataExchangeMediaTester(NULL);
  
    // Test 8a
    ENCDataExchangeMediaTester = new Edition_1_2::S63ENCDataExchangeMediaTests(testInfo->m_configuration);
    message = new string( "Test 8a - Exchange Set and Media Delivery" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataExchangeMediaTester->test8a() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 32;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataExchangeMediaTester;

    // Test 8b
    ENCDataExchangeMediaTester = new Edition_1_2::S63ENCDataExchangeMediaTests(testInfo->m_configuration);
    message = new string( "Test 8b - Single Media containing Multiple Exchange Sets" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataExchangeMediaTester->test8b() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 33;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataExchangeMediaTester;

    // Test 8c
    ENCDataExchangeMediaTester = new Edition_1_2::S63ENCDataExchangeMediaTests(testInfo->m_configuration);
    message = new string( "Test 8c - Multiple exchange sets across multiple media sets" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataExchangeMediaTester->test8c() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 34;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataExchangeMediaTester;

    // Test 8d
    ENCDataExchangeMediaTester = new Edition_1_2::S63ENCDataExchangeMediaTests(testInfo->m_configuration);
    message = new string( "Test 8d - Media validation of encrypted ENC service status" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    message = new string( ENCDataExchangeMediaTester->test8d() ? "PASSED" : "FAILED" );
    ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );
    progress = new ProgressAmount;
    progress->percentTotal = 35;
    ::PostMessage( testInfo->m_uiWindow, WM_UPDATE_TEST_PROGRESS_DIALOG, 0, LPARAM(progress) );
    delete ENCDataExchangeMediaTester;
  }

  std::string* message = new std::string( "All tests completed" );
  ::PostMessage( testInfo->m_uiWindow, WM_LOG_MESSAGE, 0, LPARAM(message) );

  // Close the test progress dialog
  ::PostMessage( testInfo->m_uiWindow, WM_CLOSE_TEST_PROGRESS_DIALOG, 0, 0 );

  delete testInfo;

  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowView construction/destruction

S63WorkFlowView::S63WorkFlowView()
  : m_dataLocation("")
  , m_unlockKey("")
  , m_logFile("")
  , m_cacheSize(CACHE_SIZE)
  , m_s63ErrorLayers()
  , m_tutTime(0)
  , m_configurationTime(0)
  , m_hardwareID("12345")
  , m_hardwareIDChanged(true)
  , m_workFlow(NULL)
  , m_s63Layer(NULL)
  , m_s52NoDataLayer(NULL)
  , m_drawingSurface(NULL)
  , m_modeManager(NULL)
  , m_mapLoaded(false)
  , m_s63Loaded(false)
  , m_enableS52(true)
  , m_extentToPrint()
  , m_tut_resultsDir()
  , m_tut_configDir()
  , m_displayIngestDialog(false)
  , m_s52DynamicRenderer(NULL)
  , m_zoomScalePtr(NULL)
  , m_muPerScreenMetre(-1)
  , m_scaleBandPtr(NULL)
  , m_simpleInterface(false)
  , m_scaleBandOption(0)
  , m_nTimerID(0)
{
  // Set the default time - test data is quite old.
  CTime time(2004, 8, 24, 12, 0, 0);
  m_configurationTime = time.GetTime();

  // Set up S52 rendering objects incase the user requests S52 rendering.
  m_s52StateObject = new S52StateObject();
  m_userRenderingProcedure = new S52UserRenderingProcedure();

    
  // Create a new Workflow object
  // This is where the S63 related operations shall be completed.

  // Provide the work flow with callbacks to the GUI, so we can alert
  // the user as appropriate.
  m_workFlow = new S63Workflow(catalogueCallback, notifyErrorCallback, ingestCellCallback, cancelledCellCallback, progressCallback, &m_hWnd, m_s52StateObject ) ;


  // set the default hardware id... so we don't need to display the configuration panel.
  m_workFlow->hwid(m_hardwareID); 

  // Set the current time on the work flow object.
  // We set this time as the test data has long since expired and
  // setting this value allows us to pretend that it is in date.
  m_workFlow->currentTime(m_configurationTime);

  // Set whether or not to display the ingest dialog in the workflow.
  m_workFlow->displayIngestDialog(m_displayIngestDialog);


  // find and load the standard MapLink configuration files
  string mapLinkConfigDir;

  if (!getMapLinkConfigDirectory(mapLinkConfigDir))
  {
    AfxMessageBox(_T("Unable to find the S52 configuration directory."), MB_ICONERROR ) ;
    return ;
  }
  mapLinkConfigDir += "/s52/";

  TSLS52DynamicRenderer::s52HomeDirectory(mapLinkConfigDir.c_str());

  // load the standard s52 configuration for the s57 filter.
  string s57FilterConfig = mapLinkConfigDir + "s57filter_disable_cospatial.ini"; // for a viewer we do not need the full co-spatial analysis
  m_workFlow->s57ConfigFile(s57FilterConfig.c_str());

  // If the sample has been unlocked before being built, we should 'mask' the unlock key
  string unlockKey("");

  if ( m_workFlow->unlock(unlockKey) ) 
  {
    m_unlockKey = "*******";
  }

  // Get the DPI of the monitor which is in use. 
  // This value will be used later to determine S63 scalebanding.
  CWnd * hwnd = GetDesktopWindow();
  CDC * cdc = hwnd->GetDC();
  HDC hdc = cdc->m_hDC;

  const int hdpi = GetDeviceCaps(hdc, LOGPIXELSX);
  const int vdpi = GetDeviceCaps(hdc, LOGPIXELSY);

  m_workFlow->setDPI(vdpi, hdpi);

  hwnd->ReleaseDC(cdc);
}

S63WorkFlowView::~S63WorkFlowView()
{
  if (m_ingestProgressDialog)
    delete m_ingestProgressDialog;

  // Clear everything up
  if ( m_modeManager )
  {
    delete m_modeManager ;
    m_modeManager = NULL ;
  }

  if (m_userRenderingProcedure)
    delete m_userRenderingProcedure;
  m_userRenderingProcedure = NULL;

  // The Dynamic Renderer is owned by the Drawing Surface
  m_s52DynamicRenderer = NULL;
  
  if ( m_s52NoDataLayer )
  {
    m_s52NoDataLayer->destroy();
    m_s52NoDataLayer = NULL;
  }

  if ( m_s52StateObject )
  {
    delete m_s52StateObject;
    m_s52StateObject = NULL;
  }

  if ( m_drawingSurface )
  {
    m_drawingSurface->clearAllDynamicRenderers();
    delete m_drawingSurface ;
    m_drawingSurface = NULL ;
  }
      
  vector< TSLCustomDataLayer * >::iterator custIt;
  for ( custIt = m_s63CustomErrorLayers.begin(); custIt != m_s63CustomErrorLayers.end(); ++custIt )
  {
    (*custIt)->destroy();
  }
  
  vector< S63ErrorLayer * >::iterator it;
  for ( it = m_s63ErrorLayers.begin(); it != m_s63ErrorLayers.end(); ++it )
  {
    delete (*it);
  }  

  if ( m_workFlow )
  {
    delete m_workFlow;
    m_workFlow = NULL;
  }

  if ( m_s63Layer )
  {
    m_s63Layer->destroy();
    m_s63Layer = NULL;    
  }
}

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowView drawing

void S63WorkFlowView::OnDraw(CDC* pDC)
{
  ASSERT( m_drawingSurface ) ;
  
  if ( m_drawingSurface )
  {
    ///////////////////////////////////////////////////
    // We need to update the scale and ownship position as these are used by the S52 Dynamic Renderer to declutter the display 
    // and to decide how to drawn some of the symbology (over cell and scale related).

    // Update the calculated scale.
    updateScale();

    // Set the ownship position as the center of the screen
    RECT windowSize;
    GetClientRect( &windowSize ) ;

    TSLTMC x;
    TSLTMC y;
    m_drawingSurface->DUToTMC((TSLDeviceUnits)fabs((windowSize.right - windowSize.left)/2.0),
      (TSLDeviceUnits)fabs((windowSize.bottom - windowSize.top)/2.0), &x, &y);
    TSLCoord position(x, y);
    m_s52StateObject->setOwnShip(&position);

    //////////////////////////////////////////////////
    // Draw to the window
    RECT rect ;
    if (m_useOpenGLSurface)
    {      
      GetClientRect( &rect ) ;
      m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;
    }
    else
    {
      // Only draw the exposed bits of the display
      if ( pDC->GetClipBox( &rect ) == NULLREGION )
        GetClientRect( &rect ) ;
      m_drawingSurface->drawDU( rect.left, rect.bottom, rect.right, rect.top, true ) ;
    }
    
    // Don't forget to draw any echo rectangle that may be active.
    if ( m_modeManager )
      m_modeManager->onDraw( rect.left, rect.bottom, rect.right, rect.top ) ;
  }
}

void S63WorkFlowView::setScale(int scale, bool updateReadout)
{
  double requestedScale = 1.0 / scale;
  double tmcPerMU = m_drawingSurface->TMCperMU();
  double tmcPerDUX = 0.0;
  double tmcPerDUY = 0.0;

  m_drawingSurface->TMCperDU(tmcPerDUX, tmcPerDUY);
  double duPerMUX = tmcPerMU / tmcPerDUX;
  double duPerMUY = tmcPerMU / tmcPerDUY;

  //Query the Screen information
  CWnd * hwnd = GetDesktopWindow();
  CDC * cdc = hwnd->GetDC();
  HDC hdc = cdc->m_hDC;

  int pixelsPerInchX = GetDeviceCaps(hdc, LOGPIXELSX);
  int pixelsPerInchY = GetDeviceCaps(hdc, LOGPIXELSY);

  hwnd->ReleaseDC(cdc);

  double pixelsPerMetreX = pixelsPerInchX / 0.0254;
  double pixelsPerMetreY = pixelsPerInchY / 0.0254;

  // scale the current displayed extent
  double kX = duPerMUX / (pixelsPerMetreX * requestedScale);
  double kY = duPerMUY / (pixelsPerMetreY * requestedScale);
  TSLEnvelope extent;
  m_drawingSurface->getTMCExtent(extent);
  extent.scale(kX, kY);

  // set the area to draw to match the requested scale. 
  double uux1, uuy1, uux2, uuy2;
  m_drawingSurface->TMCToUU(extent.bottomLeft().x(), extent.bottomLeft().y(), &uux1, &uuy1);
  m_drawingSurface->TMCToUU(extent.topRight().x(), extent.topRight().y(), &uux2, &uuy2);
  m_drawingSurface->resize(uux1, uuy1, uux2, uuy2, false);

  if (updateReadout)
    updateScale();
}


/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowView printing
BOOL S63WorkFlowView::OnPreparePrinting(CPrintInfo* pInfo)
{
  pInfo->SetMaxPage(1);
  return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowView diagnostics

#ifdef _DEBUG
void S63WorkFlowView::AssertValid() const
{
  CView::AssertValid();
}

void S63WorkFlowView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

S63WorkFlowDoc* S63WorkFlowView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(S63WorkFlowDoc)));
  return (S63WorkFlowDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// S63WorkFlowView message handlers

void S63WorkFlowView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  
  S63WorkFlowDoc * doc = GetDocument() ;
  
  // In an SDI application, this method is called every time a document is
  // loaded.  The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface ) 
  {
    CRect rect ;
    
    GetClientRect( &rect ) ;
    
    // Create a double buffered drawing surface, and an interaction interface
    // to control it.
    // Set up the initial window extents.
    if( m_useOpenGLSurface )
    {
      TSLOpenGLSurfaceCreationParameters creationParams;
      creationParams.numMultisampleSamples( 4 ); // Enable anti-aliasing
      TSLWGLSurface *surface = new TSLWGLSurface( m_hWnd, false, creationParams );

      // Check that the drawing surface was created successfully. This might fail if the host
      // machine does not support the necessary version of OpenGL.
      if( !surface->context() )
      {
        TSLSimpleString errorMsg;
        TSLThreadedErrorStack::errorString( errorMsg );
        CString unicodeMessage( errorMsg.c_str() );
        MessageBox( unicodeMessage, _T("Failed to create drawing surface"), MB_OK );
        exit(-1);
      }
      m_drawingSurface = surface;
    }
    else
    {
      m_drawingSurface = new TSLNTSurface( m_hWnd, false ) ;
      m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    }
    
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ;
    m_drawingSurface->wndResize( 0, 0, rect.Width(), rect.Height() ) ;    

    //////////////////////////////////////
    // Test Plan Reference: 3.4.2.3.3
    //  - Apply rendering to the TMF.
    /////////////////////////////////////

    // Now create and initialise the mode manager and modes
    m_modeManager = new TSLInteractionModeManagerGeneric( this, m_drawingSurface, 5, 5, 30, true ) ;
    m_modeManager->addMode( new TSLInteractionModeZoom( ID_TOOLS_ZOOM ), true ) ;
    m_modeManager->addMode( new TSLInteractionModePan( ID_TOOLS_PAN ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeGrab( ID_TOOLS_GRAB ), false ) ;
    m_modeManager->addMode( new TSLInteractionModeMagnify( ID_TOOLS_MAGNIFY ), false ) ;

    m_workFlow->setDrawingSurface(m_drawingSurface);

  }
  else if ( m_mapLoaded ) 
  {
    // We have a drawing surface with a map loaded into it.
    // Detach the map from the surface
    if ( m_drawingSurface && doc )
      doc->detachFromSurface( m_drawingSurface ) ;
  }
  
  // If the document loaded successfully, add it to the drawing surface
  m_mapLoaded = doc->addMapToSurface( m_drawingSurface ) ;
 
  if (m_mapLoaded)
  {
    // Load the appropriate rendering
    if ( m_enableS52 )
    {
      loadS52Rendering();
    }
    else
    {
      loadS63Rendering();
    }
  }
  // and reset the current view to display the entire map.
  m_drawingSurface->reset( false ) ;

  m_modeManager->resetViews( ) ;
  m_modeManager->resetMode( TSLButtonNone, 0, 0 ) ;

  if (m_zoomScalePtr == NULL)
  {
    ZoomScale * zoomScalePtr = &((CMainFrame *)(theApp.m_pMainWnd))->m_wndToolBar.m_zoomCombo;
    zoomScalePtr->setParent(this);
    m_zoomScalePtr = zoomScalePtr;
  }
  if (m_scaleBandPtr == NULL)
  {
    ScaleBand * scaleBandPtr = &((CMainFrame *)(theApp.m_pMainWnd))->m_wndToolBar.m_scaleBandCombo;
    scaleBandPtr->setParent(this);
  }
}

void S63WorkFlowView::OnToolsZoomin() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomIn( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnToolsZoomout() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->zoomOut( 30 ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnToolsReset() 
{
  if ( m_modeManager )
  {
    if ( m_modeManager->resetToFullExtent( ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  
  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, 
      TSLResizeActionMaintainTopLeft) ;

    if ( m_modeManager )
      m_modeManager->onSize( cx, cy ) ;

    // set the ownship position as the center of the screen
    TSLTMC x;
    TSLTMC y;
    m_drawingSurface->DUToTMC(cx/2,cy/2, &x, &y);
    TSLCoord position(x, y);
    m_s52StateObject->setOwnShip(&position);
  }
}

void S63WorkFlowView::OnMouseMove(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    TSLButtonType button = TSLButtonNone ;

    if ( nFlags & MK_LBUTTON )
      button = TSLButtonLeft ;
    else if ( nFlags & MK_MBUTTON )
      button = TSLButtonCentre ;
    else if ( nFlags & MK_RBUTTON )
      button = TSLButtonRight ;

    if ( m_modeManager->onMouseMove( button, point.x, point.y,
				     (bool)((nFlags & MK_SHIFT) == MK_SHIFT), (bool)((nFlags & MK_CONTROL) == MK_CONTROL)) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }

  if ( m_s63Loaded || m_mapLoaded )
  {  
    CMainFrame * pFrame = static_cast< CMainFrame * >( AfxGetMainWnd() ) ;
    CStatusBar * pStatus = pFrame->getStatusBar();

    if ( pStatus )
    {
      double lat, lon;
      if (m_drawingSurface->DUToLatLong(point.x, point.y, &lat, &lon, false, false))
      {
        int latsec = (int)fabs((lat*3600));  // convert degrees decimal to number of seconds
        int latdegrees = (int)floor(latsec / 3600.0);
        int latminutes = (int)floor(latsec/60.0) % 60;
        int latseconds = (int)(latsec % 60);
        int lonsecs = (int)fabs((lon*3600));  // convert degrees decimal to number of seconds
        int londegrees = (int)floor(lonsecs / 3600.0);
        int lonminutes = (int)floor(lonsecs/60.0) % 60;
        int lonseconds = (int)(lonsecs % 60);

        TCHAR  tmpstr [1024];

        TSLTMC x1, y1;
        m_drawingSurface->DUToTMC(point.x, point.y, &x1, &y1);

        _sntprintf( tmpstr, 1023, _T("Latitude: %03d:%02d:%02d%c Longitude: %03d:%02d:%02d%c  %f %f"), latdegrees, latminutes, latseconds, lat < 0.0 ? _T('S') : _T('N'), 
          londegrees, lonminutes, lonseconds, lon < 0.0 ? _T('W') : _T('E'), lat, lon);
        pStatus->SetPaneText(2, tmpstr);
      }
      else
      {
        pStatus->SetPaneText(2, _T("Latitude: Longitude :"));
      }
    }
  }

  CView::OnMouseMove(nFlags, point);
}

void S63WorkFlowView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonDown( point.x, point.y,
      (bool)((nFlags & MK_SHIFT) == MK_SHIFT), (bool)((nFlags & MK_CONTROL) == MK_CONTROL)) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonDown(nFlags, point);
}

void S63WorkFlowView::OnMButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonDown( point.x, point.y,
				       (bool)((nFlags & MK_SHIFT) == MK_SHIFT) , (bool)((nFlags & MK_CONTROL) == MK_CONTROL) ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnMButtonDown(nFlags, point);
}

void S63WorkFlowView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonDown( point.x, point.y,
      (bool)((nFlags & MK_SHIFT) == MK_SHIFT), (bool)((nFlags & MK_CONTROL) ==MK_CONTROL)) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnRButtonDown(nFlags, point);
}

void S63WorkFlowView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onLButtonUp( point.x, point.y,
      (bool)((nFlags & MK_SHIFT) == MK_SHIFT), (bool)((nFlags & MK_CONTROL) ==MK_CONTROL)) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  CView::OnLButtonUp(nFlags, point);
}

void S63WorkFlowView::OnMButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onMButtonUp( point.x, point.y,
      (bool)((nFlags & MK_SHIFT) == MK_SHIFT), (bool)((nFlags & MK_CONTROL) ==MK_CONTROL)) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnMButtonUp(nFlags, point);
}

void S63WorkFlowView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  // If the user is in the middle of an interaction, pass the event onto the handler
  if ( m_modeManager )
  {
    if ( m_modeManager->onRButtonUp( point.x, point.y,
      (bool)((nFlags & MK_SHIFT) == MK_SHIFT), (bool)((nFlags & MK_CONTROL) ==MK_CONTROL)) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  
  CView::OnRButtonUp(nFlags, point);
}

void S63WorkFlowView::OnToolsPan() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_PAN ) ;

}

void S63WorkFlowView::OnToolsZoom() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_ZOOM ) ;

}

void S63WorkFlowView::OnToolsGrab() 
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_GRAB ) ;
}

void S63WorkFlowView::OnUpdateToolsAction(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded.
  if (pCmdUI->m_nID != ID_TOOLS_MAGNIFY) // disable Magnify because we've not setup the layers 
    pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) ? true : false ) ;
  else
    pCmdUI->Enable( false ) ;
}

void S63WorkFlowView::OnUpdateToolsMode(CCmdUI* pCmdUI) 
{
  // Common update handler to all items that are dependant upon the map
  // being loaded, and act as radios dependant upon the current mode.
  if (pCmdUI->m_nID != ID_TOOLS_MAGNIFY) // disable Magnify because we've not setup the layers 
    pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) ? true : false ) ;
  else
    pCmdUI->Enable( false ) ;
  pCmdUI->SetCheck( ( m_mapLoaded || m_s63Loaded ) && m_modeManager && ((UINT)m_modeManager->getCurrentMode() == pCmdUI->m_nID )) ;
}

void S63WorkFlowView::OnEditCopy() 
{
  // Snap shot the current view and place it on the clipboard
  CDC * pDC = GetDC() ;
  
  // Create a bitmap
  RECT rc ;
  GetClientRect ( &rc ) ;
  
  HBITMAP newBitmap = CreateCompatibleBitmap( pDC->m_hDC, rc.right, rc.bottom );
  HDC	  newDC     = CreateCompatibleDC( pDC->m_hDC );
  bool    opened = false ;
  bool    closed = false ;
  
  if ( newBitmap && newDC )
  {
    // Select the bitmap into the DC
    SelectObject( newDC, newBitmap );
    
    // And bit blit the screen to the new bitmap and put it into the clipboard
    if (   !BitBlt( newDC, 0, 0, rc.right, rc.bottom, pDC->m_hDC, 0, 0, SRCCOPY ) 
	|| !(bool)(opened = OpenClipboard( ) )
	|| !EmptyClipboard( )
	|| !SetClipboardData( CF_BITMAP, newBitmap ) 
	|| !(bool)(closed = CloseClipboard() ) )
    {
      AfxMessageBox( _T("Cannot access clipboard"), MB_ICONERROR );
    }
  }
  else
  {
    AfxMessageBox( _T("Cannot copy map view"), MB_ICONERROR );
  }
  
  if ( opened && !closed )
    CloseClipboard() ;
  
  if ( newBitmap )
    DeleteObject( newBitmap );
  
  if ( newDC )
    DeleteDC( newDC );

  ReleaseDC(pDC);
}

void S63WorkFlowView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnLButtonDown( nFlags, point ) ;
  CView::OnLButtonDblClk(nFlags, point);
}

void S63WorkFlowView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnRButtonDown( nFlags, point ) ;
  CView::OnRButtonDblClk(nFlags, point);
}
void S63WorkFlowView::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
  // Just handle like a normal press to avoid losing the second press
  OnMButtonDown( nFlags, point ) ;
  CView::OnMButtonDblClk(nFlags, point);
}

void S63WorkFlowView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // Attempt to print using the PrintHelper, using the standard templates.
  if ( m_drawingSurface ) 
  {
    TSLEntitySet * tmp = 0 ;
    TSLSimpleString templateFilePath;

    char* templateFile;
    
    // Choose a default portrait or landscape template.
    // You can create your own using PrintTemplateStudio;
    if ( pInfo->m_rectDraw.Width() > pInfo->m_rectDraw.Height() )
    {
      templateFile = "landscape.tpt";
    }
    else
    {
      templateFile = "portrait.tpt";
    }

    // Search for the file in the drawing surface's pathlist.
    // These files will usually be in the config directory alongside the
    // tslcolours.dat etc.
    

    // load the template
    if ( TSLDrawingSurface::findFile( templateFile, templateFilePath ) )
    {
      tmp = TSLPrintHelper::loadTemplate( templateFilePath.c_str() ) ;
    }

    if ( !tmp || tmp->size() == 0 )
    {
      MessageBox( _T("Cannot find print template") ) ;
      return ;
    }

    // Set up a print item with a 3 pixel wide border, displaying the current extent.
    // All visible data layers in the drawing surface are used when printing.
    // To display at a specific scale, use item.centreAndScale rather than item.extent
    // There can be as many items as we need 
    // Note : Use the extent captured in the OnFilePrint, not the current extent.  This is
    //        because the extent may have changed to to S63WorkFlow Print Preview taking control
    //        of the window and resizing it.
    TSLPrintItem item( m_drawingSurface ) ; // Use the default print template item name 'map'
    long colour    = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 ) ;
    long style     = 1 ;
    long thickness = 3 ;
    item.setBorder( colour, style, thickness ) ;
    item.extent( m_extentToPrint ) ;

    // Add a mapping for the 'date' text object in the template to the current date
    // In the template, if there is a text string 'date', this is changed to the
    // current date and time when printed.
    const char * mappings[ ][ 2 ] = { "date", 0 } ;

    time_t ltime;
    time( &ltime );

    std::string s = ctime( &ltime ) ;

    // ctime always adds a carriage return on the end, so take it off
    size_t last = s.find_last_of( '\n' ) ;
    if ( last != std::string::npos )
      s[ last ] = '\0' ;
    
    mappings[0][1] = s.c_str() ;

    // Create an MFC specific print context.  This contains information
    // such as the printable area.  It is necessary to use and MFC specific one
    // to cope with the MFC Print Preview facility which tweaks the screen
    // to look like the printer.
    TSLMFCPrintContext context( pDC, pInfo ) ;

    // Now print the items.

    // If we wanted to print more items, for example enlargements, then we could
    // either create a custom template with fixed position items - each with a 
    // different name, or use the TSLPrintHelper::addItemTemplate method to add
    // a dynamically created item.  This allows for the same template to be used
    TSLPrintHelper::printUsingTemplate( context, 1, &item, tmp, 1, mappings ) ;

    // Tidy up the template since we no longer need it.
    tmp->destroy() ;
    
    pInfo->m_bContinuePrinting = false ;
  }
}

BOOL S63WorkFlowView::OnEraseBkgnd(CDC* pDC) 
{
  // Since the drawing surface is double buffered, we don't need to erase the
  // background.
  return true;
}


void S63WorkFlowView::OnFilePrint() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in S63WorkFlow print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrint() ;
}

void S63WorkFlowView::OnFilePrintPreview() 
{
  // We should not query the extent to print during the OnPrint, since the window may change
  // size, especially in S63WorkFlow print-preview
  if ( m_drawingSurface )
    m_drawingSurface->getTMCExtent( m_extentToPrint ) ;
  CView::OnFilePrintPreview() ;
}

void S63WorkFlowView::OnZoomToData()
{
  if ( !m_s63Layer )
  {
    // If there's no S63 data layer, reset to the full map extent.
    OnToolsReset();
    return;
  }
  
  TSLTMC x1, y1, x2, y2;
  m_s63Layer->getTMCExtent(&x1,&y1,&x2,&y2);
  double blLat, blLon;
  double trLat, trLon;
  m_s63Layer->TMCToLatLong(x1, y1, &blLat, &blLon);
  m_s63Layer->TMCToLatLong(x2, y2, &trLat, &trLon);

  double lat = ((trLat - blLat)/2.0) + blLat;
  double lon = ((trLon - blLon)/2.0) + blLon;

  m_drawingSurface->setViewedLatLongRange(lat, lon, 1024 * 10, false, false);
  setScale(INITIAL_SCALE);
  // And redraw
  m_drawingSurface->redraw();
}

void S63WorkFlowView::OnToolsMagnify()
{
  if ( m_modeManager )
    m_modeManager->setCurrentMode( ID_TOOLS_MAGNIFY ) ;
}

void S63WorkFlowView::resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
  // Ignore the override
}

void S63WorkFlowView::viewChanged(TSLDrawingSurface* drawingSurface)
{
 
}

void S63WorkFlowView::OnSavedviewsGoto1()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 0 ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnSavedviewsGoto2()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 1 ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnSavedviewsGoto3()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->savedViewGoto( 2 ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnSavedviewsReset()
{
  if ( m_modeManager )
    m_modeManager->savedViewReset( ) ;
}

void S63WorkFlowView::OnSavedviewsSet1()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 0 ) ;
}


void S63WorkFlowView::OnUpdateViewerEnabled(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(!m_simpleInterface) ;
}

void S63WorkFlowView::OnUpdateSavedviewsReset(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) ? true : false  ) ;
}

void S63WorkFlowView::OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) && m_modeManager && m_modeManager->savedViewValid( 2 ) ) ;
}

void S63WorkFlowView::OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) && m_modeManager && m_modeManager->savedViewValid( 1 ) ) ;
}

void S63WorkFlowView::OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) && m_modeManager && m_modeManager->savedViewValid( 0 ) ) ;
}

void S63WorkFlowView::OnUpdateSavedviewsSet1(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) ? true : false  ) ;
}

void S63WorkFlowView::OnSavedviewsSet2()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 1 ) ;
}

void S63WorkFlowView::OnUpdateSavedviewsSet2(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) ? true : false  ) ;
}

void S63WorkFlowView::OnSavedviewsSet3()
{
  if ( m_modeManager )
    m_modeManager->savedViewSetToCurrent( 2 ) ;
}

void S63WorkFlowView::OnUpdateSavedviewsSet3(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( ( m_mapLoaded || m_s63Loaded ) ? true : false  ) ;
}

void S63WorkFlowView::OnViewstackNextview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoNext( ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnUpdateViewstackNextview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (( m_mapLoaded || m_s63Loaded ) && m_modeManager && m_modeManager->viewStackNextValid( )) ? true : false ) ;
}

void S63WorkFlowView::OnViewstackPreviousview()
{
  if ( m_modeManager )
  {
    if ( m_modeManager->viewStackGotoPrevious( ) )
      InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnUpdateViewstackPreviousview(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( (( m_mapLoaded || m_s63Loaded ) && m_modeManager && m_modeManager->viewStackPreviousValid( )) ? true : false ) ;
}


BOOL S63WorkFlowView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  ScreenToClient(&pt);

  if ( m_modeManager )
  {
    if ( m_modeManager->onMouseWheel( zDelta, pt.x, pt.y ) )
    {
      // Request a redraw if the interaction hander requires it
      InvalidateRect( 0, false ) ;
    }
  }
  
  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void S63WorkFlowView::OnConfiguration()
{
  // Create a test configuration dialog
  TestConfiguration config(m_workFlow);

  // Populate the dialog with values which have already
  // been stored.
  config.m_unlockKey = (LPCTSTR)TSLUTF8Decoder(m_unlockKey);
  config.m_logFile = (LPCTSTR)TSLUTF8Decoder(m_logFile);
  config.m_cacheSize = m_cacheSize;
  config.m_hw_id = (LPCTSTR)TSLUTF8Decoder(m_hardwareID);
  config.m_logPermanentWarnings = m_disableWarnings;
  config.m_configurationTime = m_configurationTime;  
  config.m_displayIngestDialog = m_displayIngestDialog;
  config.m_s57ConfigFile = (LPCTSTR)TSLUTF8Decoder(m_s57ConfigFile);

  // If the user clicked 'OK'
  if (config.DoModal() == IDOK)
  {

    // Check to see if the hardware ID has changed.
    // If the hardware ID has changed then we need to re-create the TSLS63DataLayer.
    string newHWID = TSLUTF8Encoder(config.m_hw_id);
    if (newHWID != m_hardwareID)
    {
      m_hardwareIDChanged = true;
    }

    // Extract the values which were entered and store them for later.
    m_unlockKey = TSLUTF8Encoder(config.m_unlockKey);
    m_logFile = TSLUTF8Encoder(config.m_logFile);
    m_s57ConfigFile = TSLUTF8Encoder(config.m_s57ConfigFile);
    m_cacheSize = config.m_cacheSize;
    m_hardwareID = newHWID;
    m_disableWarnings = config.m_logPermanentWarnings;
    m_configurationTime = config.m_configurationTime;
    m_displayIngestDialog = config.m_displayIngestDialog;

    m_workFlow->hwid(m_hardwareID);
  }

  // Set the current time on the work flow object.
  // We set this time as the test data has long since expired and
  // setting this value allows us to pretend that it is in date.
  m_workFlow->currentTime(m_configurationTime);

  // Set whether or not to display the ingest dialog in the workflow.
  m_workFlow->displayIngestDialog(m_displayIngestDialog);

  // Set whether or not to show permanent warnings from out of date/expired data
  size_t numErrorLayers = m_s63ErrorLayers.size();
  for( size_t i = 0; i < numErrorLayers; ++i )
  {
    m_s63ErrorLayers[i]->disableErrors( config.m_logPermanentWarnings );
    m_s63ErrorLayers[i]->dataLayer()->notifyChanged();
  }

  // Update the log file as appropriate.
  if ( !m_logFile.empty() ) 
  {
    logFile(m_logFile);
  }

  // If the workflow hasn't already been unlocked
  if ( !m_workFlow->unlocked() && !m_unlockKey.empty())
  {
    // Attempt to unlock the S63 functionality.
    if (  !m_workFlow->unlock(m_unlockKey) )
    {
      AfxMessageBox(_T("The provided unlock key is invalid, the S63 functionality has not been unlocked."), MB_ICONERROR);
    }
    else
    {
      AfxMessageBox(_T("The S63 functionality has been unlocked successfully."), MB_ICONINFORMATION);
    }
  }

  // Request a redraw
  InvalidateRect(0, false);
}

void S63WorkFlowView::OnStateConfiguration()
{
  // Create a test configuration dialog
  StateConfiguration stateConfig(m_s52StateObject, m_workFlow, this);
  stateConfig.m_enableS52Rendering = m_enableS52;

  // If the user clicked 'OK'
  if (stateConfig.DoModal() == IDOK)
  {
    bool enableS52 = (bool)stateConfig.m_enableS52Rendering;
    
    enableS52Rendering(enableS52);
  }
}

void S63WorkFlowView::enableS52Rendering(bool  enableS52)
{
  // Check to see if S52 rendering state has changed
  if ( enableS52 != m_enableS52 )
  {
    // If we've now enabled S52, we need to change the rendering and apply S52 dynamic renderers.
    if ( enableS52 )
    {
      loadS52Rendering();
    }
    else
    {
      loadS63Rendering();
    }

    m_enableS52 = enableS52;
  }

  if ( m_s63Layer )
  {
    m_s63Layer->notifyChanged(true);
    InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnGenerateUserPermit()
{
  string hw_id, m_key, m_id, savePath;
  UserPermitDialog userPermitDialog;

  // Generate a new user permit dialog.
  if (userPermitDialog.DoModal() != IDOK)
  {
    //don't raise an error if they cancelled
    return;
  }

  // Retrieve any stored values.
  hw_id = TSLUTF8Encoder(userPermitDialog.m_hw_id);
  m_key = TSLUTF8Encoder(userPermitDialog.m_key);
  m_id = TSLUTF8Encoder(userPermitDialog.m_id);
  savePath = TSLUTF8Encoder(userPermitDialog.m_savePath);

  // If they're all empty, just return as they've not attempted to input anything
  if ( hw_id.empty() && m_key.empty() && m_id.empty() && savePath.empty() ) 
  {
    return;
  }

  // Ensure they've entered a path.
  if ( savePath.empty() )
  {
    AfxMessageBox( _T("Please specify a file to save the user permit."), MB_ICONERROR ) ; 
    return;
  }

  // Because the user permit generation does not use callbacks to generate errors,
  // we shall have to use the error stack. So we clear it up to this point.
  TSLThreadedErrorStack::clear() ;

  string generatedUserPermit;

  // Attempt to generate the user permit, providing the values entered in the dialog.
  if ( !m_workFlow->generateUserPermit(hw_id, m_key, m_id, generatedUserPermit) )
  {
    string logMessage;

    // Query the error stack to see if any errors have been raised.
    TSLSimpleString msg;
    TSLThreadedErrorStack::errorString(msg) ;

    if ( !msg.empty() )
    {
      logMessage = "Error - Failed to generate user permit - ";
      logMessage += msg.c_str();

      TSLThreadedErrorStack::clear() ;
    }
    else
    {
      logMessage = "Error - Failed to generate user permit.";
    }

    addLogLine(logMessage);

    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ; 
    return;
  }

  // If we've reached this point, the permit has been generated correctly.
  // So we proceed with saving it.
  TSLofstream outstream( savePath.c_str() );

  if ( !outstream.is_open() )
  {
    addLogLine("Error - Failed to generate user permit - Unable to save file.");
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;
    return;
  }

  outstream.write( generatedUserPermit.c_str(), strlen(generatedUserPermit.c_str()) );
  outstream.close();

  // Finally, provide the user with the generated permit.
  string logMessage("User permit generated: ");
  logMessage += generatedUserPermit;

  addLogLine(logMessage);
}

void S63WorkFlowView::OnS63TestRunEdition_1_1()
{
  // Run the mandated tests.
  addLogLine("Commenced running of tests.");

  // Set up the test configuration object - this is owned by the worker thread we create
  // to ensure it exists for the duration of the test run.
  S63ConfigurationObject configuration;
  configuration.m_testDataLocation = m_dataLocation;
  configuration.m_configurationLocation = m_tut_configDir + "\\";
  configuration.m_resultsDirectory = m_tut_resultsDir + "\\";
  TestInformation *testInfo = new TestInformation( configuration, m_hWnd );

  // Install the MapLink error stack callback
  TSLThreadedErrorStack::errorCallback(this, TSLErrorCategoryAll);

  CWinThread *testThread = AfxBeginThread( &performMandatedTestsEdition_1_1, testInfo );
  // Display a progress dialog for the tests
  m_testProgressDialog.setNumberOfTests( NUM_IHO_TESTS_1_1 );
  m_testProgressDialog.DoModal();

  // Wait for the test thread to terminate before returning control to the application
  WaitForSingleObject( testThread->m_hThread, INFINITE );

  // Clear the MapLink error stack callback
  TSLThreadedErrorStack::errorCallback(NULL);
}

void S63WorkFlowView::OnS63TestRunEdition_1_2()
{
  // Run the mandated tests.
  addLogLine("Commenced running of tests.");

  // Set up the test configuration object - this is owned by the worker thread we create
  // to ensure it exists for the duration of the test run.
  S63ConfigurationObject configuration;
  configuration.m_testDataLocation = m_dataLocation;
  configuration.m_configurationLocation = m_tut_configDir + "\\";
  configuration.m_resultsDirectory = m_tut_resultsDir + "\\";
  TestInformation *testInfo = new TestInformation( configuration, m_hWnd );

  // Install the MapLink error stack callback
  TSLThreadedErrorStack::errorCallback(this, TSLErrorCategoryAll);

  CWinThread *testThread = AfxBeginThread( &performMandatedTestsEdition_1_2, testInfo );
  // Display a progress dialog for the tests
  m_testProgressDialog.setNumberOfTests(NUM_IHO_TESTS_1_2);
  m_testProgressDialog.DoModal();

  // Wait for the test thread to terminate before returning control to the application
  WaitForSingleObject( testThread->m_hThread, INFINITE );

  // Clear the MapLink error stack callback
  TSLThreadedErrorStack::errorCallback(NULL);
}

void S63WorkFlowView::OnConfigureScaleBands()
{
  ScaleBandDialog scaleBands(m_workFlow);
  scaleBands.DoModal();

  if ( m_s63Layer )
  {
    m_s63Layer->notifyChanged(true);
    InvalidateRect( 0, false ) ;
  }
}

void S63WorkFlowView::OnLoadS63()
{
  LoadS63Dialog loadS63Dialog(m_workFlow);

  string dataStore("");
  string hw_id("");
  vector< pair< const TSLS63CellPermit*, string > > cellPermits;

  // Create a new 'LoadS63' dialog to allow the user to enter the required
  // display paramaters.

  if (loadS63Dialog.DoModal() != IDOK)
  {
   // If the user pressed cancel, return before we raise any errors.
   return;
  }

  if (!TSLFileHelper::directoryExists(TSLUTF8Encoder(loadS63Dialog.m_dataStore)))
  {
    addLogLine("Data Store does not exist.");
    return ;
  }
      
  // Extract the data store and cell permits from the load S63 dialog.
  dataStore = TSLUTF8Encoder(loadS63Dialog.m_dataStore);
  cellPermits.assign(loadS63Dialog.m_cellPermits.begin(), loadS63Dialog.m_cellPermits.end());

  displayS63(dataStore, cellPermits);
}

void S63WorkFlowView::loadS57DataStore(const string &dataStore, const double latitude, const double longitude, const int scale)
{
  // load the S57 data store and enable S52 rendering
  displayS57(dataStore, latitude, longitude, scale);
}

void S63WorkFlowView::OnLoadS57()
{
  LoadS57Dialog loadS57Dialog(m_workFlow);

  // Create a new 'LoadS57' dialog to allow the user to enter the required
  // display paramaters.

  if (loadS57Dialog.DoModal() != IDOK)
  {
   // If the user pressed cancel, return before we raise any errors.
   return;
  }
  
  if (!TSLFileHelper::directoryExists(TSLUTF8Encoder(loadS57Dialog.m_dataStore)))
  {
    addLogLine("Data Store does not exist.");
    return ;
  }
  // Extract the data store and cell permits from the load S63 dialog.
  string dataStore = TSLUTF8Encoder(loadS57Dialog.m_dataStore);
  loadS57DataStore(dataStore);
}

void S63WorkFlowView::OnS63TestSetup()
{
  // Generate a test setup dialog to allow the user to provide
  // all necessary parameters to run the IHO mandated tests.
  TestSetup testSetup;

  testSetup.m_dataLocation = (LPCTSTR)TSLUTF8Decoder(m_dataLocation);
  testSetup.m_configurationTime = m_tutTime;
  testSetup.m_configurationDir = (LPCTSTR)TSLUTF8Decoder(m_tut_configDir);
  testSetup.m_resultsDir = (LPCTSTR)TSLUTF8Decoder(m_tut_resultsDir);

 if (testSetup.DoModal() == IDOK)
 {
   // Extract the data provided by the users and store it for use later
   m_dataLocation = TSLUTF8Encoder(testSetup.m_dataLocation);
   m_tutTime = testSetup.m_configurationTime;
   m_tut_configDir = TSLUTF8Encoder(testSetup.m_configurationDir);
   m_tut_resultsDir = TSLUTF8Encoder(testSetup.m_resultsDir);
 }
}

void S63WorkFlowView::OnUpdateTestSetup(CCmdUI *pCmdUI)
{
  /////////////////////////////////////
  // Test Plan Reference: 3.4.1.2
  //  - Deny access to running tests until properly configured.
  /////////////////////////////////////

  // This ensures that the 'S63 Test Cases -> Run' menu is only available when
  // sufficient data has been entered into the setup dialog.
  pCmdUI->Enable( m_workFlow->unlocked() &&
                  !m_dataLocation.empty() &&
                  !m_tut_resultsDir.empty() &&
                  !m_tut_configDir.empty()) ;
}


void S63WorkFlowView::OnUpdateConfigurationDialog(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( true );
}

void S63WorkFlowView::OnUpdateAlwaysEnabled(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_workFlow->unlocked() );
}

void S63WorkFlowView::OnUpdateNeverEnabled(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( false );
}

void S63WorkFlowView::OnUpdateInstallSACertificate(CCmdUI *pCmdUI)
{
  // This method ensures that an IHO certificate can only be installed when
  // one hasn't already been specified.
  pCmdUI->Enable( m_workFlow->unlocked() &&
                 m_workFlow->ihoCertificate().empty());
}

void S63WorkFlowView::OnUpdateInstallDSCertificate(CCmdUI *pCmdUI)
{
  // This method ensures that a DS certificate can only be installed when
  // one hasn't already been specified.
  pCmdUI->Enable( m_workFlow->unlocked() &&
                 m_workFlow->dataServerCertificate().empty());
}

void S63WorkFlowView::OnUpdateRemoveSACertificate(CCmdUI *pCmdUI)
{
  // This method ensures that an IHO certificate can only be removed when
  // one has been specified.
  pCmdUI->Enable( m_workFlow->unlocked() &&
                 !m_workFlow->ihoCertificate().empty());
}

void S63WorkFlowView::OnUpdateRemoveDSCertificate(CCmdUI *pCmdUI)
{
  // This method ensures that a data server certificate can only be removed when
  // at least one has been specified.
  pCmdUI->Enable( m_workFlow->unlocked() &&
                 !m_workFlow->dataServerCertificate().empty());
}

void S63WorkFlowView::OnInstallSACertificate()
{
  installCertificate(false);
}

void S63WorkFlowView::OnInstallDSCertificate()
{
  installCertificate(true);
}

void S63WorkFlowView::installCertificate(bool dataServerCertificate)
{
  // Create a certificate management dialog, providing it with the current workflow
  // to perform S63 related operations.
  CertificateDialog certDialog(m_workFlow, dataServerCertificate);

  if ( certDialog.DoModal() != IDOK ) 
  {
    //If the user cancelled, return.
    return;
  }

  // Extract the certificate path which was specified in the dialog box.
  string certificatePath = TSLUTF8Encoder(certDialog.m_certificatePath);
  string expiryDate = TSLUTF8Encoder(certDialog.m_expiryDate);

  if ( !certificatePath.empty() )
  {
    // Install the specified certificate for use when ingesting media later.
    if ( !m_workFlow->installCertificate(certificatePath, dataServerCertificate) )
    {
      addLogLine("Error - Unable to install selected certificate.");

      AfxMessageBox(_T("The selected certificate is not valid and cannot be installed."), MB_ICONERROR);
      return;
    }
  }

  //////////////////////////////////////
  // Test Plan Reference: 3.4.2.1
  //  - Alert the user when a certificate is installed.
  /////////////////////////////////////

  // Alert the user that the certificate was successfully installed.
  string logMessage;
  
  if ( dataServerCertificate ) 
  {
    logMessage = "A new data server certificate has been installed: ";
    logMessage += certificatePath;
    logMessage += ". This is valid until ";
    logMessage += expiryDate;
    logMessage += ".";
  }
  else
  {
    logMessage = "A new SA certificate has been installed: ";
    logMessage += certificatePath;
    logMessage += ". This is valid until ";
    logMessage += expiryDate;
    logMessage += " or unless the SA issues a new one for security reasons.";
  }

  addLogLine(logMessage);
}

void S63WorkFlowView::OnRemoveSACertificate()
{
  // Provide a simple yes/no box to ask the user whether they wish to remove the SA certificate.
  if (AfxMessageBox(_T("Are you sure you wish to remove the current SA certificate?"), MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDYES)
  {
    // If yes, remove the appropriate certificate
    m_workFlow->removeIHOCertificate();
    addLogLine("Removed IHO certificate.");
  }
}

void S63WorkFlowView::OnRemoveDSCertificate()
{
  // Provide a simple yes/no box to ask the user whether they wish to clear all data server certificates.
  if (AfxMessageBox(_T("Are you sure you wish to remove the current data server certificate?"), MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDYES)
  {
    // If yes, clear all data server certificates
    m_workFlow->removeDataServerCertificate();
    addLogLine("Removed data server certificate.");
  }
}

void S63WorkFlowView::OnInstallUserPermit()
{
  // When the user selects the 'Install User Permit' dialog, provide them with a file
  // dialog to specify the user permit.

  FileDialog fileDialog(_T("S63WorkFlow - Specify user permit file"), _T("User Permit:"), _T("Text Files (*.txt)|*.txt| All Files (*.*)|*.*||"));

  if( fileDialog.DoModal() == IDOK )
  {
    // Retrieve the path to the user permit file.
    string userPermitPath = TSLUTF8Encoder(fileDialog.m_input);

    if ( userPermitPath.empty() )
    {
      return;
    }

    string logMessage("");

    // Install the user permit.
    if ( m_workFlow->installUserPermit( userPermitPath ) )
    {
      logMessage = ("User permit successfully installed: ");
      logMessage += userPermitPath;
    }
    else
    {
      logMessage = ("Unable to install user permit: ");
      logMessage += userPermitPath;
    }
    
    addLogLine(logMessage);
  }
}

void S63WorkFlowView::OnInstallCellPermit()
{
  // Create and display the cell permit management dialog.
  ManageCellPermitsDialog permitDialog(m_workFlow);

  permitDialog.DoModal();
}

void S63WorkFlowView::OnLoadLargeMedia()
{
  // Create and display the large media ingest dialog.
  LargeMediaDialog largeMedia(m_workFlow);
  
  if (largeMedia.DoModal() != IDOK)
  {
    return;
  }

  // Retrieve the information from the dialog
  string dataStore = TSLUTF8Encoder(largeMedia.m_dataStore);

  string mediaFile = TSLUTF8Encoder(largeMedia.m_largeMediaFile);

  vector<string> mediaItems;
  mediaItems.assign(largeMedia.m_mediaRecords.begin(), largeMedia.m_mediaRecords.end());

  if ( dataStore.empty() && (!mediaItems.size()) && mediaFile.empty() ) 
  {
    // If they haven't selected anything, don't raise an error.
    return;
  }

  // Ensure a data store has been specified
  if ( dataStore.empty() ) 
  {
    addLogLine("Error - Unable to ingest large media, no data store specified.");

    AfxMessageBox(_T("Unable to ingest large media, no data store specified."), MB_ICONERROR);
    return;
  }

  // Ensure media records have been selected
  if ( !mediaItems.size() )
  {
    addLogLine("Error - Unable to ingest large media, no records selected.");

    AfxMessageBox(_T("Unable to ingest large media, no records selected."), MB_ICONERROR);
    return;
  }

  if ( mediaFile.empty() )
  {
    addLogLine("Error - Unable to ingest large media, no media file specified.");

    AfxMessageBox(_T("Unable to ingest large media, no media file specified."), MB_ICONERROR);
    return;
  }

  // Firstly, extract the path from the media file path.
  wchar_t file_drive[_MAX_DRIVE];
  wchar_t file_dir[TSL_MAX_WPATH];
  std::wstring file_path;
  
  _wsplitpath(TSLUTF8Decoder(mediaFile), file_drive, file_dir, NULL, NULL);

  file_path = file_drive;
  file_path += file_dir;

  string mediaPath = TSLUTF8Encoder( file_path );

  // Get the path to the products file
  string productsFile(mediaPath);
  productsFile += "INFO\\PRODUCTS.TXT";

  // If the products file doesn't exist we must exit
  if( !TSLFileHelper::fileExists( productsFile.c_str() ) )
  {
    addLogLine("Error - Unable to ingest large media, could not locate PRODUCTS.TXT file.");

    AfxMessageBox(_T("Error - Unable to ingest large media, could not locate PRODUCTS.TXT file."), MB_ICONERROR);
    return;
  }

  // Parse the media records, so we know where to retrieve files from.
  vector<string>::iterator recordIt;
  for ( recordIt = mediaItems.begin() ; recordIt != mediaItems.end() ; ++recordIt )
  {
    // Extract the 'BXX' section from the media record
    int startIndex = recordIt->find(";", 0);
    int endIndex = recordIt->find(" - ", 0);

    ++startIndex;

    // Extract the path to the exchange set
    string folderLocation(mediaPath);
    folderLocation += recordIt->substr(startIndex, (endIndex - startIndex));
    folderLocation += "\\";

    // Get the SERIAL.ENC file
    string serialFile(folderLocation);
    serialFile += "ENC_ROOT\\SERIAL.ENC";

    // Check the file exists before proceeding
    // If the serial file doesn't exist, we raise an error and proceed
    if( !TSLFileHelper::fileExists( productsFile.c_str() ) )
    {
      addLogLine("Error - Unable to ingest large media dataset, could not locate SERIAL.ENC file.");
      AfxMessageBox(_T("Error - Unable to ingest large media dataset, could not locate SERIAL.ENC file."), MB_ICONERROR);
      continue;
    }

    string alertMessage("Commencing ingest of exchange set '");
    alertMessage += folderLocation;
    alertMessage += "'";

    AfxMessageBox(TSLUTF8Decoder(alertMessage), MB_ICONINFORMATION );
    addLogLine(alertMessage);
     
    // In order to keep the user interface responsive during an ingest,
    // create a background thread to perform the actual work for the ingest.
    IngestParameters threadParams( m_workFlow, dataStore, productsFile, serialFile, m_hWnd );

    // Perform the media ingest using the provided files
    CWinThread *backgroundThread = AfxBeginThread( &performIngest, &threadParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
    backgroundThread->m_bAutoDelete = FALSE;
    backgroundThread->ResumeThread();

    if (!m_ingestProgressDialog)
      m_ingestProgressDialog = new ProgressDialog(this);
    m_nTimerID = SetTimer (m_nTimerID, ELAPSED_TIME, NULL);
    m_ingestProgressDialog->DoModal();

    DWORD exitCode = 0;
    WaitForSingleObject( backgroundThread->m_hThread, INFINITE );
    GetExitCodeThread( backgroundThread->m_hThread, &exitCode );
    delete backgroundThread;
    if (m_nTimerID)
      KillTimer(m_nTimerID);
    m_nTimerID = 0;

    if( exitCode == 1 )
    {
      addLogLine("Media import successfully completed.");
      //AfxMessageBox( "The media import completed successfully.", MB_ICONINFORMATION );
      displayS63(dataStore, m_workFlow->cellPermits());
    }
    else
    {
      addLogLine("Media import was unsuccessful. Please see the log for details.");
      //AfxMessageBox( "The media import completed with errors.", MB_ICONERROR );
      displayS63(dataStore, m_workFlow->cellPermits());
    }

    // Print the contents of the error stack to the log window to provide notice of
    // any errors or warnings
    int errorCode = 0;
    TSLSimpleString info;

    while( TSLThreadedErrorStack::next( errorCode, info ) )
    {
      stringstream errMessage;
      errMessage << "Error " << errorCode << ": ";
      const char *message = TSLErrorStack::lookupMessage( errorCode, NULL );
      if( message )
      {
        errMessage << message << " - ";
      }
      if( !info.empty() )
      {
        errMessage << info.c_str();
      }

      addLogLine( errMessage.str().c_str() );
    }

    TSLThreadedErrorStack::clear();

  }
}

void S63WorkFlowView::OnLoadMedia()
{
  string productFile, serialFile, storeLocation;

  // Create and display the media ingest dialog.
  LoadMediaDialog loadMedia;

  if (loadMedia.DoModal() != IDOK)
  {
    //don't return an error if they cancel
    return;
  }

  // Extract the provided information from the dialog
  productFile = TSLUTF8Encoder(loadMedia.m_productFile);
  serialFile = TSLUTF8Encoder(loadMedia.m_serialFile);
  storeLocation = TSLUTF8Encoder(loadMedia.m_dataStore);

  // Ensure they've provided a PRODUCTS.TXT file.
  if ( productFile.empty() )
  {
    string msg("Error - Cannot proceed without a PRODUCTS.txt file.");
    addLogLine(msg);
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;  
    return;
  }

  // Ensure they've provided a SERIAL.ENC file.
  if ( serialFile.empty() )
  {
    addLogLine("Error - Cannot proceed without a SERIAL.enc file.");
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;  
    return;
  }

  // Ensure they've provided a valid store location.
  if ( storeLocation.empty() )
  {
    addLogLine("Error - Cannot proceed without a data store location.");
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;  
    return;
  }

  addLogLine("Beginning media ingest.");


  // Generate a path, relative to the products file to open the 'Readme.txt' for the dataset
  wchar_t file_drive[_MAX_DRIVE];
  wchar_t file_dir[TSL_MAX_WPATH];

  _wsplitpath(TSLUTF8Decoder(productFile), file_drive, file_dir, NULL, NULL);
  
  //////////////////////////////////////
  // Test Plan Reference: 3.4.2.3
  //  - Display the README.txt
  /////////////////////////////////////

  wstring readmePath = file_drive;
  readmePath += file_dir;
  readmePath += L"../ENC_ROOT/README.TXT";

  // check the file exists and contains something.
  if (TSLFileHelper::fileExists((const char *)TSLUTF8Encoder(readmePath)))
  {
    if (TSLFileHelper::fileSize((const char *)TSLUTF8Encoder(readmePath)) > 0)
    {
      // Open Notepad and display the 'README.TXT' file for the dataset.
      wstring sysCommand(L"notepad.exe ");
      ShellExecute( NULL, L"open", readmePath.c_str(), NULL, NULL, SW_SHOW  );
    }
  }

  // In order to keep the user interface responsive during an ingest,
  // create a background thread to perform the actual work for the ingest.
  IngestParameters threadParams( m_workFlow, storeLocation, productFile, serialFile, m_hWnd );

  // Perform the media ingest using the provided files 
  CWinThread *backgroundThread = AfxBeginThread( &performIngest, &threadParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
  backgroundThread->m_bAutoDelete = FALSE;
  backgroundThread->ResumeThread();

  if (!m_ingestProgressDialog)
    m_ingestProgressDialog = new ProgressDialog(this);
  m_nTimerID = SetTimer (m_nTimerID, ELAPSED_TIME, NULL);
  m_ingestProgressDialog->DoModal();

  // Query the return value from the ingest thread once the thread has terminated
  DWORD exitCode = 0;
  WaitForSingleObject( backgroundThread->m_hThread, INFINITE );
  GetExitCodeThread( backgroundThread->m_hThread, &exitCode );
  delete backgroundThread;
  if (m_nTimerID)
    KillTimer(m_nTimerID);
  m_nTimerID = 0;

  if( exitCode == 1 )
  {
    addLogLine("Media import successfully completed.");
    //AfxMessageBox( "The media import completed successfully.", MB_ICONINFORMATION );
    displayS63(storeLocation, m_workFlow->cellPermits());
  }
  else
  {
    addLogLine("Media import was unsuccessful. Please see the log for details.");
    //AfxMessageBox( "The media import completed with errors.", MB_ICONERROR );
    displayS63(storeLocation, m_workFlow->cellPermits());
  }

  // Print the contents of the error stack to the log window to provide notice of
  // any errors or warnings
  int errorCode = 0;
  TSLSimpleString info;
  while( TSLThreadedErrorStack::next( errorCode, info ) )
  {
    stringstream errMessage;
    errMessage << "Error " << errorCode << ": ";
    const char *message = TSLErrorStack::lookupMessage( errorCode, NULL );
    if( message )
    {
      errMessage << message << " - ";
    }
    if( !info.empty() )
    {
      errMessage << info.c_str();
    }

    addLogLine( errMessage.str().c_str() );
  }

  TSLThreadedErrorStack::clear();
}

void S63WorkFlowView::OnLoadS57Media()
{
  string catalogFile, storeLocation;

  // Create and display the media ingest dialog.
  LoadS57MediaDialog loadMedia;

  if (loadMedia.DoModal() != IDOK)
  {
    //don't return an error if they cancel
    return;
  }

  catalogFile = TSLUTF8Encoder(loadMedia.m_catalogFile);
  storeLocation = TSLUTF8Encoder(loadMedia.m_dataStore);

  // Ensure they've provided a CATALOG or S57 base file.
  if ( catalogFile.empty() )
  {
    string msg("Error - Cannot proceed without a CATALOG or S57 base file.");
    addLogLine(msg);
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;  
    return;
  }

  // Ensure they've provided a valid store location.
  if ( storeLocation.empty() )
  {
    addLogLine("Error - Cannot proceed without a data store location.");
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;  
    return;
  }

  addLogLine("Beginning S57 media ingest.");
        
  // In order to keep the user interface responsive during an ingest,
  // create a background thread to perform the actual work for the ingest.
  IngestParameters threadParams( m_workFlow, storeLocation, catalogFile, "", m_hWnd );

  // Perform the media ingest using the provided files 
  CWinThread *backgroundThread = AfxBeginThread( &performS57Ingest, &threadParams, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
  backgroundThread->m_bAutoDelete = FALSE;
  backgroundThread->ResumeThread();

  if (!m_ingestProgressDialog)
    m_ingestProgressDialog = new ProgressDialog(this);
  m_nTimerID = SetTimer (m_nTimerID, ELAPSED_TIME, NULL);
  m_ingestProgressDialog->DoModal();

  // Query the return value from the ingest thread once the thread has terminated
  DWORD exitCode = 0;
  WaitForSingleObject( backgroundThread->m_hThread, INFINITE );
  GetExitCodeThread( backgroundThread->m_hThread, &exitCode );
  delete backgroundThread;
  if (m_nTimerID)
    KillTimer(m_nTimerID);
  m_nTimerID = 0;

  if( exitCode == 1 )
  {
    addLogLine("Media import successfully completed.");
    //AfxMessageBox( "The media import completed successfully.", MB_ICONINFORMATION );

    // The ingest was successful so display the new data.
    displayS57(storeLocation);
  }
  else
  {
    addLogLine("Media import was unsuccessful. Please see the log for details.");
    //AfxMessageBox( "The media import completed with errors.", MB_ICONERROR );
    displayS57(storeLocation);
  }

  // Print the contents of the error stack to the log window to provide notice of
  // any errors or warnings
  int errorCode = 0;
  TSLSimpleString info;
  while( TSLThreadedErrorStack::next( errorCode, info ) )
  {
    stringstream errMessage;
    errMessage << "Error " << errorCode << ": ";
    const char *message = TSLErrorStack::lookupMessage( errorCode, NULL );
    if( message )
    {
      errMessage << message << " - ";
    }
    if( !info.empty() )
    {
      errMessage << info.c_str();
    }

    addLogLine( errMessage.str().c_str() );
  }

  TSLThreadedErrorStack::clear();
}

void S63WorkFlowView::OnUpdateLoadMedia(CCmdUI *pCmdUI)
{
  // This method ensures that the 'Load/Update Media' menu item is only available
  // when cell permits, iho certificate and hardware id has been specified.
  pCmdUI->Enable( m_workFlow->unlocked() &&
                 !m_workFlow->cellPermits().empty() &&
                 !m_workFlow->ihoCertificate().empty() &&
                 !m_workFlow->hwid().empty());
}

bool S63WorkFlowView::catalogueCallback(string &catalogueLocation)
{
  // When the Catalogue Callback gets hit it is an opportunity for the user to specify the location 
  // of the CATALOG.031 file to use in the current ingest.

  // Open a file dialog to allow the user to specify the catalogue file.
  FileDialog fileDialog(_T("Please locate the CATALOG.031 file."), _T("CATALOG.031:"), _T("CATALOG Files (*.031)|*.031|All Files (*.*)|*.*||"));

  if (fileDialog.DoModal() == IDOK)
  {
    // Retrieve the catalogue location from the file dialog box
    catalogueLocation = TSLUTF8Encoder(fileDialog.m_input);
    return true;
  }

  return false;
}

void S63WorkFlowView::notifyErrorCallback(int s63ErrorCode, const char* cellName, const char* dataServerID, const char* additionalInformation, void * arg)
{   
  // When the Notify Error Callback gets hit, it's an opportunity to use the 
  // GUI to report the provided error.
  ostringstream errorMsg;

  // Ensure that permenant warnings are disabled if appropriate
  if ( m_disableWarnings && ( s63ErrorCode == 20 || s63ErrorCode == 25 || s63ErrorCode == 27 ) )
  {
    return;
  }

  // If the returned error code is '-1', assume it's a warning.
  if ( s63ErrorCode != -1 )
  {
    // If a cell and dataserver haven't been provided, provide a default error message
    if ( !cellName || !dataServerID ) 
    {
      errorMsg << "Error [SSE ";
      errorMsg.width( 2 );
      errorMsg.fill( '0' );
      errorMsg.setf( ios::dec );
      errorMsg << s63ErrorCode << "]: " << additionalInformation;
    }
    else // Otherwise give a more informative one.
    {
      errorMsg << "(DataServer: " << dataServerID << ",  Cell: " << cellName << ") Error [SSE ";
      errorMsg.width( 2 );
      errorMsg.fill( '0' );
      errorMsg.setf( ios::dec );
      errorMsg << s63ErrorCode << "]: " << additionalInformation;
    }
  }
  else // Display the warning.
  {
    errorMsg << "Warning: " << additionalInformation;
  }

  if ( arg ) 
  {      
    string *message = new string( errorMsg.str() );
    ::PostMessage( *((HWND*)arg), WM_LOG_MESSAGE, 0, LPARAM(message) );
  }
  else
  {
    // Log the error
    addLogLine( errorMsg.str() );  
  }
}

bool S63WorkFlowView::ingestCellCallback(const TSLSimpleString& cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber)
{
  // The Ingest Cell Callback is hit when a user needs to determine whether or not to ingest the current
  // cell. 

  // Therefore, we provide a dialog which displays all cell information 
  // and the option of cancelling or proceeding.
  IngestCell ingestCell(cellName.c_str(), dataServerID, cellEdition, cellUpdateNumber, cellExtent);

  return (ingestCell.DoModal() == IDOK);
}

bool S63WorkFlowView::cancelledCellCallback(const char* cellName, const char* dataServerID, const char ** cellReplacements, int numberOfCells)
{    
  // The Cancelled Cell Callback is hit when an update contains a cell which has been cancelled.

  // Therefore, we provide a dialog which displays all cell information 
  // and the option of keeping or discarding the cell.
  CancelledCellDialog cancelledCell(cellName, dataServerID);

  return (cancelledCell.DoModal() == IDOK);
}

bool S63WorkFlowView::progressCallback(double percentOfExchangeSetDone, double percentOfCurrentCellDone, void *arg)
{
  // Post a user message to the UI thread so it can update the progress dialog.
  // The user-defined argument contains the HWND of the window to post the message to
  ProgressAmount *progress = new ProgressAmount;
  progress->percentTotal = percentOfExchangeSetDone;
  progress->percentOfCell = percentOfCurrentCellDone;
  ::PostMessage( *((HWND*)arg), WM_UPDATE_INGEST_PROGRESS_DIALOG, 0, (LPARAM)progress );

  // If the ingest has been cancelled via the dialog, return false to abort the ingest
  if (!m_ingestProgressDialog)
    return true;

  return !m_ingestProgressDialog->hasBeenCancelled();
}

BOOL S63WorkFlowView::PreTranslateMessage(MSG* pMsg)
{
  // MFC does not automatically handle windows messages passed in from a different thread,
  // therefore we need to process these manually.
  static double percentTotal = 0.0;
  static double percentOfCell = 0.0;

  switch( pMsg->message )
  {
  case WM_UPDATE_INGEST_PROGRESS_DIALOG:
    {
      // Update the progress dialog to reflect the progress of the ingest.
      ProgressAmount *progress = (ProgressAmount*)pMsg->lParam;
      if (m_ingestProgressDialog)
      {
        m_ingestProgressDialog->setProgress( progress->percentTotal, progress->percentOfCell );
      }
      percentOfCell = progress->percentOfCell;
      percentTotal = progress->percentTotal;
      delete progress;
      return TRUE;
    }

  case WM_CLOSE_INGEST_PROGRESS_DIALOG:
    {
      // The ingest thread has requested we close the progress dialog.
      if (m_ingestProgressDialog)
        m_ingestProgressDialog->EndDialog( 1 );
      if (m_nTimerID)
        KillTimer(m_nTimerID);
      m_nTimerID = 0;
      percentTotal = 0.0;
      percentOfCell = 0.0;
      return TRUE;
    }

  case WM_UPDATE_TEST_PROGRESS_DIALOG:
    {
      // Update the progress dialog to reflect the progress of the ingest.
      ProgressAmount *progress = (ProgressAmount*)pMsg->lParam;
      m_testProgressDialog.setProgress( (int)progress->percentTotal );
      delete progress;
      return TRUE;
    }

  case WM_CLOSE_TEST_PROGRESS_DIALOG:
    {
      // The ingest thread has requested we close the progress dialog.
      m_testProgressDialog.EndDialog( 1 );
      return TRUE;
    }

  case WM_LOG_MESSAGE:
    {
      // One of our worker threads has requested we add some information to the log window
      string *message = (string*)pMsg->lParam;
      addLogLine( *message );
      delete message;
      return TRUE;
    }
  case WM_TIMER:
    // provide an indication of progress.
    if (m_ingestProgressDialog && m_nTimerID)
      m_ingestProgressDialog->setProgress( percentTotal, percentOfCell );
    percentOfCell += 10;
    if (percentOfCell > 90)
      percentOfCell = 20;
    return TRUE;

  default:
    return __super::PreTranslateMessage(pMsg);
  }
}

void S63WorkFlowView::OnUpdateCurrentScale(CCmdUI *pCmdUI)
{
  pCmdUI->Enable( m_drawingSurface != 0 );

  updateScale(pCmdUI);
}

void S63WorkFlowView::updateScale(CCmdUI *pCmdUI)
{
  // Ensure we have a drawing surface before proceeding.
  if ( !m_drawingSurface )
    return;
  // Set the requested scale for display on menu etc...
  if (m_zoomScalePtr != NULL && m_zoomScalePtr->m_scaleChanged)
  {
    setScale(m_zoomScalePtr->m_scale, false);
    m_zoomScalePtr->m_scaleChanged = false;
  }

  TCHAR buffer[256];
  int hdpi, vdpi;

  // Get the DPI of the monitor in use.
  m_workFlow->getDPI(vdpi, hdpi);

  // convert the dpi to dots per metre.
  const double dotsPerMetre = 100 * vdpi / CM_PER_INCH;

  // Retrieve the number of TMC's per pixel.
  double tmcPerDuX, tmcPerDuY;
  m_drawingSurface->TMCperDU(tmcPerDuX, tmcPerDuY);

  // And the number of TMC's per map unit.
  double tmcPerMu = m_drawingSurface->TMCperMU();

  // Using these values we can calculate the number of pixels which represent one map unit.
  double DuPerMU = tmcPerMu / tmcPerDuY;

  // We can now calculate the scale.
  double MUPerScreenMetreY = dotsPerMetre / DuPerMU ;

  int muPerScreenMetre = (int)(MUPerScreenMetreY + 0.5);

  if (m_muPerScreenMetre != muPerScreenMetre)
  {
    // And display it in the status bar.
    _sntprintf( buffer, 255, _T("Scale: 1:%d"), muPerScreenMetre );

    // And update the S52 state object.
    m_s52StateObject->setScale(muPerScreenMetre);
    
    if (pCmdUI)
    {
      pCmdUI->SetText(buffer);
    }
    else
    {
      CMainFrame * pFrame = static_cast< CMainFrame * >( AfxGetMainWnd() ) ;
      CStatusBar * pStatus = pFrame->getStatusBar();

      if ( pStatus )
      {
        pStatus->SetPaneText(1, buffer);
      }
    }
    if (m_zoomScalePtr != NULL)
    {
      m_zoomScalePtr->setScale(TSLUTF8Encoder(buffer));
    }

    m_muPerScreenMetre = muPerScreenMetre ;
  }
}


void S63WorkFlowView::displayS63(const string& dataStore, vector< pair< const TSLS63CellPermit*, string > > &cellPermits)
{
  // -1 on scale causes display to be centered on the center of the loaded datastore.
  display(dataStore, 0.0, 0.0, -1, cellPermits, true);
}


void S63WorkFlowView::displayS57(const string& dataStore, const double latitude, const double longitude, const int scale)
{
  vector< pair< const TSLS63CellPermit*, string > > cellPermits;
  display(dataStore, latitude, longitude, scale, cellPermits, false);
}

void S63WorkFlowView::display(const string& dataStore, const double latitude, const double longitude, const int scale, vector< pair< const TSLS63CellPermit*, string > > &cellPermits, bool isS63)
{
  // Get the document for use with layer manipulation
  S63WorkFlowDoc * doc = GetDocument() ;
  bool updateScaleBands = false;
  // Query the workflow to generate the S63 Layer.
  if ( !m_s63Layer || m_hardwareIDChanged || !isS63)
  {
    // If the hardware ID has changed we need to create the TSLS63DataLayer.
    m_hardwareIDChanged = false;
    m_s63Layer = NULL;

    m_workFlow->generateDataLayer(m_hardwareID, m_cacheSize, m_s63Layer);
    
    // Add the layer to the drawing surface, so that the user can see the S63.
    // The old layer is deleted by the doc.
    doc->addS63LayerToSurface(m_s63Layer, m_drawingSurface);  
    updateScaleBands = true;
  }
  
  // Ensure that we received valid layers before we proceed.
  if ( !m_s63Layer )
  {
    // Add the contents of the error stack to provide an indication of what went wrong
    int errorCode = 0;
    TSLSimpleString info;
    while( TSLThreadedErrorStack::next( errorCode, info ) )
    {
      stringstream errMessage;
      errMessage << "Error " << errorCode << ": ";
      const char *message = TSLErrorStack::lookupMessage( errorCode, NULL );
      if( message )
      {
        errMessage << message << " - ";
      }
      if( !info.empty() )
      {
        errMessage << info.c_str();
      }

      addLogLine( errMessage.str().c_str() );
    }

    TSLThreadedErrorStack::clear();

    addLogLine("Error - Unable to generate S63 data layer.");
    AfxMessageBox( _T("An error has occurred, please check the Log Window."), MB_ICONERROR ) ;
    return;
  }

  // If S52 is enabled, ensure the rendering has been loaded
  if ( m_enableS52 )
  {
    loadS52Rendering();
  }

  S63ErrorLayer * errorLayer = NULL;
  
  // Add the data source to the workflow.
  bool permitAdded;
  if (isS63)
  {
    permitAdded = m_workFlow->addS63DataSource(m_s63Layer, dataStore, dataStore, cellPermits, errorLayer);
  }
  else
  {
    permitAdded = m_workFlow->addS57DataSource(m_s63Layer, dataStore, dataStore, errorLayer);
  }
  
  if ( errorLayer )
  {
    // Set whether or not errors are disabled on the error layer
    errorLayer->disableErrors(m_disableWarnings);
  
    // Add the Error layer to a custom data layer.
    TSLCustomDataLayer * customDataLayer = new TSLCustomDataLayer();
    customDataLayer->setClientCustomDataLayer(errorLayer);
  
    // Finally add the custom layer and the S63 layer to the drawing surface
    doc->addCustomLayerToSurface(dataStore.c_str(), customDataLayer, m_drawingSurface);
    
    m_s63ErrorLayers.push_back(errorLayer);
    m_s63CustomErrorLayers.push_back(customDataLayer);
  }

  // Set the S63 as being loaded
  m_s63Loaded = true;
  
  // Update the Scale Bands, now that the layer has been added to the drawing surface.
  if (updateScaleBands)
  {
    m_workFlow->updateScaleBands();
    if (m_scaleBandPtr)
      m_scaleBandPtr->SetCurSel(0);
  }
  m_s63Layer->notifyChanged();
  
  if (scale != -1)
  {
    // initial start display position and scale - set on command line.
    m_drawingSurface->setViewedLatLongRange(latitude, longitude, 1024 * 10, false, false);
    setScale(scale);   
  }
  else
  {
    // Finally, centre on the middle of the data extent the surface to display our newly loaded S57 data
    if (updateScaleBands)
    {
      TSLTMC x1, y1, x2, y2;
      m_s63Layer->getTMCExtent(&x1,&y1,&x2,&y2);
      double blLat, blLon;
      double trLat, trLon;
      m_s63Layer->TMCToLatLong(x1, y1, &blLat, &blLon);
      m_s63Layer->TMCToLatLong(x2, y2, &trLat, &trLon);

      double lat = ((trLat - blLat)/2.0) + blLat;
      double lon = ((trLon - blLon)/2.0) + blLon;

      m_drawingSurface->setViewedLatLongRange(lat, lon, 1024 * 10, false, false);
      setScale(INITIAL_SCALE);
    }
  }
  InvalidateRect(NULL, false);
}

void S63WorkFlowView::loadS63Rendering()
{
  if ( !m_drawingSurface )
  {
    return;
  }

  if (!m_s63Layer || m_hardwareIDChanged)
  {
    // If the hardware ID has changed then we need to recreate the TSLS63DataLayer
    m_hardwareIDChanged = false;
    m_s63Layer = NULL;

    m_workFlow->generateDataLayer(m_hardwareID, m_cacheSize, m_s63Layer);

    // Get the document for use with layer manipulation
    S63WorkFlowDoc * doc = GetDocument() ;

    // Add the layer to the drawing surface, so that the user can see the S63.
    // The old layer is deleted in addS63LayerToSurface
    doc->addS63LayerToSurface(m_s63Layer, m_drawingSurface);    
  }

  // If there's an S52 no data layer added, remove it.
  if ( m_s52NoDataLayer )
  {
    m_s52NoDataLayer->destroy();
    m_s52NoDataLayer = NULL;
  }

  // Clear any existing rendering.
  m_drawingSurface->clearAllRendering();
  m_drawingSurface->clearAllDynamicRenderers();

  // Construct the path to the rendition file
  string pathToRendition;
  string mapLinkConfigDir;

  if (!getMapLinkConfigDirectory(mapLinkConfigDir))
  {
    AfxMessageBox(_T("Unable to find the S52 configuration directory."), MB_ICONERROR ) ;
    return ;
  }
  pathToRendition = mapLinkConfigDir + "/s52/S63.rnd";

  // Load the rendering information on to the drawing surface
  if ( m_s63Layer && !m_s63Layer->loadRendering(pathToRendition.c_str()) )
  {
    //If we can't load it from the samples directory, attempt to load it locally
    if ( !m_s63Layer->loadRendering("S63.rnd") )
    {
      AfxMessageBox(_T("Unable to load rendering file."), MB_ICONERROR );
    }
  }
}

void S63WorkFlowView::loadS52Rendering()
{   
  // Check to see if the S52 No Data Layer is present in the sample.
  if ( !m_s52NoDataLayer)
  {
    // Get the document for use with layer manipulation
    S63WorkFlowDoc * doc = GetDocument() ;

    if ( !m_s52NoDataLayer )
    {
      // Create a new no data layer
      m_s52NoDataLayer = new TSLS52NoDataLayer(NULL);

      // Add it to the surface
      doc->addLayerToSurface(NO_DATA_LAYER_NAME, m_s52NoDataLayer, m_drawingSurface);

      // Make it so we can not select the information.
      m_drawingSurface->setDataLayerProps(NO_DATA_LAYER_NAME, TSLPropertySelect, (TSLPropertyValue)false);
    }
  }

  if (!m_s63Layer || m_hardwareIDChanged)
  {
    // If the hardware ID has changed we need to recreate the TSLS63DataLayer
    m_hardwareIDChanged = false;
    m_s63Layer = NULL;

    m_workFlow->generateDataLayer(m_hardwareID, m_cacheSize, m_s63Layer);

    // Get the document for use with layer manipulation
    S63WorkFlowDoc * doc = GetDocument() ;
    // Add the layer to the drawing surface, so that the user can see the S63.
    // The old layer is deleted in addS63LayerToSurface
    doc->addS63LayerToSurface(m_s63Layer, m_drawingSurface);    
  }


  // And send it to the back.
  m_drawingSurface->sendToBack(NO_DATA_LAYER_NAME);
  m_drawingSurface->bringInFrontof(NO_DATA_LAYER_NAME, S63WorkFlowDoc::m_mapLayerName);

  // is it visible?
  m_drawingSurface->setDataLayerProps(NO_DATA_LAYER_NAME, TSLPropertyVisible, (TSLPropertyValue)m_s52StateObject->displayNoDataLayer());

  // Clear any existing rendering.
  m_drawingSurface->clearAllRendering();  
  m_drawingSurface->clearAllDynamicRenderers();

  // Construct the path to the S52 rendition file.
  string pathToRendition;
  string mapLinkConfigDir;

  if (!getMapLinkConfigDirectory(mapLinkConfigDir))
  {
    AfxMessageBox(_T("Unable to find the S52 configuration directory."), MB_ICONERROR ) ;
    return ;
  }
  pathToRendition = mapLinkConfigDir + "/s52/S52.rnd";
  mapLinkConfigDir += "/s52";

  TSLS52DynamicRenderer::s52HomeDirectory(mapLinkConfigDir.c_str());

  // Create the S52 renderer.
  m_s52DynamicRenderer = new TSLS52DynamicRenderer(m_s52StateObject, m_userRenderingProcedure);

  // And add it to the drawing surface
  if (!m_drawingSurface->addDynamicRenderer(m_s52DynamicRenderer, -1, S63WorkFlowDoc::m_s63LayerName))
  {
    // we do not have a layer yet so delete the dynamic renderer
    m_s52DynamicRenderer->destroy();
    m_s52DynamicRenderer = NULL;
  }

  // Load the rendering information on to the drawing surface
  if ( !m_s52NoDataLayer->loadRendering(pathToRendition.c_str()) )
  {
    //If we can't load it from the samples directory, attempt to load it locally
    if ( !m_s52NoDataLayer->loadRendering("S52.rnd") )
    {
      AfxMessageBox(_T("Unable to load rendering file."), MB_ICONERROR );
    }
  }

  // Load the rendering information on to the drawing surface
  if ( !m_s63Layer->loadRendering(pathToRendition.c_str()) )
  {
    //If we can't load it from the samples directory, attempt to load it locally
    if ( !m_s63Layer->loadRendering("S52.rnd") )
    {
      AfxMessageBox(_T("Unable to load rendering file."), MB_ICONERROR );
    }
  }
}

S63WorkFlowView * S63WorkFlowView::GetView()
{
  //http://support.microsoft.com/kb/108587
  CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

  if (!pFrame)
  {
    return NULL;
  }

  CView * pView = pFrame->GetActiveView();

  if ( !pView )
    return NULL;

  // Fail if view is of wrong kind
  // (this could occur with splitter windows, or additional
  // views on a single document
  if ( ! pView->IsKindOf( RUNTIME_CLASS(S63WorkFlowView) ) )
    return NULL;

  return (S63WorkFlowView *) pView;
}

///////////////////////////////////////////////////////////////////
// Scale banding
// TBD : convert numbers to enum
void S63WorkFlowView::OnViewScaleBandFixDefault()
{
  m_scaleBandOption = 0;
  bool check = updateScaleBands(ID_VIEW_DEFAULTSCALEBANDDISPLAY);
}
void S63WorkFlowView::OnViewScaleBandFixIgnore()
{
  m_scaleBandOption = 1;
  bool check = updateScaleBands(ID_VIEW_SCALEBANDOVER);
}
void S63WorkFlowView::OnViewScaleBandFixOverview()
{
  m_scaleBandOption = 2;
  bool check = updateScaleBands(ID_VIEW_OVERVIEWSCALEBAND);
}
void S63WorkFlowView::OnViewScaleBandFixGeneral()
{
  m_scaleBandOption = 3;
  bool check = updateScaleBands(ID_VIEW_GENERALSCALEBAND);
}
void S63WorkFlowView::OnViewScaleBandFixCoastal()
{
  m_scaleBandOption = 4;
  bool check = updateScaleBands(ID_VIEW_COASTALSCALEBAND);
}
void S63WorkFlowView::OnViewScaleBandFixApproach()
{
  m_scaleBandOption = 5;
  bool check = updateScaleBands(ID_VIEW_APPROACHSCALEBAND);
}
void S63WorkFlowView::OnViewScaleBandFixHarbour()
{
  m_scaleBandOption = 6;
  bool check = updateScaleBands(ID_VIEW_HARBOURSCALEBAND);
}
void S63WorkFlowView::OnViewScaleBandFixBerthing()
{
  m_scaleBandOption = 7;
  bool check = updateScaleBands(ID_VIEW_BERTHINGSCALEBAND);
}

void S63WorkFlowView::OnViewScaleBandFixUpdate(CCmdUI *pCmdUI)
{
  bool check = updateScaleBands(pCmdUI->m_nID);

  pCmdUI->Enable( true ) ;
  pCmdUI->SetCheck( check ) ;
}

void S63WorkFlowView::OnUpdateViewShowLat(CCmdUI *pCmdUI)
{
  S63WorkFlowDoc * pDoc = GetDocument() ;

  if ( pDoc != NULL )
  {
    bool   bCheck;

    if ( pDoc->showingGridLayer() )
    {
      bCheck = true;
    }
    else
    {
      bCheck = false;
    }

    pCmdUI->SetCheck( bCheck ) ;
  }
}

void S63WorkFlowView::OnViewShowLat()
{
  S63WorkFlowDoc * pDoc = GetDocument() ;

  if ( pDoc != NULL )
  {
    if ( pDoc->showingGridLayer() )
    {
      pDoc->removeGridLayerFromSurface( m_drawingSurface );
    }
    else
    {
      pDoc->addGridLayerToSurface( m_drawingSurface );
    }
    m_drawingSurface->redraw();
  }
}

void S63WorkFlowView::OnViewShowMGRS()
{
  S63WorkFlowDoc * pDoc = GetDocument() ;

  if (pDoc != NULL)
  {
    if ( pDoc->showingMGRSLayer() )
    {
      pDoc->removeMGRSGridLayerFromSurface( m_drawingSurface );
    }
    else
    {
      pDoc->addMGRSGridLayerToSurface( m_drawingSurface );
    }
    m_drawingSurface->redraw();
  }
}

void S63WorkFlowView::OnUpdateViewShowMGRS(CCmdUI *pCmdUI)
{
  S63WorkFlowDoc * pDoc = GetDocument();

  if ( pDoc != NULL )
  {
    bool bCheck;

    if( pDoc->showingMGRSLayer() )
    {
      bCheck = true;
    }
    else
    {
      bCheck = false;
    }

    pCmdUI->SetCheck( bCheck );
  }
}


bool S63WorkFlowView::updateScaleBands(int id)
{
  bool check = false;
  double lower = 1.0, upper = 9999999999.00;
  switch (m_scaleBandOption)
  {
   case 1: // overscale
      check = id == ID_VIEW_SCALEBANDOVER;
      if (check)
      {
        // all bands need to be present - the overscale will then force all bands to display.
        m_workFlow->clearScaleBands();
        m_workFlow->updateScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandOverscale, lower, upper, false);
      }
      break;
    case 2:
      check = id == ID_VIEW_OVERVIEWSCALEBAND;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandOverview, lower, upper, false);
      }
      break;
    case 3:
      check = id == ID_VIEW_GENERALSCALEBAND;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandGeneral, lower, upper, false);
      }
      break;
    case 4:
      check = id == ID_VIEW_COASTALSCALEBAND;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandCoastal, lower, upper, false);
      }
      break;
    case 5:
      check = id == ID_VIEW_APPROACHSCALEBAND;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandApproach, lower, upper, false);
      }
      break;
    case 6:
      check = id == ID_VIEW_HARBOURSCALEBAND;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandHarbour, lower, upper, false);
      }
      break;
    case 7:
      check = id == ID_VIEW_BERTHINGSCALEBAND;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->setScaleBand(TSLS63ScaleBandBerthing, lower, upper, false);
      }
      break;
    case 0: // drop through
    default:
      check = id == ID_VIEW_DEFAULTSCALEBANDDISPLAY;
      if (check)
      {
        m_workFlow->clearScaleBands();
        m_workFlow->updateScaleBands();
      }
      break;
  }
  if (check)
  {
    if (m_s63Layer)
      m_s63Layer->notifyChanged(true);
    InvalidateRect( 0, false ) ;
  }
  return check;
}


void S63WorkFlowView::OnEnterIdle(UINT nWhy, CWnd* pWho)
{
  __super::OnEnterIdle(nWhy, pWho);
}


void S63WorkFlowView::OnRemoveS57Stores()
{
  if (m_s63Layer)
  {
    m_s63Layer->removeData();
    m_s63Layer->notifyChanged();
    InvalidateRect( 0, false ) ;
  }
}


void S63WorkFlowView::noDataLayerVisible(bool toggle)
{
	S63WorkFlowDoc * doc = GetDocument() ;

	if (m_drawingSurface)
		m_drawingSurface->setDataLayerProps("s52nodatalayer", TSLPropertyVisible, (TSLPropertyValue)toggle);
}

void S63WorkFlowView::setupDynamicColours(string file)
{
	S63WorkFlowDoc * doc = GetDocument() ;
	if (m_drawingSurface)
		m_drawingSurface->setupDynamicColours(file.c_str());
}

bool S63WorkFlowView::error(TSLErrorCategory category, int errorCode, const char* errorNote, const char* additionalInformation)
{
  std::stringstream str;
  const char* cat(NULL);
  switch(category)
  {
  case TSLErrorCategoryDebug:
    cat = "Debug";
    break;
  case TSLErrorCategoryError:
    cat = "Error";
    break;
  case TSLErrorCategoryFatal:
    cat = "Fatal";
    break;
  case TSLErrorCategoryInfo:
    cat = "Info";
    break;
  case TSLErrorCategorySyntax:
    cat = "Syntax";
    break;
  case TSLErrorCategoryWarn:
    cat = "Warning";
    break;
  default:
    break;
  }
  str << "MapLink Error(";
  if( cat )
  {
    str << cat;
  }
  else
  {
    str << "Category " << category;
  }
  str << "): Code " << errorCode << ": " << errorNote << ": " << additionalInformation;

  // Copy the string - Will be deleted by the message handler
  string* message = new string( str.str() );
  ::PostMessage( m_hWnd, WM_LOG_MESSAGE, 0, LPARAM(message));

  return true;
}