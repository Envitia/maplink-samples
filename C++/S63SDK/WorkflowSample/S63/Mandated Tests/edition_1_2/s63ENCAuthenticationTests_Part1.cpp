//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63ENCAuthenticationTests_Part1.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    S63ENCAuthenticationTests_Part1::S63ENCAuthenticationTests_Part1(const S63ConfigurationObject &config)
      : S63TestBase(config)
    {
      m_dataLocation = config.m_testDataLocation;
      m_configDir = config.m_configurationLocation;
      m_configFile = m_configDir + "S63ENCAuthenticationTests_Part1.ini";
      m_scriptsDir = m_configDir + "../scripts/";

      m_certificatePath = m_dataLocation + "/4 Authentication_Part1/Test 4b/IHO.CRT";

      readConfiguration(m_configFile); 
    }

    S63ENCAuthenticationTests_Part1::~S63ENCAuthenticationTests_Part1()
    {
    }

    void S63ENCAuthenticationTests_Part1::readConfiguration(const string &configFile)
    {
      char buffer[1024];

      // General Info
      TSLProfileHelper::setDefaultFilename( configFile.c_str() );
      TSLProfileHelper::setDefaultSection( "GENERAL" );

      if ( TSLProfileHelper::lookupProfile( "coordinateSystemName", buffer, 1024 ) )
      {
        m_csys = TSLCoordinateSystem::findByName(buffer);
      }

      // User Info
      TSLProfileHelper::setDefaultSection( "USERDETAILS" );
      if ( TSLProfileHelper::lookupProfile( "hw_id", buffer, 1024 ) )
      {
        m_hw_id = buffer;
      }

      // ENC
      TSLProfileHelper::setDefaultSection( "ENC" );

      if ( TSLProfileHelper::lookupProfile( "dsid", buffer, 1024 ) )
      {
        m_dsID = buffer;
      }
    }
    
    
    bool S63ENCAuthenticationTests_Part1::loadTest4a( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath )
    {
      // Read test-specific config
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("permit", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/4 Authentication_Part1/Test 4a/" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4a
      ////////////////////////////////////////////////////////////////////////////////////

      // The data client and ingest manager will be deleted by the caller
      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Failed to create dataClient
      if( !dataClient )
      {
        TSLErrorStack::addError(0, "Failed to create dataClient", TSLErrorCategoryInfo);
        return false;
      }

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      // Note: This is the certificate provided by the IHO.
      //       When the exchange set is ingested, a different certificate will be provided,
      //       as defined by the test definition.
      manager = new TSLS63MediaIngestManager( dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks so far have been as expected
      // Add the first set of cell permits to the manager and make sure that the
      // return value indicates that they are added correctly
      if ( !dataClient->anyErrorSoFar() || !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      // Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      // Load the SERIAL.ENC file
      // file will be deleted by the caller
      file = TSLS63SerialENCFile::create( serialFile.c_str() );
      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set.
      //
      // The error SSE 26 will be raised on the data client.
      //
      // The test specifies that this error must only be raised once, MapLink will
      // instead raise this error for each cell/update that is ingested.
      //
      // The OEM application will need to handle the multiple errors, and only display
      // one to the user in order to pass the test.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    
    bool S63ENCAuthenticationTests_Part1::test4a()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST4A");

      std::string firstError;

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      const char* dataStorePath("OEM_1_2/test4a");

      // Perform test 4a
      bool result = loadTest4a( file, manager, dataClient, dataStorePath);

      if( file )
      {
        file->destroy();
      }
      if( manager )
      {
        manager->destroy();
      }

      if( !dataClient )
      {
        result = false;
      }
      else
      {
        // Ensure the calls to the data client were correct
        if( dataClient->didItWork(&firstError) == false )
        {
          std::string msg("Error from scripted data client: ");
          msg += firstError;
          TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
          result = false;
        }
      }

      delete dataClient;

      return result;
    }


    bool S63ENCAuthenticationTests_Part1::test4b()
    {
      // Read test-specific config - The Test 4B script has the data for 4A and 4B
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST4B");

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      const char* dataStorePath("OEM_1_2/test4b");

      // Load the 4A test
      if ( !loadTest4a( file, manager, dataClient, dataStorePath) )
      {
        TSLErrorStack::addError(0, "Initial test setup invalid, failed to run test 4a.", TSLErrorCategoryInfo);
        if( file )
        {
          file->destroy();
        }
        if( manager )
        {
          manager->destroy();
        }
        if( dataClient )
        {
          delete dataClient;
        }
        return false;
      }

      // Not required for test 4b
      file->destroy();

      // Now load the 4B test data, using the same manager and data client initialised 
      // from 4A above
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("permit4B", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit4B' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("productsfile4B", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile4B' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile4B", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile4B' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/4 Authentication_Part1/Test 4b/" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4b
      ////////////////////////////////////////////////////////////////////////////////////

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        manager->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        manager->destroy();
        return false;
      }

      if ( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        manager->destroy();
        return false;
      }      

      // Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      // Set the base directory for the file paths in the data client script
      // to the test 4b directory.
      dataClient->setExchangeSetPrefix( dataClientExchangeSetPrefix );

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file4B = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file4B )
      {
        TSLErrorStack::addError(0, "Failed to load 4B SERIAL.ENC", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // The error SSE 26 should not be rasied on the data, as the certificate
      // should now point to a valid IHO certificate
      //
      // The test requires that the application informs the user when the new certificate has been installed
      // successfully. 
      // MapLink will not automatically do this, however if loadExchangeSet
      // returns true, the certificate is valid.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file4B, m_certificatePath.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        file4B->destroy();
        delete dataClient;
        manager->destroy();
        return false;
      }

      // cleanup
      file4B->destroy();
      manager->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient->didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      delete dataClient;

      return true;
    }
    
    bool S63ENCAuthenticationTests_Part1::test4c()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST4C");

      std::string firstError;

      // Read test-specific config
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("permit", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/4 Authentication_Part1/Test 4c/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test4c";
      TSLFileHelper::createDirectory(dataStore.c_str());
     
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4c
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager* manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks to far have been as expected
      // Add the first set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      // Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      // Load the SERIAL.ENC file
      TSLS63SerialENCFile* file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set.
      //
      // The error SSE 05 will be raised on the data client, and the load will fail
      // as the certificate cannot be installed
      ////////////////////////////////////////////////////////////////////////////////////
      if ( manager->loadExchangeSet( productsFile.c_str(), file, "" ) == true )
      {
        TSLErrorStack::addError(0, "Loaded exchange set with invalid certificate", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        return false;
      }

      // Cleanup
      file->destroy();
      manager->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }


    bool S63ENCAuthenticationTests_Part1::test4d()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST4D");

      std::string firstError;

      // Read test-specific config
      // There are two version of the key configuration parameters
      // One is 'Expired' which come from the data path with the expired certificate
      // The other is 'Current' which come from the data path with the current certificate
      char buffer[1024];
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("permitExpired", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permitExpired' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permitExpired( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("productsfileExpired", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileExpired' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileExpired( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileExpired", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileExpired' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileExpired( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificateExpired", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificateExpired' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFileExpired( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("permitCurrent", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permitCurrent' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permitCurrent( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("productsfileCurrent", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileCurrent' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileCurrent( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileCurrent", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileCurrent' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileCurrent( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificateCurrent", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificateCurrent' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFileCurrent( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefixExpired( m_dataLocation + "/4 Authentication_Part1/Test 4d/Expired/" );
      std::string dataClientExchangeSetPrefixCurrent( m_dataLocation + "/4 Authentication_Part1/Test 4d/Current/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test4d";
      TSLFileHelper::createDirectory(dataStore.c_str());
     
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4d
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefixExpired, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permitExpired.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager* manager = new TSLS63MediaIngestManager( &dataClient, certificateFileExpired.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks so far have been as expected
      // Add the first set of cell permits to the manager and make sure that the
      // return value indicates that they are added correctly
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile* file = TSLS63SerialENCFile::create( serialFileExpired.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set using expired certificate
      // This call will 'install' the provided certificate, validate it against the previously
      // supplied IHO.CRT (In this case itself), and then ingest the exchange set.
      //
      // SSE 22 will be raised on the data client, as the certificate has expired.
      // SSE 05 will be raised on the data client, as no valid certificate is available for the exchange set ingest.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( manager->loadExchangeSet( productsFileExpired.c_str(), file, certificateFileExpired.c_str() ) == true )
      {
        TSLErrorStack::addError(0, "Loaded exchange set with invalid certificate", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        return false;
      }
      file->destroy();
      manager->destroy();

      ////////////////////////////////////////////////////////////////////////////////////
      // Load a data set using a current certificate
      ////////////////////////////////////////////////////////////////////////////////////

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collectionCurrent = TSLS63CellPermitCollection::create( permitCurrent.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collectionCurrent )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collectionCurrent->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collectionCurrent->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collectionCurrent->destroy();
        return false;
      }


      ////////////////////////////////////////////////////////////////////////////////////
      // Need to construct a new media ingest manager
      // Otherwise, loading the exchange set will create an SSE26 error because the ingest manager certificate
      // will differ from the exchange set certificate
      ////////////////////////////////////////////////////////////////////////////////////
      manager = new TSLS63MediaIngestManager( &dataClient, certificateFileCurrent.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      if ( !manager->addCellPermits( collectionCurrent, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collectionCurrent->destroy();
        manager->destroy();
        return false;
      }      
      //Destroy the collection of cell permits as the manager takes copies
      collectionCurrent->destroy();

      // Set the exchange set prefix
      dataClient.setExchangeSetPrefix( dataClientExchangeSetPrefixCurrent );

      //Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFileCurrent.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 4D Current SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set, which is the same as test above
      //
      // All cells should ingest without warnings/errors.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFileCurrent.c_str(), file, certificateFileCurrent.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        return false;
      }

      file->destroy();
      manager->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }


    bool S63ENCAuthenticationTests_Part1::test4e()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST4E");

      std::string firstError;

      char buffer[1024];
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("permit", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/4 Authentication_Part1/Test 4e/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test4e";
      TSLFileHelper::createDirectory(dataStore.c_str());
     
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4e
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager* manager = new TSLS63MediaIngestManager( &dataClient, certificateFile.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks to far have been as expected
      if ( dataClient.anyErrorSoFar() == false)
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // This test checks that the system performs validation on the pre-installed certificate (IHO.CRT)
      //
      // The system must raise SSE 08 when the certificate is validated (By this point in the test it has been raised).
      // The system must also raise SSE 05 when attempting to install an exchange set. (below)
      ////////////////////////////////////////////////////////////////////////////////////

      // Add the cell permits to the manager
      manager->addCellPermits( collection, m_dsID.c_str() );
      
      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile* file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      // Load the exchange set
      // This must fail, and raise SSE05 on the data client.
      if ( manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) == true )
      {
        TSLErrorStack::addError(0, "Loaded exchange set with invalid certificate", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        return false;
      }

      // Cleanup
      file->destroy();
      manager->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }


    bool S63ENCAuthenticationTests_Part1::test4f()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST4F");

      std::string firstError;

      // Read test-specific config
      char buffer[1024];
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("permitDS1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permitDS1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permitDS1( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("productsfileDS1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileDS1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileDS1( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileDS1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileDS1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileDS1( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificateDS1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificateDS1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFileDS1( m_dataLocation + buffer );
      std::string dataClientExchangeSetPrefixDS1( m_dataLocation + "/4 Authentication_Part1/Test 4f/DS1/" );

      if( !TSLProfileHelper::lookupProfile("permitDS2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permitDS2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permitDS2( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("productsfileDS2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileDS2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileDS2( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileDS2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileDS2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileDS2( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificateDS2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificateDS1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFileDS2( m_dataLocation + buffer );
      std::string dataClientExchangeSetPrefixDS2( m_dataLocation + "/4 Authentication_Part1/Test 4f/DS2/" );

      std::string dataStore = m_resultsPath + "OEM_1_2/test4f";
      TSLFileHelper::createDirectory(dataStore.c_str());
     
      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefixDS1, dataStore, m_csys, false);

      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4f - Data Server 1
      ////////////////////////////////////////////////////////////////////////////////////

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permitDS1.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 3 valid cell permits
      if( collection->size() != 3 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 3 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager* managerDS1 = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      managerDS1->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks to far have been as expected
      if ( dataClient.anyErrorSoFar() == false )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        managerDS1->destroy();
        return false;
      }

      // Add the first set of cell permits to the manager
      if( !managerDS1->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits for DS1 to manager", TSLErrorCategoryInfo);
        collection->destroy();
        managerDS1->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile* fileDS1 = TSLS63SerialENCFile::create( serialFileDS1.c_str() );

      if( !fileDS1 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        managerDS1->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set from Data Server 1
      // 
      // This exchange set should load without any errors/warnings
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !managerDS1->loadExchangeSet( productsFileDS1.c_str(), fileDS1, certificateFileDS1.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        managerDS1->destroy();
        fileDS1->destroy();
        return false;
      }

      // Cleanup
      fileDS1->destroy();
      managerDS1->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }


      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 4f - Data Server 2
      ////////////////////////////////////////////////////////////////////////////////////

      // Set the prefix for all paths in the data client script
      dataClient.setExchangeSetPrefix( dataClientExchangeSetPrefixDS2 );

      // Load PERMIT.TXT 
      collection = TSLS63CellPermitCollection::create( permitDS2.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // Permit should contain 5 valid cell permits
      if( collection->size() != 5 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 3 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Construct the media ingest manager
      //
      // Note: MapLink will not raise SSE26 here, as certificate validation is performed
      //       when an exchange set is ingested.
      ////////////////////////////////////////////////////////////////////////////////////
      TSLS63MediaIngestManager* managerDS2 = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      managerDS2->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks to far have been as expected
      if ( dataClient.anyErrorSoFar() == false)
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        managerDS2->destroy();
        return false;
      }

      // Add the first set of cell permits to the manager
      if( !managerDS2->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits for DS2 to manager", TSLErrorCategoryInfo);
        collection->destroy();
        managerDS2->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile* fileDS2 = TSLS63SerialENCFile::create( serialFileDS2.c_str() );

      if( !fileDS2 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        managerDS2->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set for Data Server 2
      //
      // The error SSE 26 will be raised on the data client, as this certificate
      // cannot authenticate against the IHO certificate.
      //
      // The test specifies that this error must only be raised once, MapLink will
      // instead raise this error for each cell/update that is ingested.
      //
      // The OEM application will need to handle the multiple errors, and only display
      // one to the user in order to pass the test.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !managerDS2->loadExchangeSet( productsFileDS2.c_str(), fileDS2, certificateFileDS2.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        managerDS2->destroy();
        fileDS2->destroy();
        return false;
      }

      fileDS2->destroy();
      managerDS2->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }
  };
};