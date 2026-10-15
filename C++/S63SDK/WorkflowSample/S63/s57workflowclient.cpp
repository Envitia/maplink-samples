//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "S57WorkflowClient.h"
#include <sstream>
#include <limits>

static TSLGeodeticExtent sGeodeticExtent;

S57WorkflowClient::S57WorkflowClient(const string &dataStore, const long long currentTime, TSLDrawingSurface *surface)
: m_dataStore(dataStore)
, m_currentTime(currentTime)
, m_ingestCellCallback(NULL)
, m_cancelledCellCallback(NULL)
, m_progressCallback(NULL)
, m_userCallbackArgument(NULL)
, m_notifyErrorCallbacks()
, m_serialPath()
, m_dataServers()
, m_cellVersions()
, m_cellExtents()
, m_displayIngestDialog(false)
, m_coordSys(NULL)
, m_cellNames(NULL)
, m_dataServerIDs(NULL)
{
  // Create the root directory.
  TSLFileHelper::createDirectory(m_dataStore.c_str());


  // We need a coordinate system (projection) - try the background map first.
  const TSLCoordinateSystem * dynamicArc = NULL;
  /*
  // This is an example of querying the background map.
  if (surface)
  {
    TSLDataLayer *layer = surface->getDataLayer(MAP_LAYER_NAME);
    if (layer)
    {
      if (layer->layerType() == TSLDataLayerTypeMapDataLayer)
      {
        dynamicArc = ((TSLMapDataLayer*)layer)->queryCoordinateSystem();

        if (dynamicArc)
        {
          m_coordSys = dynamicArc->clone( 1351 );
        }
      }
    }
  }*/
  if (!dynamicArc)
  {
    // Create and clone a coordinate system with which we will display the S63 data.
    dynamicArc = TSLCoordinateSystem::findByName("Dynamic ARC Grid (Greenwich)");

    if (dynamicArc)
    {
      m_coordSys = dynamicArc->clone( 1351 );
      m_coordSys->setTMCperMU( TMC_PER_MU );
    }
  }

  // Append a separator to the provided data store, this makes life easier
  // later on.
  m_dataStore += "\\";

  // At the point of creating the client, one of two situations may be occurring:
  // 1 - The user is using the client to perform a media ingest.
  // 2 - The user is using the client to manage an S63 data layer.

  // Given the above, we may be pointing to an existing store or a new store.
  // If it's an existing store we should now look for the index file so that 
  // we can extract extent and version information.

  //Firstly, retrieve the data servers from the index file
  string indexFile(m_dataStore);
  indexFile += "index.ini";

  // Retrieve the number of data servers from the index file.
  int count = 0;
  TSLProfileHelper::lookupProfile(indexFile.c_str(), "DataServers", "Count", &count);

  // If we have no count, there's no point continuing as we have no data server information.
  if ( !count ) 
  {
    return;
  }

  // Loop through all the entries in the index file and populate the data servers collection.
  char buffer[1024];
  for ( int i = 0; i < count ; ++ i )
  {
    TSLProfileHelper::lookupProfile(indexFile.c_str(), "DataServers", "DataServer", buffer, 1024, "", i);
    
    if ( strcmp(buffer, "") != 0 )
    {
      string dataServer(buffer);
      m_dataServers.push_back(dataServer);
    }
  }

  // Now that we have a list of data servers, we have to analyse the appropriate
  // index files for all provided data servers.

  vector<string>::iterator it;
  for ( it = m_dataServers.begin(); it != m_dataServers.end(); ++it )
  {
    // Working from the data store, the data server index file is located at:
    // DATA_STORE\DATA_SERVER_ID\DATA_SERVER_ID.ini".

    string dataServerFile(m_dataStore);
    dataServerFile += *it;
    dataServerFile += "/";
    dataServerFile += *it;
    dataServerFile += ".ini";

    // Retrieve the number of cells from the data server index file.
    int cellCount = 0;
    TSLProfileHelper::lookupProfile(dataServerFile.c_str(), "Cells", "Count", &cellCount, 0);

    // Again, if we have no count value, there's no point continuing with this data server
    if ( !cellCount ) 
    {
      continue;
    }

    char cellBuffer[1024];
    for ( int cellIndex = 0; cellIndex < cellCount; ++cellIndex )
    {
      // Retrieve the cell information from the data server index file
      TSLProfileHelper::lookupProfile(dataServerFile.c_str(), "Cells", "Cell", cellBuffer, 1024, "", cellIndex);

      // Provided we have a cell name, we can now load the individual cell information
      if ( strcmp(cellBuffer, "") != 0 )
      {         
        int version;
        double lonBL, latBL, lonTR, latTR;
   
        string cellName(cellBuffer);

        if ( cellName.substr(0, 9) == "CANCELLED" )
        {
          // If the cell has been cancelled, don't include it in memory.
          continue;
        }

        // Load the version and extent information from the file.
        TSLProfileHelper::lookupProfile(dataServerFile.c_str(), cellName.c_str(), "version", &version);
        TSLProfileHelper::lookupProfile(dataServerFile.c_str(), cellName.c_str(), "lonBL", &lonBL);
        TSLProfileHelper::lookupProfile(dataServerFile.c_str(), cellName.c_str(), "latBL", &latBL);
        TSLProfileHelper::lookupProfile(dataServerFile.c_str(), cellName.c_str(), "lonTR", &lonTR);
        TSLProfileHelper::lookupProfile(dataServerFile.c_str(), cellName.c_str(), "latTR", &latTR);

        // Generate an extent, based on the values we have extracted
        TSLGeodeticExtent extent(latBL, lonBL, latTR, lonTR) ;

        // Finally, add these values to the version and cell maps.
        // We can uniquely identify the values by having a key of < DataServerID, Cell Name >.
        m_cellVersions.insert( make_pair( make_pair( cellName, *it ), version ) ) ;
        m_cellExtents.insert( make_pair( make_pair( cellName, *it ), extent ) ) ;
      }
    }
  }
}

S57WorkflowClient::~S57WorkflowClient()
{
  if ( m_coordSys )
  {
    m_coordSys->destroy();
  }

  // Check & deallocate array pointers
  if ( m_cellNames )
  {
    deallocateArrayPointers( m_cellNames );
  }
  if ( m_dataServerIDs )
  {
    deallocateArrayPointers( m_dataServerIDs );
  }
}

void S57WorkflowClient::attachmentForCell (const char* filename, const char* cellName, const char* dataServerID, TSLSimpleString& attachmentID, TSLSimpleString& attachmentLookup)
{
  const string dataServer = dataServerID;
  string cellDir = "";
  string cell(cellName);

  // Before we proceed with storing the attachment, ensure we have the correct directory structure set up.
  constructBaseDirectory(dataServer, cell, cellDir) ;

  // We will also need the attachments directory created
  string attachDir = cellDir;
  attachDir += "/attach/";

  TSLFileHelper::createDirectory( attachDir.c_str() );

  // Extract the file name from the provided file.
  char file_name[_MAX_FNAME];
  char file_extension[_MAX_EXT];
  
  _splitpath(filename, NULL, NULL, file_name, file_extension);

  // Set up the path to the attachment, relative to the data store.
  string attachPath = attachDir;
  attachPath += file_name;
  attachPath += file_extension;
 
  // Copy the file from the provided location to an appropriate location in the store.
  TSLFileHelper::copyFile( filename, attachPath.c_str() );
}

bool S57WorkflowClient::cellCancelled (const char* cellName, const char* dataServerID, const char** replacementCells, int numberOfReplacementCells)
{
  // When this method is called, it indicates that a cell has been cancelled

  // We therefore ask the user whether or not they wish to keep this cell, or
  // whether they would prefer to discard it.

  if ( m_cancelledCellCallback ) 
  {
    if ( !m_cancelledCellCallback( cellName, dataServerID, replacementCells, numberOfReplacementCells ) )
    {
      // The user has decided to discard the cell, so we go about removing it from the data store

      // First, remove it from memory.       
      map<pair<string, string>, int>::iterator versionIterator;
      map<pair<string, string>, TSLGeodeticExtent>::iterator extentIterator;

      versionIterator = m_cellVersions.find( make_pair( cellName, dataServerID ) ) ;
      extentIterator = m_cellExtents.find( make_pair( cellName, dataServerID ) ) ;

      if ( versionIterator == m_cellVersions.end() || extentIterator == m_cellExtents.end() ) 
      {
        // We never had it in memory, suggesting we never had it all!
        return false;
      }

      m_cellVersions.erase( versionIterator ) ;
      m_cellExtents.erase( extentIterator ) ;

      //Update the configuration file for the data server in question
      string configFile = m_dataStore;
      configFile += dataServerID;
      configFile += "/";
      configFile += dataServerID;
      configFile += ".ini";

      // Blank out the version and extent information.
      TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "version", " ");
      TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "lonBL", " ");
      TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "latBL", " ");
      TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "lonTR", " ");
      TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "latTR", " ");

      // Next, we update the cell index section

      // First, get the number of cells there.
      int cellCount = 0;
      TSLProfileHelper::lookupProfile( configFile.c_str(), "Cells", "Count", &cellCount ) ;

      char cellBuffer[1024];
      for ( int cellIndex = 0; cellIndex < cellCount; ++cellIndex )
      {
        // Retrieve the cell information from the data server index file
        TSLProfileHelper::lookupProfile(configFile.c_str(), "Cells", "Cell", cellBuffer, 1024, "", cellIndex);

        if ( strcmp(cellBuffer, cellName) == 0 )
        {
          string cancelledCell("CANCELLED - ");
          cancelledCell += cellName;

          TSLProfileHelper::writeProfile(configFile.c_str(), "Cells", "Cell", cancelledCell.c_str(), cellIndex );

          break;
        }
      }

      // Finally, remove the associated files
      string cellDirectory(m_dataStore);
      cellDirectory += dataServerID;
      cellDirectory += "/";
      cellDirectory += cellName;

      string tmfDirectory(cellDirectory);
      tmfDirectory += "/tmf/";

      string blobDirectory(cellDirectory);
      blobDirectory += "/blob/";

      string tmfFile(tmfDirectory);
      tmfFile += cellName;
      tmfFile += ".tmf";

      string blobFile(blobDirectory);
      blobFile += cellName;
      blobFile += ".blb";

      // Remove Blob File
      TSLFileHelper::deleteFile( blobFile.c_str() );

      // Remove Encrypted TMF
      TSLFileHelper::deleteFile( tmfFile.c_str() );

      // Remove Directories
      TSLFileHelper::deleteDirectory( blobDirectory.c_str() );
      TSLFileHelper::deleteDirectory(  tmfDirectory.c_str() );
      TSLFileHelper::deleteDirectory( cellDirectory.c_str() );

      return false;
    }
  }

  return true;
}

bool S57WorkflowClient::cellsInExtent (const TSLGeodeticExtent& extent, const char **&cellNames, const char **&dataServerIDs, int& numberOfCells) 
{
  // If the data store contains no cells, return failure.
  if ( m_cellExtents.empty() )
  {
    return false;
  }

  // Otherwise we need to iterate over the cells to determine if they interesect the extent parameter.
  map<pair<string, string>, TSLGeodeticExtent>::const_iterator it(m_cellExtents.begin());
  map<pair<string, string>, TSLGeodeticExtent>::const_iterator itEnd(m_cellExtents.end());

  vector< map<pair<string, string>, TSLGeodeticExtent>::const_iterator > cells;

  for ( ; it != itEnd; ++it )
  {
    TSLGeodeticExtent cellExtent = it->second;

    // Does this cell intersect the current extent?
    if  ( !(extent.longitudeBL() <= cellExtent.longitudeTR()
       && extent.latitudeBL() <= cellExtent.latitudeTR()
	     && extent.longitudeTR()   >= cellExtent.longitudeBL()
       && extent.latitudeTR()   >= cellExtent.latitudeBL()) ) 
    {
      continue;
    }

    // If it does, push it on the short list.
    cells.push_back(it);
  }
  
  numberOfCells = (int)cells.size();

  // If no cells have been found, return failure.
  if ( numberOfCells <= 0 )
  {
    return false;
  }

  // Check & deallocate array pointers
  if ( m_cellNames )
  {
    deallocateArrayPointers( m_cellNames );
  }
  if ( m_dataServerIDs )
  {
    deallocateArrayPointers( m_dataServerIDs );
  }

  // Create the new array pointers.
  m_cellNames = allocateArrayPointers( numberOfCells );
  m_dataServerIDs = allocateArrayPointers( numberOfCells );
    
  // And populate the arrays with the cell and dataserver information.
  for ( int i = 0; i < numberOfCells; ++i )
  {
    m_cellNames[i] = cells[i]->first.first.c_str();
    m_dataServerIDs[i] = cells[i]->first.second.c_str();
  }

  cellNames = m_cellNames;
  dataServerIDs = m_dataServerIDs;

  return true;
}

const char** S57WorkflowClient::allocateArrayPointers ( int numElements )
{
  return new const char*[numElements];
}

void S57WorkflowClient::deallocateArrayPointers( const char** constCharArray )
{
  delete [] constCharArray;
  constCharArray = 0;
}


const TSLCoordinateSystem* S57WorkflowClient::coordinateSystemForCell (const char* cellName, const char* dataServerID)
{
  return m_coordSys;
}

int S57WorkflowClient::currentCellVersion (const char* cellName, const char* dataServerID)
{
  // To provide the cell version we must search our in-memory collections.
  // So simply search our cell versions map, using the provided cell name and data server ID as the key.
  string cell(cellName);
  string dataServer(dataServerID);

  map<pair<string, string>, int>::const_iterator mapIterator = m_cellVersions.find( make_pair( cell, dataServer ) );
  
  if ( mapIterator != m_cellVersions.end() )
  {
    // Return the version.
    return mapIterator->second;
  }

  // If the cell has not been found in memory, we don't own it.
  // Therefore we return -1 to indicate we don't have the cell.
  return -1;
}

const TSLGeodeticExtent& S57WorkflowClient::dataStoreExtent ()
{
  // If the cell extents are empty, return an empty extent.
  if ( m_cellExtents.empty() )
  {
    return sGeodeticExtent;
  }

  map<pair<string, string>, TSLGeodeticExtent>::const_iterator it(m_cellExtents.begin());
  map<pair<string, string>, TSLGeodeticExtent>::const_iterator itEnd(m_cellExtents.end());

  vector< map<pair<string, string>, TSLGeodeticExtent>::const_iterator > cells;

  double minLat = DBL_MAX;
  double minLon = DBL_MAX;
  double maxLat = -(DBL_MAX);
  double maxLon = -(DBL_MAX);
  
  // Iterate over the cell extents to determine the current data store extent.
  for ( ; it != itEnd; ++it )
  {
    TSLGeodeticExtent cellExtent = it->second;

    if (cellExtent.longitudeBL() < minLon)
    {
      minLon = cellExtent.longitudeBL();
    }
    
    if (cellExtent.latitudeBL() < minLat)
    {
      minLat = cellExtent.latitudeBL();
    }
    
    if (cellExtent.longitudeTR() > maxLon)
    {
      maxLon = cellExtent.longitudeTR();
    }
    
    if (cellExtent.latitudeTR() > maxLat)
    {
      maxLat = cellExtent.latitudeTR();
    }
  }

  // Return the current data store extent.
  m_extent =   TSLGeodeticExtent(minLat, minLon, maxLat, maxLon);
  return m_extent;
}

TSLTimeType S57WorkflowClient::currentTime ()
{
  // Return the time specified in the configuration dialog  
  return m_currentTime;
}

TSLS63CellLoadResultEnum S57WorkflowClient::loadCellIntermediaryData (const char* cellName, const char* dataServerID, TSLS63EncryptedData* intermediaryData)
{    
  // The path to the cell intermediary data is as follows:
  // DATA_STORE\DATA_SERVER_ID\CELL_NAME\BLOB\CELL_NAME.blb

  // Generate the path to the blob to be loaded.
  string pathToBlob = m_dataStore;
  pathToBlob += dataServerID;
  pathToBlob += "/";
  pathToBlob += cellName;
  pathToBlob += "/blob/";
  pathToBlob += cellName;
  pathToBlob += ".blb";
  
  // Attempt to open the blob.
  TSLifstream ifstream(pathToBlob.c_str());

  // If the blob isn't present, return false.
  if ( !ifstream.is_open() )
  {
    return TSLS63CellLoadResultNotFound;
  }

  // Load the blob in to the intermediaryData variable.
  const bool result = intermediaryData->loadData(ifstream);

  // Close the stream and return.
  ifstream.close();

  return (result ? TSLS63CellLoadResultOK : TSLS63CellLoadResultNotFound);
}

TSLS63CellLoadResultEnum S57WorkflowClient::loadCellTMFData (const char* cellName, const char* dataServer, TSLS63EncryptedData* tmfData)
{
  // The path to the cell TMF is as follows:
  // DATA_STORE\DATA_SERVER_ID\CELL_NAME\TMF\CELL_NAME.tmf

  // Generate the path to the tmf to be loaded.
  string pathToTMF = m_dataStore;
  pathToTMF += dataServer;
  pathToTMF += "/";
  pathToTMF += cellName;
  pathToTMF += "/tmf/";
  pathToTMF += cellName;
  pathToTMF += ".tmf";

  // Attempt to open the tmf.
  TSLifstream ifstream(pathToTMF.c_str());

  // If the tmf isn't present, return false.
  if ( !ifstream.is_open() )
  {
    return TSLS63CellLoadResultNotFound;
  }

  // Load the tmf in to the tmfData variable.
  const bool result = tmfData->loadData(ifstream);

  // Close the stream and return
  ifstream.close();

  return (result ? TSLS63CellLoadResultOK : TSLS63CellLoadResultNotFound);
}

bool S57WorkflowClient::locateCatalogueForCell (const char* cellName, const char* dataServerID, int exchangeSetNumber, TSLSimpleString& locationOfCatalogue)
{
  return false;
}

void S57WorkflowClient::notifyError (TSLS63ErrorCodeEnum s63ErrorCode, const char* cellName, const char* dataServerID, const char* additionalInformation)
{
  // The notify error callback is called if an error is identified 
  // at any stage of processing S63 data.

  // The entries below are the errors specified in section 11 of the
  // IHO S-63 Data Protection Scheme specification document.

  // If we have no callbacks, return.
  if ( m_notifyErrorCallbacks.empty() )
  {
    return;
  }

  string errorMessage;

  switch(s63ErrorCode)
  {
  case TSLS63ErrorCodeSSE1:
    {
      errorMessage = "Self Signed Key is invalid";
      break;
    }
  case TSLS63ErrorCodeSSE2:
    {
      errorMessage = "Format of Self Signed Key file is incorrect";
      break;
    }
  case TSLS63ErrorCodeSSE3:
    {
      errorMessage = "SA Signed Data Server Certificate is invalid";
      break;
    }
  case TSLS63ErrorCodeSSE4:
    {
      errorMessage = "Format of SA Signed DS Certificate is incorrect";
      break;
    }
  case TSLS63ErrorCodeSSE5: 
    {
      errorMessage = "SA Digital Certificate (X509) file is not available. A valid certificate can be obtained from the IHO website or your data supplier.";
      break;
    }
  case TSLS63ErrorCodeSSE6:
    {
      errorMessage = "The SA Signed Data Server Certificate is invalid. The SA may have issued a new public key or the ENC may originate from another service. A new SA public key can be obtained from the IHO website or from your data supplier";
      break;
    }
  case TSLS63ErrorCodeSSE7:
    {
      errorMessage = "SA signed DS Certificate file is not available. A valid certificate can be obtained from the IHO website or your data supplier";
      break;
    }
  case TSLS63ErrorCodeSSE8:
    {
      errorMessage = "SA Digital Certificate (X509) file incorrect format. A valid certificate can be obtained from the IHO website or your data supplier.";
      break;
    }
  case TSLS63ErrorCodeSSE9:
    {
      errorMessage = "ENC signature is invalid";
      break;
    }
  case TSLS63ErrorCodeSSE10:
    {
      errorMessage = "Permits not available for this Data Server. Contact your data supplier to obtain the correct permits.";
      break;
    }
  case TSLS63ErrorCodeSSE11:
    {
      errorMessage = "Cell Permit file not found. Load the permit file provided by the data supplier.";
      break;
    }
  case TSLS63ErrorCodeSSE12:
    {
      errorMessage = "Cell Permit format is incorrect. Contact your data supplier and obtain a new permit file.";
      break;
    }
  case TSLS63ErrorCodeSSE13:
    {
      errorMessage = "Cell Permit validation failed";
      break;
    }
  case TSLS63ErrorCodeSSE14:
    {
      errorMessage = "Incorrect system date, check that the computer clock (if accessible) is set correctly or contact your system supplier.";
      break;
    }
  case TSLS63ErrorCodeSSE15:
    {
      errorMessage = "Subscription service has expired. Please contact your data supplier to renew the subscription licence";
      break;
    }
  case TSLS63ErrorCodeSSE16:
    {
      errorMessage = "ENC CRC value is incorrect. Contact your data supplier as ENC(s) may be corrupted or missing data.";
      break;
    }
  case TSLS63ErrorCodeSSE17:
    {
      errorMessage = "Userpermit is invalid (checksum is incorrect). Check that the correct hardware device (dongle) is connected or contact your system supplier to obtain a valid userpermit.";
      break;
    }
  case TSLS63ErrorCodeSSE18: 
    {
      errorMessage = "HW_ID is incorrect format";
      break;
    }
  case TSLS63ErrorCodeSSE19:
    {
      errorMessage = "Permits are not valid for this system. Contact your data supplier to obtain the correct permits";
      break;
    }
  case TSLS63ErrorCodeSSE20: 
    {
      errorMessage = "Subscription service will expire in less than 30 days. Please contact your data supplier to renew the subscription licence";
      break;
    }
  case TSLS63ErrorCodeSSE21:
    {
      errorMessage = "Decryption failed no valid cell permit found. Permits may be for another system or new permits may be required, please contact your supplier to obtain a new licence";
      break;
    }
  case TSLS63ErrorCodeSSE22:
    {
      errorMessage = "SA Digital Certificate (X509) has expired. A new SA public key can be obtained from the IHO website or from your data supplier.";
      break;
    }
  case TSLS63ErrorCodeSSE23:
    {
      errorMessage = "SSE 23 Non sequential update, previous update(s) missing try reloading from the base media. If the problem persists contact your data supplier.";
      break;
    }
  case TSLS63ErrorCodeSSE24:
    {
      errorMessage = "ENC Signature format incorrect, contact your data supplier";
      break;
    }
  case TSLS63ErrorCodeSSE25:
    {
      errorMessage = "The permit for ENC ";
      errorMessage += cellName;
      errorMessage += " has expired. This cell may be out of date and MUST NOT be used for Primary NAVIGATION.";
      break;
    }
  case TSLS63ErrorCodeSSE26:
    {
      errorMessage = "This ENC is not authenticated by the IHO acting as the Scheme Administrator";
      break;
    }
  case TSLS63ErrorCodeSSE27:
    {
      errorMessage = "ENC cell ";
      errorMessage += cellName;
      errorMessage += " is not up to date. A New Edition, Re-issue or Update for this cell is missing and therefore MUST NOT be used for Primary NAVIGATION.";
      break;
    }
  default:
    {
      errorMessage = "Unknown Error";
      break;
    }
  }


  list< pair< NotifyErrorCallback, void * > >::iterator callbackIt;

  for ( callbackIt = m_notifyErrorCallbacks.begin() ; callbackIt != m_notifyErrorCallbacks.end() ; ++callbackIt )
  {
    (callbackIt->first)(s63ErrorCode, cellName, dataServerID, errorMessage.c_str(), callbackIt->second);
  }


}

void S57WorkflowClient::notifyWarning (const char* additionalInformation, const char* cellName, const char* dataServerID)
{
  // If a non-fatal error is raised, this call back will be hit.

  // Firstly, we must ensure we have a GUI callback to return our errors to.
  if ( !m_notifyErrorCallbacks.empty() )
  {
    //If we do, we provide '-1' as an error code. 
    //This identifies the message as a warning and not an error.

    list< pair< NotifyErrorCallback, void * > >::iterator callbackIt;

    for ( callbackIt = m_notifyErrorCallbacks.begin() ; callbackIt != m_notifyErrorCallbacks.end() ; ++callbackIt )
    {
      (callbackIt->first)(-1, cellName, dataServerID, additionalInformation, callbackIt->second);
    }
  }
}


void S57WorkflowClient::populateEntityInformation (TSLEntity* entity, TSLEntitySet* entitySet, const char* cellName, const char* dataServerID)
{
  ////////////////////////////////////////////////
  ////     TEST PLAN REFERENCE: 3.4.2.3.3     ////
  ////////////////////////////////////////////////

  // This callback gives the user the opportunity to set specific information
  // on an entity. For example: Make a specific entity's edge style red.
}

bool S57WorkflowClient::progress (double percentOfExchangeSetDone, double percentOfCurrentCellDone)
{
  if ( m_progressCallback ) 
  {
    return m_progressCallback( percentOfExchangeSetDone, percentOfCurrentCellDone, m_userCallbackArgument);
  }
  return true;
}

void S57WorkflowClient::storeIngestedCell (const TSLS63EncryptedData* cellTMF, const TSLS63EncryptedData* cellIntermediaryData, const char* cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber)
{
  //////////////////////////////////////
  // Test Plan Reference: 3.4.2.3.3
  //  - Storage of information for update and display.
  /////////////////////////////////////

  // When this method is called, it indicates that a cell is ready for transfer to the
  // data store. 

  // We therefore firstly ask the user whether or not they wish to ingest this cell, or
  // whether they would prefer to discard it.

  // If the m_displayIngestDialog flag is not set, we assume they wish to accept all 
  // valid cells.

  if ( m_ingestCellCallback && m_displayIngestDialog ) 
  {
    if ( !m_ingestCellCallback( cellName, dataServerID, cellExtent, cellEdition, cellUpdateNumber ) )
    {
      return;
    }
  }
  
  ////////////////////////////////////////////////
  ////     TEST PLAN REFERENCE: 3.4.2.3.3     ////
  ////////////////////////////////////////////////

  // If the callback hasn't been set, or the user accepted, we proceed with storing the cell.

  // What's being stored:
  // - Encrypted TMF data, which contains the cell geometry and coordinate system.
  // - Intermediary data, which contains the s57 brand data.
  // - Cell information, which contains the extent and versioning information.

  // Firstly, we construct the base directory structure to ensure we have somewhere
  // to copy to!
  string dataServer(dataServerID);
  string cell(cellName);
  string cellDir = "";

  constructBaseDirectory(dataServer, cell, cellDir) ;

  // Make the tmf directory
  string tmfDir = cellDir;
  tmfDir += "/tmf/";

  TSLFileHelper::createDirectory( tmfDir.c_str() );

  // Make the blob directory
  string blobDir = cellDir;
  blobDir += "/blob/";

  TSLFileHelper::createDirectory( blobDir.c_str() );

  // At this point, the directory structure has been setup. We now 
  // proceed with saving out the relevant information for use later.

  // TMF
  string tmfFile = tmfDir;
  tmfFile += cellName;
  tmfFile += ".tmf";

  // Create a stream and save out the TMF.
  if ( cellTMF ) 
  {
    TSLofstream tmfStream( tmfFile.c_str() );
    cellTMF->saveData(tmfStream);
    tmfStream.close();
  }

  // Blob
  string blobFile = blobDir;
  blobFile += cellName;
  blobFile += ".blb";

  if ( cellIntermediaryData ) 
  {
    // Create a stream and save out the blob.
    TSLofstream blobStream( blobFile.c_str() );
    cellIntermediaryData->saveData(blobStream);
    blobStream.close();
  }

  // Now that we have saved out the blob and TMF information
  // we must update the configuration files to reflect the changes.

  // Firstly, we update the data server configuration file.
  string configFile = m_dataStore;
  configFile += dataServerID;
  configFile += "/";
  configFile += dataServerID;
  configFile += ".ini";

  // Write out the version and extent information.
  TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "version", cellUpdateNumber);
  TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "lonBL", cellExtent.longitudeBL());
  TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "latBL", cellExtent.latitudeBL());
  TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "lonTR", cellExtent.longitudeTR());
  TSLProfileHelper::writeProfile(configFile.c_str(), cellName, "latTR", cellExtent.latitudeTR());

  // At this point, we should have stored everything as required.
  // However, if this cell is new (i.e. not in memory) we will have to
  // add it to the data server index file and potentially the main index file.

  // If we have no in memory copy, we'll need to update the data server index file
  map<pair<string, string>, int>::const_iterator mapIterator = m_cellVersions.find( make_pair( cellName, dataServer ) );
  
  if ( mapIterator == m_cellVersions.end()  )
  {
    // Firstly, get the number of cells already in the file
    int noOfCells = 0;
    TSLProfileHelper::lookupProfile( configFile.c_str(), "Cells", "Count", &noOfCells ) ;

    // Now add the new cell to the end of the section
    TSLProfileHelper::writeProfile( configFile.c_str(), "Cells", "Cell", cellName, noOfCells);

    // Increment the count and re-save it
    noOfCells++;
    TSLProfileHelper::writeProfile( configFile.c_str(), "Cells", "Count", noOfCells );

    // We may also need to update the dataserver index, so let's check to see
    // if the data server is stored in memory.
    vector<string>::iterator dataServerIt;
    bool found(false);

    for ( dataServerIt = m_dataServers.begin(); dataServerIt != m_dataServers.end(); ++dataServerIt )
    {
      if ( (*dataServerIt) == dataServerID )
      {
        found = true;
        break;
      }
    }

    // If the data server ID is not held in memory, we will also need to update the data server register
    if ( !found ) 
    {
      // Create the path to the main data server register
      string indexFile(m_dataStore);
      indexFile += "index.ini";

      // Firstly, get the number of data servers already in the file
      int noOfDS = 0;
      TSLProfileHelper::lookupProfile( indexFile.c_str(), "DataServers", "Count", &noOfDS ) ;

      // Now add the data server on to the end of the section
      TSLProfileHelper::writeProfile( indexFile.c_str(), "DataServers", "DataServer", dataServerID, noOfDS);

      // Increment the count and re-save it
      noOfDS++;
      TSLProfileHelper::writeProfile( indexFile.c_str(), "DataServers", "Count", noOfDS );

      // It's also important to add the data server to the in-memory collection
      m_dataServers.push_back(dataServerID);
    }

  }

  // Finally, we update the version and extents collection with the new ingest information
  m_cellVersions.insert( make_pair( make_pair( cellName, dataServerID ), cellEdition ) ) ;
  m_cellExtents.insert( make_pair( make_pair( cellName, dataServerID ), cellExtent ) ) ;

}

void S57WorkflowClient::constructBaseDirectory(const string &dataServer, const string &cellName, string &cellPath) const
{
  // Make the base directory
  TSLFileHelper::createDirectory( m_dataStore.c_str() );

  // Make the data server directory
  string dataServerDir = m_dataStore;
  dataServerDir += "/" + dataServer;

  TSLFileHelper::createDirectory( dataServerDir.c_str() );

  // Make the cell directory
  cellPath = dataServerDir;
  cellPath += "/" + cellName;

  TSLFileHelper::createDirectory( cellPath.c_str() );
}  

bool S57WorkflowClient::fileExists(const TSLSimpleString &filePath) const
{
  return TSLFileHelper::fileExists( filePath.c_str() );
}  

void S57WorkflowClient::displayIngestDialog(const bool displayIngestDialog)
{
  m_displayIngestDialog = displayIngestDialog;
}

void S57WorkflowClient::setSerialPath(const string &serialPath)
{
  m_serialPath = serialPath;
}

void S57WorkflowClient::addErrorCallback(const NotifyErrorCallback callback, void * arg)
{
  m_notifyErrorCallbacks.push_back( make_pair( callback, arg ) );
}

void S57WorkflowClient::setIngestCallback(const IngestCellCallback callback)
{
  m_ingestCellCallback = callback;
}

void S57WorkflowClient::setProgressCallback(const ProgressCallback callback)
{
  m_progressCallback = callback;
}

void S57WorkflowClient::setCancelledCellCallback(const CancelledCellCallback callback)
{
  m_cancelledCellCallback = callback;
}

void S57WorkflowClient::setUserCallbackArgument( void *userArg )
{
  m_userCallbackArgument = userArg;
}

vector<string>& S57WorkflowClient::dataServers()
{
  return m_dataServers;
}

map<pair<string, string>, TSLGeodeticExtent>& S57WorkflowClient::cellExtents()
{
  return m_cellExtents;
}