/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "addwmsservicepage.h"
#include "servicewizardpageenum.h"
#include <QMovie>
#include <QMessageBox>
#include <QSettings>

#include "services/service.h"
#include "MapLink.h"

AddWMSServicePage::AddWMSServicePage(QWidget *parent)
  : QWizardPage(parent)
  , m_loadingMovie( new QMovie(":/animations/images/splash_loader.gif") )
  , m_loadInProgress( false )
  , m_serviceLoaded( false )
  , m_service( NULL )
  , m_editInProgress( false )
{
  setupUi(this);
  connect(serviceAddress, SIGNAL(editTextChanged(const QString&)), this, SLOT(serviceURLChanged(const QString&)));
  connect(serviceAddress, SIGNAL(currentIndexChanged(int)), this, SLOT(serviceURLSelectionChanged(int)));
  connect(this, SIGNAL(signalShowError(const QString&)), this, SLOT(showError(const QString&)));
  connect(this, SIGNAL(signalServiceConnectionEstablished()), this, SLOT(serviceConnectionEstablished()));
  connect(wmsInitialVersionRequest, SIGNAL(activated(int)), this, SLOT(initialWMSVersionChanged(int)));
  connect(swapXYAxis, SIGNAL(stateChanged(int)), this, SLOT(axisInversionChanged(int)) );

  // Don't show the loading animation on startup, wait for the user to try to connect
  loadingAnimationContainer->setVisible(false);

  loadingAnimation->setMovie(m_loadingMovie);
}

AddWMSServicePage::~AddWMSServicePage()
{
  delete m_loadingMovie;
}

void AddWMSServicePage::initializePage()
{
  wmsInitialVersionRequest->addItem( "1.3.0", QVariant( TSLWMSVersion_1_3_0 ) );
  wmsInitialVersionRequest->addItem( "1.1.1", QVariant( TSLWMSVersion_1_1_1 ) );
  wmsInitialVersionRequest->addItem( "1.1.0", QVariant( TSLWMSVersion_1_1_0 ) );

  // Populate the URL dropdown with any services that have previously been used
  serviceAddress->clear();
  QSettings settings;
  int numPreviousEntries = settings.beginReadArray( "wmsservices" );
  for( int i = 0; i < numPreviousEntries; ++i )
  {
    settings.setArrayIndex( i );

    QString serviceName( settings.value( "name" ).toString() );
    QString serviceURL( settings.value( "url" ).toString() );

    serviceAddress->addItem( QString( serviceName + " (" + serviceURL + ")" ), serviceURL );
  }
  settings.endArray();

  // Reset the page status to the default in case this is due to the user
  // navigating backwards and forwards through the wizard
  m_loadInProgress = false;
  m_serviceLoaded = false;

  loadingAnimationContainer->setVisible(false);
  m_loadingMovie->stop();

  // Ensure we recieve any callbacks from the service object so we can update the page accordingly
  m_service->pushCallbackObject( this );
}

void AddWMSServicePage::cleanupPage()
{
  m_loadInProgress = false;

  // Tell the service loading thread to cancel the current connection sequence as navigating back
  // from this page requires starting again.
  m_service->cancelSequence();

  loadingAnimationContainer->setVisible(false);
  m_loadingMovie->stop();

  completeChanged();

  m_service->popCallbackObject();

  QWizardPage::cleanupPage();
}

bool AddWMSServicePage::isComplete() const
{
  return !serviceAddress->currentText().isEmpty() && !m_loadInProgress;
}

int AddWMSServicePage::nextId() const
{
  return SelectLayers;
}

void AddWMSServicePage::serviceURLChanged( const QString &/*url*/ )
{
  m_editInProgress = true;

  // Changing the service URL means that the service must always be reloaded
  m_serviceLoaded = false;
  completeChanged();
}

void AddWMSServicePage::serviceURLSelectionChanged( int /*index*/ )
{
  m_editInProgress = false;

  // Changing the service URL means that the service must always be reloaded
  m_serviceLoaded = false;
  completeChanged();
}

void AddWMSServicePage::initialWMSVersionChanged( int index )
{
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->setInitialRequestWMSVersion( (TSLWMSVersionEnum)wmsInitialVersionRequest->itemData( index ).toInt() );
}

void AddWMSServicePage::axisInversionChanged( int state )
{
  TSLWMSDataLayer *wmsLayer = reinterpret_cast< TSLWMSDataLayer* >( m_service->dataLayer() );
  wmsLayer->invertXYAxis( state == Qt::Checked );
}

bool AddWMSServicePage::validatePage()
{
  if( m_serviceLoaded )
  {
    // If we have already loaded a service then the wizard can advance to the next page

    // Clear the loaded flag so that the page will work if the user navigates back to it from the next page
    m_serviceLoaded = false;

    return true;
  }
  else if( !m_loadInProgress )
  {
    // No service has been loaded yet (or the service to load has changed).
    // Show the frame containing the 'please wait' animation and start playback
    // so the user can see the application is doing something
    loadingAnimationContainer->setVisible(true);
    m_loadingMovie->start();

    // Signal that the completed status has changed so that the next button is disabled while the service loads in
    // another thread
    m_loadInProgress = true;
    completeChanged();

    QByteArray utf8URL;
    QVariant currentItemData = serviceAddress->itemData( serviceAddress->currentIndex() );
    if( !m_editInProgress && currentItemData.isValid() )
    {
      // The item data contains the URL string to use
      utf8URL = currentItemData.toString().toUtf8();
    }
    else
    {
      utf8URL = serviceAddress->currentText().toUtf8();
    }

    m_service->loadService( utf8URL.constData() );

    return false;
  }

  return false;
}

void AddWMSServicePage::onError( const std::string &message )
{
  // Send a signal to perform the necessary UI updates in the correct thread
  emit signalShowError( QString::fromUtf8( message.c_str() ) );
}

void AddWMSServicePage::onNextSequenceAction()
{
  emit signalServiceConnectionEstablished();
}

void AddWMSServicePage::showError( const QString& message )
{
  // Reset the state of the page so that the user can try a different URL
  m_loadInProgress = false;
  m_loadingMovie->stop();
  loadingAnimationContainer->setVisible(false);

  // Show the error message after hiding the connecting animation
  QMessageBox::critical( this, tr( "Error Connecting to Service" ), message );

  // Reactivate the next button
  completeChanged();
}

void AddWMSServicePage::serviceConnectionEstablished()
{
  // Turn off the loading animation since we've now loaded the service
  m_loadingMovie->stop();
  loadingAnimationContainer->setVisible(false);

  m_serviceLoaded = true;
  m_loadInProgress = false;

  // Signal the wizard to advance to the next page
  wizard()->next();
}
