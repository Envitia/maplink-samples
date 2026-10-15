//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63ENCAuthenticationTests_Part2.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    S63ENCAuthenticationTests_Part2::S63ENCAuthenticationTests_Part2(const S63ConfigurationObject &config)
      : S63TestBase(config)
    {
      m_dataLocation = config.m_testDataLocation;
      m_configDir = config.m_configurationLocation;
      m_configFile = m_configDir + "S63ENCAuthenticationTests_Part2.ini";
      m_scriptsDir = m_configDir + "../scripts/";

      m_certificatePath = m_dataLocation + "/4 Authentication_Part1/Test 4b/IHO.CRT";

      readConfiguration(m_configFile); 
    }

    S63ENCAuthenticationTests_Part2::~S63ENCAuthenticationTests_Part2()
    {
    }

    void S63ENCAuthenticationTests_Part2::readConfiguration(const string &configFile)
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
    
    

    bool S63ENCAuthenticationTests_Part2::test5a()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST5A");

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      // Load the configuration data
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
        delete dataClient;
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/5 Authentication_Part2/Test 5a/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test5a";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 5a
      ////////////////////////////////////////////////////////////////////////////////////

      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      // Permit should contain 2 valid cell permits
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      // Note that this certificate is provided by the IHO, and is different to that used below
      manager = new TSLS63MediaIngestManager( dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        delete dataClient;
        return false;
      }

      if ( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        delete dataClient;
        return false;
      }      

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 5A SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // The error SSE 06 should be rasied on the data client for cell GB61021A
      // Cell GB61021B should load without error
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }

      //Destroy the SERIAL.ENC file and manager
      file->destroy();
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


    bool S63ENCAuthenticationTests_Part2::test5b()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST5B");

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      // Load the configuration data
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
        delete dataClient;
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/5 Authentication_Part2/Test 5b/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test5b";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 5b
      ////////////////////////////////////////////////////////////////////////////////////

      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      // Permit should contain 3 valid cell permits
      if( collection->size() != 3 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 3 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      // Note that this certificate is provided by the IHO, and is different to that used below
      manager = new TSLS63MediaIngestManager( dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        manager->destroy();
        collection->destroy();
        delete dataClient;
        return false;
      }

      if ( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        delete dataClient;
        return false;
      }      

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 5B SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // The exchange set should be ingested successfully, but should raise SSE 26 on the data client
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }

      //Destroy the SERIAL.ENC file and manager
      file->destroy();
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


    bool S63ENCAuthenticationTests_Part2::loadTest5c( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath )
    {
      // Load the configuration data for test 5C
      // The config section is set by the caller
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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/5 Authentication_Part2/Test 5c/" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 5c
      ////////////////////////////////////////////////////////////////////////////////////

      // dataClient will be deleted by the caller
      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), dataClient );

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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      // Note that this certificate is provided by the IHO, and is different to that used below
      // manager will be deleted by the caller
      manager = new TSLS63MediaIngestManager( dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      if ( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }      

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      // file will be deleted by the caller
      file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 5C SERIAL.ENC", TSLErrorCategoryInfo);
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // SSE 09 will be raised on the data client for cell GBK01620
      // Call GBK01640 will load without error
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCAuthenticationTests_Part2::test5c()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST5C");

      std::string firstError;

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      const char* dataStorePath("OEM_1_2/test5c");

      // Load the configuration data for test 5C
      // and run the test
      bool result( loadTest5c( file, manager, dataClient, dataStorePath) );

      //Destroy the SERIAL.ENC file and manager
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
    

    bool S63ENCAuthenticationTests_Part2::loadTest5d( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath )
    {
      // Load the 5c test
      if ( !loadTest5c( file, manager, dataClient, dataStorePath) )
      {
        TSLErrorStack::addError(0, "Initial test setup is invalid, as test 5c failed", TSLErrorCategoryInfo);
        // Caller will handle cleanup
        return false;
      }
      TSLErrorStack::addError(0, "TEST 5D START", TSLErrorCategoryInfo);
      // Ensure the calls to the data client were correct
      if( dataClient->anyErrorSoFar() == false )
      {
        TSLErrorStack::addError(0, "Initial test setup is invalid, as test 5c failed", TSLErrorCategoryInfo);
        return false;
      }

      //Destroy the SERIAL.ENC file
      file->destroy();
      file = NULL;

      // Load the configuration data for test 5D
      // The config section is set by the caller
      char buffer[1024];
      if( !TSLProfileHelper::lookupProfile("productsfile5D", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile5D", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificate5D", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      // Set the data exchange location as 5D
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/5 Authentication_Part2/Test 5d/" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 5d
      ////////////////////////////////////////////////////////////////////////////////////

      //Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 5D SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      
      // Set the data exchange location as 5D
      dataClient->setExchangeSetPrefix( dataClientExchangeSetPrefix );

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // Cell GBK01620 will be ingested without error
      // Cell GBK01660 will raise SSE 24 on the data client.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }

      return true;
    }


    bool S63ENCAuthenticationTests_Part2::test5d()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST5D");

      std::string firstError;

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      const char* dataStorePath("OEM_1_2/test5d");

      // Load the 5d test. Will also load 5c
      bool result( loadTest5d( file, manager, dataClient, dataStorePath) );

      //Destroy the SERIAL.ENC file and manager
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


    bool S63ENCAuthenticationTests_Part2::test5e()
    {
      // Set test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST5E");

      std::string firstError;

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      const char* dataStorePath("OEM_1_2/test5e");

      // The test requires it starts in the state as at the end of test 5d
      // so first load and execute that test.
      // Will also load 5c
      if ( !loadTest5d( file, manager, dataClient, dataStorePath) )
      {
        TSLErrorStack::addError(0, "Initial test setup invalid, as test 5d failed", TSLErrorCategoryInfo);
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

      // Ensure the calls to the data client were correct
      if( dataClient->anyErrorSoFar() == false )
      {
        TSLErrorStack::addError(0, "Initial test setup is invalid, as test 5d failed", TSLErrorCategoryInfo);
        return false;
      }

      // File no longer needed
      file->destroy();
      file = NULL;

      // Load the configuation data for 5e specific parts
      // of this test. 
      char buffer[1024];
      if( !TSLProfileHelper::lookupProfile("productsfile5E", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile5E' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("permit5E", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit5E' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string permit( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("serialfile5E", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile5E' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("certificate5E", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate5E' from config", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      // Set the data exchange location as 5E
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/5 Authentication_Part2/Test 5e/" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());


      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 5e
      ////////////////////////////////////////////////////////////////////////////////////

      // Set the data clients exchange set prefix
      dataClient->setExchangeSetPrefix( dataClientExchangeSetPrefix );

      // Load the cell permits for test 5e
      TSLS63CellPermitCollection* collection( TSLS63CellPermitCollection::create( permit.c_str(), dataClient ) );
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load 5e PERMIT.TXT", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      
      if( !manager->addCellPermits(collection) )
      {
        TSLErrorStack::addError(0, "Failed to add 5e permits to manager", TSLErrorCategoryInfo);
        manager->destroy();
        collection->destroy();
        delete dataClient;
        return false;
      }
      collection->destroy();

      //Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 5E SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
    
      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // This will raise SSE 09 for cell GB301820
      // and SSE 24 for cell GB301860
      // A sequential update error may also be thrown, as permitted by the test definition.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }

      //Destroy the SERIAL.ENC file and manager
      file->destroy();
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

    bool S63ENCAuthenticationTests_Part2::test5f()
    {
      // Set test-specific config 
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST5F");

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);

      // Load the configuation data
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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/5 Authentication_Part2/Test 5f/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test5f";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 5f
      ////////////////////////////////////////////////////////////////////////////////////

      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load PERMIT.TXT 
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      // Permit should contain 7 valid cell permits
      if( collection->size() != 7 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 7 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      // Note that this certificate is provided by the IHO, and is different to that used below
      manager = new TSLS63MediaIngestManager( dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        delete dataClient;
        return false;
      }

      if ( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        delete dataClient;
        return false;
      }      

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );

      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load 5F SERIAL.ENC", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the exchange set
      //
      // The 7 cells should load without any errors/warnings
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }

      //Destroy the SERIAL.ENC file and manager
      file->destroy();
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
  };
};