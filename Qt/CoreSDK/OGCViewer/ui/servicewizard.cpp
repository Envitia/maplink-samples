/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include "servicewizard.h"
#include "services/servicelist.h"

#include <QSettings>
#include "credentialsdialog.h"

using namespace Services;

ServiceWizard::ServiceWizard( ServiceList *serviceList, TSLDataLayer *coordinateProvidingLayer, QWidget *parent, Qt::WindowFlags flags)
  : QWizard( parent, flags )
  , m_connectedService( NULL )
  , m_serviceList( serviceList )
  , m_coordinateProvidingLayer( coordinateProvidingLayer )
{
  setupUi(this);
  setStartId(ActionType);

  m_serviceList->setCredentialsCallback( this );

  
  connect(this, SIGNAL(signalShowCredentialsDialog( QString*, QString* )), 
          this, SLOT(showCredentialsDialog( QString*, QString* )));

  // Turn off the help button as additional help is displayed via tooltips
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

ServiceWizard::~ServiceWizard()
{
  m_serviceList->setCredentialsCallback( NULL );
  delete m_connectedService;
}

void ServiceWizard::setService( Service *newService )
{
  if( m_connectedService )
  {
    delete m_connectedService;
  }

  m_connectedService = newService;
  newService->useTransformParametersFromLayer( m_coordinateProvidingLayer );

  // Tell the service's data layer to use the common remote loader we share between all data layers
  TSLDataLayer *serviceLayer = m_connectedService->dataLayer();
  serviceLayer->addLoader( m_serviceList->getCommonLoader(), ServiceList::loadCallback, m_serviceList,
                           ServiceList::allLoadedCallback, m_serviceList );

  // Inform the subsequent pages of the wizard of the service option they need to
  // use when configuring the service setup
  selectLayersPage->setService( newService );
  selectCoordSysPage->setService( newService );
  dimensionsPage->setService( newService );

  // These pages are specific to one type of service
  switch( newService->type() )
  {
  case ServiceTypeWMS:
    addWMSServicePage->setService( (WMSService*)newService );
    wmsServiceOptionsPage->setService( (WMSService*)newService );
    addWMTSServicePage->setService( NULL );
    wmtsServiceOptionsPage->setService( NULL );
    break;

  case ServiceTypeWMTS:
    addWMTSServicePage->setService( (WMTSService*)newService );
    wmtsServiceOptionsPage->setService( (WMTSService*)newService );
    addWMSServicePage->setService( NULL );
    wmsServiceOptionsPage->setService( NULL );
    break;

  default:
    break;
  }
}

void ServiceWizard::accept()
{
  // The user finished the wizard - add the new service they configured to the main service list
  // so it will show up in the application
  m_serviceList->addService( m_connectedService );

  recordLoadedServices();

  // The service list takes ownership of the service
  m_connectedService = NULL;

  QWizard::accept();
}

void ServiceWizard::reject()
{
  // The user cancelled the wizard - tell the current service to abort any loading it is doing in the loading thread
  if( m_connectedService )
  {
    m_connectedService->cancelSequence();
  }

  // Even if the wizard was rejected, record the service for future use if it was successfully connected to
  recordLoadedServices();

  QWizard::reject();
}

void ServiceWizard::recordLoadedServices()
{
  if( m_connectedService )
  {
    QString serviceTypePrefix = m_connectedService->type() == ServiceTypeWMS ? "wmsservices" : "wmtsservices";

    const char *serviceDisplayName = m_connectedService->getServiceDisplayName();
    const std::string &serviceURL = m_connectedService->getServiceURL();
    if( serviceDisplayName && !serviceURL.empty() )
    {
      // Add this service to the recorded list of previously used services.
      // Make sure we don't add it more than once by making the set of service URLs unique
      std::set< std::pair< QString, QString > > serviceURLs;

      QSettings settings;
      int numPreviousEntries = settings.beginReadArray( serviceTypePrefix );
      for( int i = 0; i < numPreviousEntries; ++i )
      {
        settings.setArrayIndex( i );
        serviceURLs.insert( std::make_pair( settings.value( "name" ).toString(),
                                            settings.value( "url" ).toString() ) );
      }
      settings.endArray();

      serviceURLs.insert( std::make_pair( serviceDisplayName, QString::fromUtf8( serviceURL.c_str() ) ) );

      // Now write the list back out
      settings.beginWriteArray( serviceTypePrefix );
      std::set< std::pair< QString, QString > >::iterator serviceIt( serviceURLs.begin() );
      std::set< std::pair< QString, QString > >::iterator serviceItE( serviceURLs.end() );
      for( int index = 0; serviceIt != serviceItE; ++serviceIt, ++index )
      {
        settings.setArrayIndex( index );
        settings.setValue( "name", serviceIt->first );
        settings.setValue( "url", serviceIt->second );
      }
      settings.endArray();

      // Ensure the values are written out so that they're available immediately if the wizard is started again
      settings.sync();
    }
  }
}

void ServiceWizard::onCredentialsRequired (TSLSimpleString& username, TSLSimpleString& password)
{
  QString QUsername;
  QString QPassword;
  m_mutex.lock();
  emit signalShowCredentialsDialog( &QUsername, &QPassword );
  m_waitCond.wait(&m_mutex);
  m_mutex.unlock();
  username = QUsername.toUtf8();
  password = QPassword.toUtf8();
}

void ServiceWizard::showCredentialsDialog( QString* username, QString* password )
{
  CredentialsDialog dialog( this );
  if( dialog.exec() == QDialog::Accepted )
  {
    *username = dialog.lineEditUsername->text();
    *password = dialog.lineEditPassword->text();
  }
  m_mutex.lock();
  m_waitCond.wakeAll();
  m_mutex.unlock();
}
