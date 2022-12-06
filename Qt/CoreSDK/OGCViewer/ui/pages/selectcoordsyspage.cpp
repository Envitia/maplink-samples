/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "selectcoordsyspage.h"
#include "servicewizardpageenum.h"
#include <QMessageBox>

#include "MapLink.h"
#include "tslplatformhelper.h"

SelectCoordSysPage::SelectCoordSysPage(QWidget *parent)
  : QWizardPage(parent)
  , m_service( NULL )
  , m_loadInProgress( false )
{
  setupUi(this);

  setCommitPage( true );

  connect(this, SIGNAL(signalShowServiceOptionsPage()), this, SLOT(showServiceOptionsPage()) );
  connect(this, SIGNAL(signalShowError(const QString&)), this, SLOT(showError(const QString&)));
}

SelectCoordSysPage::~SelectCoordSysPage()
{
}

void SelectCoordSysPage::initializePage()
{
  // Ensure we recieve any callbacks from the service object so we can update the page accordingly
  m_service->pushCallbackObject( this );

  // Populate the combobox with the choice of coordinate systems the user has based on their previous selections
  // The combobox is sorted alphabetically, so store the index in the choice list along with each item
  // so that we can easily tell the callback thread which coordinate system was selected.
  unsigned int numCoordinateSystems = m_service->numCoordSystemChoices();
  const char **coordinateSystems = m_service->coodinateSystemChoices();
  for( unsigned int i = 0; i < numCoordinateSystems; ++i )
  {
    uint32_t epsgID = 0;

    // Coordinate system strings may or may not be fully qualified, this code performs some very basic parsing to try
    // and extract the EPSG identifier from these
    const char *epsgMarker = TSLPlatformHelper::strcasestr( coordinateSystems[i], "EPSG:" );
    if( epsgMarker )
    {
      const char *lastSeperator = strrchr( epsgMarker + 5, ':' );
      if( lastSeperator )
      {
        sscanf( lastSeperator + 1, "%u", &epsgID );
      }
      else
      {
        // Try to parse it as an unqualified string (e.g. 'EPSG:num')
        sscanf( coordinateSystems[i], "%*4s:%u", &epsgID );
      }
    }

    QString displayName;
    if( epsgID != 0 )
    {
      // Use the Transverse Mercator Snyder formula instead of JHS formula as the Snyder
      // formula is predominant.
      const TSLCoordinateSystem *coordSys = TSLCoordinateSystem::findByEPSG(epsgID);
      if( coordSys && coordSys->name() )
      {
        displayName = QString( "%1 (%2)" ).arg( QString::fromUtf8( coordSys->name() ) ).arg( QString::fromUtf8( coordinateSystems[i] ) );
      }
    }

    if( displayName.isEmpty() )
    {
      // If there is no coordinate system name, just list the base identifier
      displayName = QString::fromUtf8( coordinateSystems[i] );
    }

    coordSysList->addItem( displayName, QVariant( i ) );
  }

  coordSysList->model()->sort(0);
}

void SelectCoordSysPage::cleanupPage()
{
  m_service->popCallbackObject();

  QWizardPage::cleanupPage();
}

bool SelectCoordSysPage::validatePage()
{
  if( !m_loadInProgress )
  {
    m_loadInProgress = true;

    QVariant selectedCoordSys( coordSysList->itemData( coordSysList->currentIndex() ) );
    m_service->setCoordinateSystemChoice( selectedCoordSys.toInt() );
    m_service->advanceConnectionSequence();

    // Disable the next button, the wizard will advance programmatically in the callback from the
    // service loader thread
    emit completeChanged();

    return false;
  }

  return true;
}

bool SelectCoordSysPage::isComplete() const
{
  return !m_loadInProgress;
}

int SelectCoordSysPage::nextId() const
{
  switch( m_service->type() )
  {
  case ServiceTypeWMS:
    return WMSServiceOptions;

  case ServiceTypeWMTS:
    return WMTSServiceOptions;

  default:
    return -1;
  }
}

void SelectCoordSysPage::showServiceOptionsPage()
{
  // Signal the wizard to advance to the next page
  wizard()->next();
}

void SelectCoordSysPage::showError( const QString& message )
{
  // Reset the state of the page so that the user can try a different URL
  m_loadInProgress = false;

  // Show the error message after hiding the connecting animation
  QMessageBox::critical( this, tr( "Error Choosing Coordinate System" ), message );
}

void SelectCoordSysPage::onError( const std::string &message)
{
  // Send a signal to perform the necessary UI updates in the correct thread
  emit signalShowError( QString::fromUtf8( message.c_str() ) );
}

void SelectCoordSysPage::onNextSequenceAction()
{
  emit signalShowServiceOptionsPage();
}
