//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
#include "../stdafx.h"
#include "s63workflow.h"
#include "../S52/s52stateobject.h"
#include "../s63workflowview.h" // only used for setting map cache size.

#include <Shlobj.h>

#include <sstream>


S63Workflow::S63Workflow(const LocateCatalogueCallback catalogueCallback, const NotifyErrorCallback notifyErrorCallback,
                         const IngestCellCallback ingestCellCallback, const CancelledCellCallback cancelledCellCallback,
                         const ProgressCallback progressCallback, void *arg, S52StateObject *s52StateObject)
: m_manager(NULL)
, m_catalogueCallback(catalogueCallback)
, m_notifyErrorCallback(notifyErrorCallback)
, m_ingestCellCallback(ingestCellCallback)
, m_cancelledCellCallback(cancelledCellCallback)
, m_progressCallback(progressCallback)
, m_isUnlocked(false)
, m_hw_id(NULL)
, m_callbackArgument( arg )
, m_userPermit()
, m_ihoCertificate()
, m_dataServerCertificate()
, m_cellPermitStore()
, m_cellPermitCollections()
, m_currentTime()
, m_scaleBands()
, m_horizontalDPI(0)
, m_verticalDPI(0)
, m_drawingSurface(NULL)
, m_displayIngestDialog(false)
, m_userScaleBands()
, m_s63DataClients()
, m_s57DataClients()
, m_s57ConfigFile()
, m_s52StateObject(s52StateObject)
{
  // Set the current time, this can be changed via the workflow configuration dialog.
  // Defaults to the current time.
  __time64_t ltime;
  _time64( &ltime );
  
  m_currentTime = ltime;

  // Clear the permit store.
  m_cellPermitStore.clear();

  // Create a default hardware ID.
  // This defaults to '12345' as per section 5.1 of the S-63 Test Data Implementation Guide
  m_hw_id = new TSLS63HWID("12345");

  // Set up the defaults for the scale bands, as defined in the S63 standard.
  m_scaleBands = new TSLS63DataLayerScaleBands();
  m_userScaleBands = new TSLS63DataLayerScaleBands();

  // Set the default scale band values.
  configureScaleBands();
  if (!m_iniFileLocation.empty())
  {
    m_s52StateObject->readConfiguration(m_iniFileLocation.c_str());
  }
}


S63Workflow::~S63Workflow()
{
  // Cleanup
  if ( m_manager )
  {
    m_manager->destroy();
    m_manager = NULL;
  }
  
  if ( m_hw_id )
  {
    delete m_hw_id;
  }

  // Clear up cell permits.
  vector< TSLS63CellPermitCollection* >::iterator it;
  for ( it = m_cellPermitCollections.begin(); it != m_cellPermitCollections.end(); ++it )
  {
    (*it)->destroy();
  }
  
  vector< S63WorkflowClient* >::iterator s63It;
  for ( s63It = m_s63DataClients.begin(); s63It != m_s63DataClients.end(); ++s63It )
  {
    delete (*s63It);
  }

  vector< S57WorkflowClient* >::iterator s57It;
  for ( s57It = m_s57DataClients.begin(); s57It != m_s57DataClients.end(); ++s57It )
  {
    delete (*s57It);
  }
  
  // Clean up the real scale bands
  if ( m_scaleBands ) 
  {
    m_scaleBands->destroy();
    m_scaleBands = NULL;
  }

  // And the user representation of the scale bands.
  if ( m_userScaleBands ) 
  {
    m_userScaleBands->destroy();
    m_userScaleBands = NULL;
  }

  m_callbackArgument = NULL;

}
void S63Workflow::setDPI(const int vdpi, const int hdpi)
{
  m_verticalDPI = vdpi;
  m_horizontalDPI = hdpi;
}

void S63Workflow::getDPI(int& vdpi, int& hdpi) const
{
  vdpi = m_verticalDPI;
  hdpi = m_horizontalDPI;
}

bool S63Workflow::setScaleBand(const TSLS63ScaleBandEnum type, const double lowerThreshold, const double upperThreshold, const bool updateUserScaleBands)
{
  /*
  IMPORTANT NOTE ON SCALE BANDING
  -------------------------------
  Scale banding has been provided in this sample for example purposes only
  and is not guaranteed to be accurate.

  Scale banding suffers from the following limitations:

    - The accuracy of the Scale Banding is affected by the projection of 
      the coordinate system in use.

    - Scale banding depends on the DPI ( Dots Per Inch ) of the monitor in use.
      The queried value is rarely 100% accurate and will therefore also
      affect the scale bands.

  With dynamic arc projections (such as the one used in the sample) the scale band
  does not directly correspond to the set scale because the X-Axis values are scaled
  based upon the view position north and south of the equator.

  */

  if ( lowerThreshold < 1 || upperThreshold < 1  )
  {
    return false;
  }

  // Firstly, update the user scale bands. This is the 1:XXX representation, not the MU per Pixel representation.
  if (updateUserScaleBands)
    m_userScaleBands->addBand(type, lowerThreshold, upperThreshold);

  // Ensure we have a drawing surface to use prior to setting the 'real' scale band value.
  if ( !m_drawingSurface ) 
  {
    return false;
  }

  // Get the DPI of the monitor in use.
  double tmcPerMU = m_drawingSurface->TMCperMU();

  // convert the dpi to dots per metre.
  const double dotsPerMetre = 100 * m_horizontalDPI / CM_PER_INCH;

  double lowerTMCperDU = (1 / (lowerThreshold / dotsPerMetre)) * tmcPerMU;
  double upperTMCperDU = (1 / (upperThreshold / dotsPerMetre)) * tmcPerMU;

  // Add the band to the 'real' scale band collection. Note that adding the band
  // at this point will cause the internals to remove the existing band prior to adding this
  // one.
  m_scaleBands->addBand(type, tmcPerMU / lowerTMCperDU, tmcPerMU / upperTMCperDU);

  return true;
}

void S63Workflow::removeDataServerCertificate()
{
  // Clear the registered data server certificate.
  m_dataServerCertificate.clear();
}

void S63Workflow::removeIHOCertificate()
{
  // Clear the IHO certificate
  m_ihoCertificate.clear();
}

bool S63Workflow::installCertificate(const string &certificatePath, const bool dataServerCertificate)
{
  TSLSimpleString certificateIssuer;
  TSLTimeType certificateValidFromDate;
  TSLTimeType certificateExpiryDate;

  // First, ensure the specified file points to a valid certificate
  // and extract the expiry times and certificate issuer
  if ( !queryCertificate(certificatePath, certificateValidFromDate, certificateExpiryDate, certificateIssuer) )
  {
    return false;
  }

  // Using the current time, and those queried from the certificate, ensure
  // that the certificate is 'in date'.
  if ( !TSLS63MediaIngestManager::validateCertificateTimes( m_currentTime, certificateValidFromDate, certificateExpiryDate ) )
  {
    return false;
  }

  // Once we've validated the certificate, update the appropriate stored certificates.
  if ( dataServerCertificate )
  {
    m_dataServerCertificate = certificatePath;
  }
  else
  {
    m_ihoCertificate = certificatePath;
  }

  return true;
}

bool S63Workflow::queryCertificate(const string &certificatePath, TSLTimeType& certificateValidFromDate, TSLTimeType& certificateExpiryDate, TSLSimpleString& certificateIssuer) const
{  
  // Ensure the specified certificate is valid.
  return TSLS63MediaIngestManager::queryCertificate( certificatePath.c_str(), certificateValidFromDate, certificateExpiryDate, &certificateIssuer );
}

bool S63Workflow::generateUserPermit(const string &hw_id, const string &m_key, const string &m_id, string &result) const
{
  // Ensure the provided values aren't empty
  if ( hw_id.empty() || m_key.empty() || m_id.empty() ) 
  {
    return false;
  }

  // Create a new hardware ID based around the provided string representation
  const TSLS63HWID hardwareID(hw_id.c_str());
  TSLSimpleString generatedPermit("");

  ////////////////////////////////////////////////
  ////      TEST PLAN REFERENCE: 3.4.2.2      ////
  ////////////////////////////////////////////////

  // Generate the user permit with the provided parameters. 
  TSLS63UserPermitGeneratorResult error = TSLS63UserPermitGenerator::generateUserPermit(&hardwareID, m_key.c_str(), m_id.c_str(), generatedPermit);

  // If a user permit was generated
  if ( !generatedPermit.empty() ) 
  {
    // Populate the result variable
    result = string(generatedPermit.c_str());
  }
  
  // And return whether or not it was a success.
  return (error == TSLS63UserPermitGeneratorResultSuccess);
}

bool S63Workflow::installUserPermit(const string &permitPath)
{
  /////////////////////////////////////
  // Test Plan Reference: 3.4.2.2
  //  - Provide a means of 'installing' user permits.
  /////////////////////////////////////

  // The user permit is not used at this point, so merely store the path to the file
  m_userPermit = permitPath;
  return true;
}

bool S63Workflow::installCellPermits(const string &permitPath, const string &dataServerID)
{
  return generateCellPermits(permitPath, dataServerID, m_cellPermitStore);
}

void S63Workflow::removeCellPermit(const string &cellName, const string &dataServerID)
{
  vector<pair<const TSLS63CellPermit*, string> >::iterator it;
 
  // Iterate through all the cell permits in the store
  for ( it = m_cellPermitStore.begin() ; it != m_cellPermitStore.end() ; ++it ) 
  {
    if (!it->first)
    {
      continue;
    }

    // The cell name and dataServerID should uniquely identify a permit,
    // so loop through until we find ther permit which matches.
    if ( cellName == it->first->cellName() && it->second == dataServerID ) 
    {
      m_cellPermitStore.erase(it);
      return;
    }
  }
}  
    
bool S63Workflow::getMediaRecords(const string &mediaFilePath, vector<string>& mediaRecords) const
{
  // Create a new S63 media file using the file provided by the user.
  const TSLS63MediaFile * mediaFile = TSLS63MediaFile::create(mediaFilePath.c_str());

  // Ensure the file exists and that it contains records.
  if ( !mediaFile || mediaFile->size() < 1 )
  {
    return false;
  }

  const int mediaSize = mediaFile->size();

  // Loop through the media file and extract all information about the 
  // media records.
  for ( int i = 0 ; i < mediaSize ; ++i ) 
  {
    const TSLS63MediaRecord * mediaRecord = (*mediaFile)[i];

    if ( !mediaRecord )
    {
      continue;
    }

    // Generate a string representation of the media record 
    string mediaPath(mediaRecord->mediaLocation());
    mediaPath += " - ";
    mediaPath += mediaRecord->mediaSetNumber();

    // Add the formatted string to the list of media records.
    mediaRecords.push_back(mediaPath);
  }
  
  return true;
}

bool S63Workflow::startIngest(const string &dataStore, const string &productFile, const string &serialFile)
{
  // First ensure we have everything we will need for the media ingest.
  if ( !m_hw_id || m_ihoCertificate.empty() || dataStore.empty() || 
       productFile.empty() || serialFile.empty() || m_cellPermitStore.empty() )
  {
    return false;
  }

  // Create a new work flow client to manage the media ingest.
  // Provide the client with the path to where the data is to be stored
  // and also provide the current time.
  S63WorkflowClient * workflowClient = new S63WorkflowClient(dataStore, m_currentTime, m_drawingSurface);
  
  if ( !workflowClient )
  {
    return false;
  }

  // Set up the path to the SERIAL.ENC file.
  workflowClient->setSerialPath(serialFile);

  // Flag whether or not to display the ingest cell dialog.
  workflowClient->displayIngestDialog(m_displayIngestDialog);

  // Set up the GUI callbacks. 
  workflowClient->setCatalogueCallback(m_catalogueCallback);
  workflowClient->addErrorCallback(m_notifyErrorCallback, m_callbackArgument);
  workflowClient->setIngestCallback(m_ingestCellCallback);
  workflowClient->setCancelledCellCallback(m_cancelledCellCallback);
  workflowClient->setProgressCallback(m_progressCallback);
  workflowClient->setUserCallbackArgument(m_callbackArgument);

  // Create the MediaIngestManager, providing it with the recently constructed 
  // data client, the IHO Certificate to use and a valid hardware ID.
  // The recommended oldest version for the S-63 SDK as a save format is MapLink 8.0.
  m_manager = new TSLS63MediaIngestManager(workflowClient, m_ihoCertificate.c_str(), m_hw_id, TSL_CURRENT_MAPLINK_VERSION);

  if ( !m_manager )
  {
    delete workflowClient;
    return false;
  }
  // Turn off any heavy processing - accept geometry as is.
  // Only do this is performance is very critical - test as this will cause depth areas to not be displayed as expected.
  //m_manager->setProcessingFlags(TSLFilterProcessNone);

  vector<pair<const TSLS63CellPermit*, string> >::const_iterator it(m_cellPermitStore.begin()), itEnd(m_cellPermitStore.end());

  // Iterate over the stored cell permits and add them to the ingest manager.
  while ( it != itEnd )
  {
    if ( !(it->first) )
    {
      continue;
    }
   
    string dataServerID = it->second;

    if ( !dataServerID.empty() )
    {
      // If a data server has been provided, add it to the manager
      m_manager->addCellPermit(it->first, dataServerID.c_str()); 
    }
    else
    {
      // If a data server has not been provided, leave it as NULL.
      m_manager->addCellPermit(it->first); 
    }

    ++it;
  }

  // Create the SERIAL.ENC file.
  TSLS63SerialENCFile * serialENCFile = TSLS63SerialENCFile::create( serialFile.c_str() ) ;
  
  if ( !serialENCFile ) 
  {
    delete workflowClient;
    return false;
  }

  // Finally, once everything has been setup, load the exchange set.
  // This will then begin the media ingest phase and trigger regular callbacks in the
  // S63WorkflowClient class.

  bool result(false);

  // If we have a data server certificate installed, we use that one in preference to the SA certificate
  if ( !m_dataServerCertificate.empty() )
  {
     result = m_manager->loadExchangeSet(productFile.c_str(), serialENCFile, m_dataServerCertificate.c_str(), s57ConfigFile().c_str());
  }
  else
  {
     result = m_manager->loadExchangeSet(productFile.c_str(), serialENCFile, m_ihoCertificate.c_str(), s57ConfigFile().c_str());
  }

  delete workflowClient;
  serialENCFile->destroy();

  return result;
}

bool S63Workflow::startS57Ingest(const string &dataStore, const string &s57File)
{
  // First ensure we have everything we will need for the media ingest.
  if ( dataStore.empty() || s57File.empty() )
  {
    return false;
  }

  // Create a new work flow client to manage the media ingest.
  // Provide the client with the path to where the data is to be stored
  // and also provide the current time.
  S57WorkflowClient * workflowClient = new S57WorkflowClient(dataStore, m_currentTime, m_drawingSurface );
  
  if ( !workflowClient )
  {
    return false;
  }
  
  // Flag whether or not to display the ingest cell dialog.
  workflowClient->displayIngestDialog(m_displayIngestDialog);

  // Set up the GUI callbacks. 
  workflowClient->addErrorCallback(m_notifyErrorCallback, m_callbackArgument);
  workflowClient->setIngestCallback(m_ingestCellCallback);
  workflowClient->setCancelledCellCallback(m_cancelledCellCallback);
  workflowClient->setProgressCallback(m_progressCallback);
  workflowClient->setUserCallbackArgument(m_callbackArgument);

  // Create the MediaIngestManager, providing it with the recently constructed 
  // data client, the IHO Certificate to use and a valid hardware ID.
  // The recommended oldest version for the S-63 SDK as a save format is MapLink 8.0.
  TSLS57MediaIngestManager * manager = new TSLS57MediaIngestManager(workflowClient, TSL_CURRENT_MAPLINK_VERSION);

  if ( !manager )
  {
    delete workflowClient;
    return false;
  }
  // Turn off any heavy processing - accept geometry as is.
  //manager->setProcessingFlags(TSLFilterProcessNone);

  // Finally, once everything has been setup, load the exchange set.
  // This will then begin the media ingest phase and trigger regular callbacks in the
  // S63WorkflowClient class.
  
  TSLSimpleString directory;
  TSLSimpleString fileName;
  TSLSimpleString fileExtension;
  TSLFileHelper::decomposePathname(s57File.c_str(), directory, fileName, fileExtension);
  
  bool result(false);
  int numberOfErrorsAtStart = TSLThreadedErrorStack::size(TSLErrorCategoryError | TSLErrorCategoryFatal);

  if ( fileName == "CATALOG" )
  {
    result = manager->loadCatalog(s57File.c_str(), s57ConfigFile().c_str());
  }
  else
  {
    result = manager->loadCell(s57File.c_str(), s57ConfigFile().c_str());
  }
  delete workflowClient;
  manager->destroy();

  int numberOfErrorsAtEnd= TSLThreadedErrorStack::size(TSLErrorCategoryError | TSLErrorCategoryFatal);

  if (numberOfErrorsAtEnd != numberOfErrorsAtStart && result)
  {
    // potentially an issue.
    result = false;
  }

  return result;
}

bool S63Workflow::addS63DataSource(TSLS63DataLayer * s63Layer, const string& dataStoreID, const string& dataStore, vector<pair<const TSLS63CellPermit*, string> >& cellPermits, S63ErrorLayer *& s63ErrorLayer)
{  
  if ( !s63Layer )
  {
    return false;
  }

  S63WorkflowClient * dataLayerClient = new S63WorkflowClient(dataStore, m_currentTime, m_drawingSurface);  
  dataLayerClient->addErrorCallback(m_notifyErrorCallback, m_callbackArgument);  

  // Retrieve the list of cell extents from the client
  map<pair<string, string>, TSLGeodeticExtent> cellExtents = dataLayerClient->cellExtents();

  // Create the error layer, based on the cell extents.
  s63ErrorLayer = new S63ErrorLayer(cellExtents, getDefaultCoordinateSystem());

  // And add an error callback to notify the layer.
  dataLayerClient->addErrorCallback(S63ErrorLayer::notifyErrorCallback, s63ErrorLayer);  
  
  s63Layer->addDataClient(dataStoreID.c_str(), dataLayerClient);
  m_s63DataClients.push_back(dataLayerClient);

  // Iterate over all the provided cell permits and add each to the data layer.
  vector<pair<const TSLS63CellPermit*, string> >::const_iterator it;
  bool result = true;
  for ( it = cellPermits.begin(); it != cellPermits.end(); ++it ) 
  {
    // For each permit extract the name of the cell.
    const TSLS63CellPermit * permit = it->first;

    if ( !permit )
    {
      continue;
    }

    string cellName(permit->cellName());

    map<pair<string, string>, TSLGeodeticExtent>::const_iterator cellIterator;

    //Attempt to find the <dataServer, cellName> key in the cell extents map.
    cellIterator = cellExtents.find( make_pair( cellName, it->second ) );
  
    if ( cellIterator != cellExtents.end() )
    {
      // Once we find the correct cell, add the cell permit and
      // extent to the S63 data layer.
      if( !s63Layer->addCellPermit(dataStore.c_str(), permit, cellIterator->second, it->second.c_str()) )
      {
        // Failed to add the permit to the data layer
        result = false;
        std::ostringstream errorMsg;
        errorMsg << "Unable to add the cell permit to the layer for cell " << cellName;
        TSLErrorStack::addError(99999, errorMsg.str().c_str());
      }
    }
  }
  return result;
}

bool S63Workflow::addS57DataSource(TSLS63DataLayer * s63Layer, const string& dataStoreID, const string& dataStore, S63ErrorLayer *& s63ErrorLayer)
{  
  if ( !s63Layer )
  {
    return false;
  }

  S57WorkflowClient * dataLayerClient = new S57WorkflowClient(dataStore, m_currentTime, m_drawingSurface); 
  dataLayerClient->addErrorCallback(m_notifyErrorCallback, m_callbackArgument);  

  // Retrieve the list of cell extents from the client
  map<pair<string, string>, TSLGeodeticExtent> cellExtents = dataLayerClient->cellExtents();

  // Create the error layer, based on the cell extents.
  s63ErrorLayer = new S63ErrorLayer(cellExtents, getDefaultCoordinateSystem());

  // And add an error callback to notify the layer.
  dataLayerClient->addErrorCallback(S63ErrorLayer::notifyErrorCallback, s63ErrorLayer);  

  s63Layer->addDataClient(dataStoreID.c_str(), dataLayerClient);

  m_s57DataClients.push_back(dataLayerClient);

  return true;
}

void S63Workflow::generateDataLayer(const string &hw_id, const long cacheSize, TSLS63DataLayer *& s63Layer)
{
  // Set the hardware ID.
  TSLS63HWID * hardwareID = new TSLS63HWID(hw_id.c_str());
  
  TSLCoordinateSystem * coordSys = getDefaultCoordinateSystem();

  // Create the S63 Data Layer, providing all the above as parameters.
  s63Layer = new TSLS63DataLayer(hardwareID, coordSys, m_scaleBands);
  s63Layer->createDataHandler();

  //S63 Layer clones the coordSys, so we need to tidy it up.
  if (coordSys)
    coordSys->destroy();

  // Cache size at this point is in mb, so we up it to kb.
  long scaledCacheSize = cacheSize * 1024;
  s63Layer->cacheSize(scaledCacheSize);

  if ( hardwareID )
  {
    delete hardwareID;
    hardwareID = NULL;
  }
}

TSLCoordinateSystem * S63Workflow::getDefaultCoordinateSystem() const
{
  // Set up the same coordinate system which was used to ingest the media.
  TSLCoordinateSystem const* dynamicArc = TSLCoordinateSystem::findByName("Dynamic ARC Grid (Greenwich)");
  
  if ( !dynamicArc )
  {
    return NULL;
  }

  TSLCoordinateSystem * coordSys = dynamicArc->clone( 1351 );
  coordSys->setTMCperMU( TMC_PER_MU );

  return coordSys;
}

void S63Workflow::getCellExpiry(const string &cellName, const string &dataServerID, long long &expiryDate)
{
  vector<pair<const TSLS63CellPermit*, string> >::const_iterator it;
  
  // To get the cell expiry date, simply iterate over the cell permit store
  // and search for the appropriate cell permit.

  for ( it = m_cellPermitStore.begin() ; it != m_cellPermitStore.end() ; ++it ) 
  {
    // As a cell is uniquely identified by it's name and dataserverID, simply
    // compare all keys in the store with the provided name and id.
    if ( cellName == it->first->cellName() && it->second == dataServerID ) 
    {
      expiryDate = it->first->expiryDate();
      return;
    }
  }
}

void S63Workflow::retrieveCellPermits(const string &fileName, const string &dataServerID, vector<pair<const TSLS63CellPermit*, string> >& cellPermits)
{
  generateCellPermits(fileName, dataServerID, cellPermits);
}

bool S63Workflow::generateCellPermits(const string &fileName, const string &dataServerID, vector<pair<const TSLS63CellPermit*, string> >& cellPermits)
{
  // Create a basic work flow client, this is required to report errors only.
  // We provide a blank store, as we have no requirement to use this when generating cell permits.
  S63WorkflowClient * workflowClient = new S63WorkflowClient("", m_currentTime, m_drawingSurface);

  // It is important however to specify the error callback, so the user is alerted
  // of any problems that occur.
  workflowClient->addErrorCallback(m_notifyErrorCallback, m_callbackArgument);
  
  // Create a new cell permit collection providing the specified permit file
  TSLS63CellPermitCollection * permitCollection = TSLS63CellPermitCollection::create(fileName.c_str(), workflowClient);
  
  if ( !permitCollection )
  {
    delete workflowClient;
    return false;
  }

  m_cellPermitCollections.push_back(permitCollection);

  // Loop through each permit in the collection
  int size = permitCollection->size();
  for ( int i = 0 ; i < size; ++i ) 
  {
    // Extract the data server ID from the permit
    const TSLS63CellPermit * permit = (*permitCollection)[i];
    const char * dataServer = permit->dataServerID();

    // If a permit contains a data server, that one has precedent over the user
    // provided one.

    string dsID;

    if ( dataServer )
    {
      dsID = dataServer;
      cellPermits.push_back( make_pair( permit, dsID ) );
    }
    else // If no data server is contained in the PERMIT.TXT, use the user provided one.
    {
      // If the user hasn't provided a data server, return, as a dataserver is required
      if ( dataServerID.empty() )
      {
        delete workflowClient;
        return false;
      }
      else // Otherwise, use the user provided data server.
      {
        dsID = dataServerID; 
        cellPermits.push_back( make_pair( permit, dsID ) );
      }
    }
  }

  delete workflowClient; 

  return true;
}

bool S63Workflow::unlock(const string &unlockKey)
{
  // Attempt to unlock the support via the utility functions class.

  // This unlock call will unlock both S63 and S57 media ingest.
  m_isUnlocked = TSLUtilityFunctions::unlockSupport( TSLKeyedS63, unlockKey.c_str() );

  // If you are only using S57 data then you just need to unlock S57 media-ingest.
  // You can obtain the unlock code from support@envitia.com
  //   m_isUnlocked = TSLUtilityFunctions::unlockSupport( TSLKeyedS57MediaIngest, "unlock code" );
  return m_isUnlocked;
}

bool S63Workflow::unlocked() const
{
  // Return whether or not the functionality has been unlocked.
  return m_isUnlocked;
}

void S63Workflow::setDrawingSurface(TSLDrawingSurface * drawingSurface)
{
  m_drawingSurface = drawingSurface;
}

void S63Workflow::updateScaleBands()
{
  // Ensure the scale bands have been setup
  double lower, upper;

  clearScaleBands();

  m_userScaleBands->getBand(TSLS63ScaleBandOverview, lower, upper);
  setScaleBand(TSLS63ScaleBandOverview, lower, upper);

  m_userScaleBands->getBand(TSLS63ScaleBandGeneral, lower, upper);
  setScaleBand(TSLS63ScaleBandGeneral, lower, upper);

  m_userScaleBands->getBand(TSLS63ScaleBandCoastal, lower, upper);
  setScaleBand(TSLS63ScaleBandCoastal, lower, upper);

  m_userScaleBands->getBand(TSLS63ScaleBandApproach, lower, upper);
  setScaleBand(TSLS63ScaleBandApproach, lower, upper);
 
  m_userScaleBands->getBand(TSLS63ScaleBandHarbour, lower, upper);
  setScaleBand(TSLS63ScaleBandHarbour, lower, upper);
  
  m_userScaleBands->getBand(TSLS63ScaleBandBerthing, lower, upper);
  setScaleBand(TSLS63ScaleBandBerthing, lower, upper);

  writeConfigurationData(false);
}

void S63Workflow::clearScaleBands()
{
  m_scaleBands->removeBand(TSLS63ScaleBandOverview);
  m_scaleBands->removeBand(TSLS63ScaleBandGeneral);
  m_scaleBands->removeBand(TSLS63ScaleBandCoastal);
  m_scaleBands->removeBand(TSLS63ScaleBandApproach);
  m_scaleBands->removeBand(TSLS63ScaleBandHarbour);
  m_scaleBands->removeBand(TSLS63ScaleBandBerthing);

  m_scaleBands->removeBand(TSLS63ScaleBandOverscale);
}


void S63Workflow::configureScaleBands()
{
  // Set the default scale band values.
  setScaleBand(TSLS63ScaleBandOverview, OVERVIEW_LOWER, OVERVIEW_UPPER);
  setScaleBand(TSLS63ScaleBandGeneral, GENERAL_LOWER, GENERAL_UPPER);
  setScaleBand(TSLS63ScaleBandCoastal, COASTAL_LOWER, COASTAL_UPPER);
  setScaleBand(TSLS63ScaleBandApproach, APPROACH_LOWER, APPROACH_UPPER);
  setScaleBand(TSLS63ScaleBandHarbour, HARBOUR_LOWER, HARBOUR_UPPER);
  setScaleBand(TSLS63ScaleBandBerthing, BERTHING_LOWER, BERTHING_UPPER);
#if 0
  // overlapping - for North Carolina - missing scale bands
  setScaleBand(TSLS63ScaleBandOverview, 1500000.0, 999999999.00);
  setScaleBand(TSLS63ScaleBandGeneral, 60000.00, 7000000.00);
  setScaleBand(TSLS63ScaleBandCoastal, 20000.00, 1500000.00);
  setScaleBand(TSLS63ScaleBandApproach, 10000.00, 40000.00);
  setScaleBand(TSLS63ScaleBandHarbour, 200.00, 10000.00);
  setScaleBand(TSLS63ScaleBandBerthing, 1.0, 4000.0);
#endif

  // See if we can access the AppData folder and find the enc_viewer.ini file
  TCHAR appDataFolder[2048];
  if( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataFolder ) != S_FALSE )
  {
    string folder = TSLUTF8Encoder(appDataFolder);
    folder += "/Envitia";

    if (!TSLFileHelper::directoryExists(folder.c_str()))
    {
      TSLFileHelper::createDirectory(folder.c_str());
    }
    folder += "/enc_viewer";
    if (!TSLFileHelper::directoryExists(folder.c_str()))
    {
      TSLFileHelper::createDirectory(folder.c_str());
    }
    m_iniFileLocation = folder + "/enc_viewer.ini";

    TSLProfileHelper::setDefaultFilename(m_iniFileLocation.c_str());

    if (!TSLFileHelper::fileExists(m_iniFileLocation.c_str()))
    {
      writeConfigurationData(true);
    }

    int cacheSize;
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SETTINGS", "Cache", &cacheSize, CACHE_SIZE);
    S63WorkFlowView *view = S63WorkFlowView::GetView();
    if (view)
      view->cacheSize(cacheSize);

    // Note: Defaults should always be sensible.
    double lower, upper;
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandOverview_lower", &lower, OVERVIEW_LOWER);
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandOverview_upper", &upper, OVERVIEW_UPPER);
    setScaleBand(TSLS63ScaleBandOverview, lower, upper);

    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandGeneral_lower", &lower, GENERAL_LOWER);
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandGeneral_upper", &upper, GENERAL_UPPER);
    setScaleBand(TSLS63ScaleBandGeneral, lower, upper);

    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandCoastal_lower", &lower, COASTAL_LOWER);
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandCoastal_upper", &upper, COASTAL_UPPER);
    setScaleBand(TSLS63ScaleBandCoastal, lower, upper);

    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandApproach_lower", &lower, APPROACH_LOWER);
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandApproach_upper", &upper, APPROACH_UPPER);
    setScaleBand(TSLS63ScaleBandApproach, lower, upper);

    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandHarbour_lower", &lower, HARBOUR_LOWER);
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandHarbour_upper", &upper, HARBOUR_UPPER);
    setScaleBand(TSLS63ScaleBandHarbour, lower, upper);

    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandBerthing_lower", &lower, BERTHING_LOWER);
    TSLProfileHelper::lookupProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandBerthing_upper", &upper, BERTHING_UPPER);
    setScaleBand(TSLS63ScaleBandBerthing, lower, upper);
  }
}

void S63Workflow::writeConfigurationData(bool initialSettings)
{
  // find the ini file location.
  if (m_iniFileLocation.empty())
  {
    TCHAR appDataFolder[2048];
    if( SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataFolder ) != S_FALSE )
    {
      string folder = TSLUTF8Encoder(appDataFolder);
      folder += "/Envitia";

      if (!TSLFileHelper::directoryExists(folder.c_str()))
      {
        TSLFileHelper::createDirectory(folder.c_str());
      }
      folder += "/enc_viewer";
      if (!TSLFileHelper::directoryExists(folder.c_str()))
      {
        TSLFileHelper::createDirectory(folder.c_str());
      }
      m_iniFileLocation = folder + "/enc_viewer.ini";

      TSLProfileHelper::setDefaultFilename(m_iniFileLocation.c_str());
    }
  }
  if (!m_iniFileLocation.empty())
  {
    // write the settings out
    if (initialSettings)
    {
    }
    else
    {
    }
    if (m_s52StateObject)
    {
      m_s52StateObject->writeConfiguration(m_iniFileLocation.c_str());
    }

    S63WorkFlowView *view = S63WorkFlowView::GetView();
    int cacheSize = CACHE_SIZE;
    if (view)
      cacheSize = view->cacheSize();

    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SETTINGS", "Cache", cacheSize);
    
    double upper, lower;
    m_userScaleBands->getBand(TSLS63ScaleBandOverview, lower, upper);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandOverview_lower", lower);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandOverview_upper", upper);

    m_userScaleBands->getBand(TSLS63ScaleBandGeneral, lower, upper);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandGeneral_lower", lower);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandGeneral_upper", upper);

    m_userScaleBands->getBand(TSLS63ScaleBandCoastal, lower, upper);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandCoastal_lower", lower);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandCoastal_upper", upper);

    m_userScaleBands->getBand(TSLS63ScaleBandApproach, lower, upper);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandApproach_lower", lower);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandApproach_upper", upper);

    m_userScaleBands->getBand(TSLS63ScaleBandHarbour, lower, upper);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandHarbour_lower", lower);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandHarbour_upper", upper);

    m_userScaleBands->getBand(TSLS63ScaleBandBerthing, lower, upper);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandBerthing_lower", lower);
    TSLProfileHelper::writeProfile(m_iniFileLocation.c_str(), "SCALE_BANDS", "TSLS63ScaleBandBerthing_upper", upper);

    TSLProfileHelper::flushProfileStrings();
  }
}

void S63Workflow::saveSettings(S52StateObject *state)
{
  TSLSimpleString iniFile;
  TSLProfileHelper::getDefaultFilename( iniFile ) ;
  state->writeConfiguration(iniFile);
}