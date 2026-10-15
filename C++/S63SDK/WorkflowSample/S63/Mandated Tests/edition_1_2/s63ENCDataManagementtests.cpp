//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63ENCDataManagementtests.h"
#include "scripteds63dataclient.h"

#include <sstream>

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    S63ENCDataManagementTests::S63ENCDataManagementTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
    {
      m_dataLocation = config.m_testDataLocation;
      m_configDir = config.m_configurationLocation;
      m_configFile = m_configDir + "s63ENCDataManagementtests.ini";
      m_scriptsDir = m_configDir + "../scripts/";

      readConfiguration(m_configFile); 
    }

    S63ENCDataManagementTests::~S63ENCDataManagementTests()
    {
    }

    void S63ENCDataManagementTests::readConfiguration(const string &configFile)
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

    bool S63ENCDataManagementTests::test7a()
    {
      TSLS63MediaIngestManager* manager(NULL);
      ScriptedS63DataClient* dataClient;

      bool result = loadTest7a(manager, dataClient, "TEST7A", "OEM_1_2/test7a");

      if( manager )
      {
        manager->destroy();
      }
      if( dataClient )
      {
        delete dataClient;
      }
      return result;
    }

    bool S63ENCDataManagementTests::loadTest7a( TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* configSection, const char* dataStorePath )
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection(configSection);
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7a" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7a
      // The data client and manager will be deleted by the caller
      ////////////////////////////////////////////////////////////////////////////////////

      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      manager = new TSLS63MediaIngestManager( dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);


      // Add the cell permits for data server 1
      TSLS63CellPermitCollection * collectionDS1 = TSLS63CellPermitCollection::create( permitDS1.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collectionDS1 )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }
      if( !manager->addCellPermits( collectionDS1, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collectionDS1->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collectionDS1->destroy();

      // Add the cell permits for data server 2
      TSLS63CellPermitCollection * collectionDS2 = TSLS63CellPermitCollection::create( permitDS2.c_str(), dataClient );

      // Failed to load PERMIT.TXT
      if( !collectionDS2 )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }
      if( !manager->addCellPermits( collectionDS2, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collectionDS2->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collectionDS2->destroy();


      // Load the exchange set for data server 1
      TSLS63SerialENCFile *fileDS1 = TSLS63SerialENCFile::create( serialFileDS1.c_str() );
      if( !fileDS1 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        return false;
      }
      if ( manager->loadExchangeSet( productsFileDS1.c_str(), fileDS1, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        fileDS1->destroy();
        return false;
      }
      //Destroy the SERIAL.ENC file
      fileDS1->destroy();


      // Load the exchange set for data server 2
      TSLS63SerialENCFile *fileDS2 = TSLS63SerialENCFile::create( serialFileDS2.c_str() );
      if( !fileDS2 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        return false;
      }
      if ( manager->loadExchangeSet( productsFileDS2.c_str(), fileDS2, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        fileDS2->destroy();
        return false;
      }
      //Destroy the SERIAL.ENC file
      fileDS2->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      // Ensure the ingest state matches the test definition
      if( dataClient->numIngestedCells() != 7 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }

      int edition(0);
      int update(0);
      if( !dataClient->getIngestedCellInfo("GB281600", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB281600' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB281800", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB281800' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB282000", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB282000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283000", edition, update) ||
           edition != 1 ||
           update != 4 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283100", edition, update) ||
           edition != 1 ||
           update != 3 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283100' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283200", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283200' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283300", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283300' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCDataManagementTests::test7b()
    {
      TSLS63MediaIngestManager* manager(NULL);
      ScriptedS63DataClient* dataClient;

      // Test 7b requires that test 7a has been run first, 
      // in order to setup the initial test environment.
      const char* configSection("Test7B");
      const char* dataStorePath("OEM_1_2/test7b");
      if( !loadTest7a(manager, dataClient, configSection, dataStorePath ) )
      {  
        if( manager )
        {
          manager->destroy();
        }
        if( dataClient )
        {
          delete dataClient;
        }
        TSLErrorStack::addError(0, "Test environment setup failed, because test 7a failed", TSLErrorCategoryInfo);
        return false;
      }


      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection(configSection);
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("permit", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7b" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7b
      ////////////////////////////////////////////////////////////////////////////////////

      // Data from this point onwards is relative to the test 7b directory
      dataClient->setExchangeSetPrefix(dataClientExchangeSetPrefix);

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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        manager->destroy();
        return false;
      }

      // Cell permits have been added to the manager, collection is no longer needed
      collection->destroy();

      // Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialFile.c_str() );
      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }

      // Load the exchange set
      // This exchange set will be merged with the existing data from test 7a
      if ( manager->loadExchangeSet( productsFile.c_str(), file, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        file->destroy();
        return false;
      }

      // Cleanup
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


      // Ensure the ingest state matches the test definition
      // 7 cells from test 7a, plus 2 new cells from 7b
      if( dataClient->numIngestedCells() != 9 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      int edition(0);
      int update(0);
      if( !dataClient->getIngestedCellInfo("GB255000", edition, update) ||
           edition != 3 ||
           update != 3 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB255000' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB270000", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB270000' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB281600", edition, update) ||
           edition != 1 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB281600' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB281800", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB281800' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB282000", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB282000' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283000", edition, update) ||
           edition != 1 ||
           update != 4 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283000' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283100", edition, update) ||
           edition != 1 ||
           update != 3 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283100' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283200", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283200' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB283300", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB283300' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      
      delete dataClient;
      return true;
    }

    bool S63ENCDataManagementTests::test7c()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST7C");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7c" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test7c";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7c
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      TSLS63MediaIngestManager* manager( new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID ) );
      manager->setProcessingFlags(TSLFilterProcessNone);
      
      // 1. Install the ENC permit file for DS1
      TSLS63CellPermitCollection* collectionDS1( TSLS63CellPermitCollection::create( permitDS1.c_str(), &dataClient ) );
      if( !collectionDS1 )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT for DS1", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      if( !manager->addCellPermits( collectionDS1, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits for DS1 to manager", TSLErrorCategoryInfo);
        collectionDS1->destroy();
        manager->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collectionDS1->destroy();
      
      // 2. Load the exchange set for DS1
      TSLS63SerialENCFile *fileDS1( TSLS63SerialENCFile::create( serialFileDS1.c_str() ) );
      if( !fileDS1 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for DS1", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileDS1.c_str(), fileDS1, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for DS1", TSLErrorCategoryInfo);
        fileDS1->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileDS1->destroy();

      // 3. Try to load the exchange set for DS2
      //    This should not ingest any cells, as the cell permits have not been installed
      TSLS63SerialENCFile *fileDS2 = TSLS63SerialENCFile::create( serialFileDS2.c_str() );
      if( !fileDS2 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for DS2", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      // The call to loadExchangeSet should return true, but not cells should be ingested.
      if ( manager->loadExchangeSet( productsFileDS2.c_str(), fileDS2, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Managed to load exchange set for DS2 before cell permits have been installed", TSLErrorCategoryInfo);
        fileDS2->destroy();
        manager->destroy();
        return false;
      }

      // 3a. Check the state of the data client, to ensure that:
      //     The ingest for DS1 was correct
      //     The ingest for DS2 did nothing
      if( dataClient.numIngestedCells() != 4 )
      {
        TSLErrorStack::addError(0, "3a. Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }
      int edition(0);
      int update(0);
      if( !dataClient.getIngestedCellInfo("GB281600", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "3a. State of cell 'GB281600' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB281800", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "3a. State of cell 'GB281800' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB282000", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "3a. State of cell 'GB282000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB283000", edition, update) ||
           edition != 1 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "3a. State of cell 'GB283000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
   
      // 4. Install the cell permits for DS2
      TSLS63CellPermitCollection* collectionDS2( TSLS63CellPermitCollection::create( permitDS2.c_str(), &dataClient ) );
      if( !collectionDS2 )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT for DS2", TSLErrorCategoryInfo);
        fileDS2->destroy();
        manager->destroy();
        return false;
      }
      if( !manager->addCellPermits( collectionDS2, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits for DS2 to manager", TSLErrorCategoryInfo);
        collectionDS2->destroy();
        fileDS2->destroy();
        manager->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collectionDS2->destroy();

      // 5. Load the exchange set for DS2
      //    Now that the cell permits have been installed this should succeed
      if ( manager->loadExchangeSet( productsFileDS2.c_str(), fileDS2, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for DS2", TSLErrorCategoryInfo);
        fileDS2->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileDS2->destroy();

      manager->destroy();

      
      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      // 6. Ensure the ingest state matches the test definition
      if( dataClient.numIngestedCells() != 6 )
      {
        TSLErrorStack::addError(0, "5b. Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB281600", edition, update) ||
           edition != 1 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "5b. State of cell 'GB281600' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB281800", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "5b. State of cell 'GB281800' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB282000", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "5b. State of cell 'GB282000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB283000", edition, update) ||
           edition != 1 ||
           update != 4 )
      {
        TSLErrorStack::addError(0, "5b. State of cell 'GB283000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB283100", edition, update) ||
           edition != 1 ||
           update != 3 )
      {
        TSLErrorStack::addError(0, "5b. State of cell 'GB283100' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB283200", edition, update) ||
           edition != 1 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "5b. State of cell 'GB283200' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }
  
    bool S63ENCDataManagementTests::test7d()
    {
      TSLS63MediaIngestManager* manager(NULL);
      ScriptedS63DataClient* dataClient;

      const char* configSection("TEST7D");
      const char* dataStorePath("OEM_1_2/test7d");
      bool result = loadTest7d(manager, dataClient, configSection, dataStorePath);

      if( manager )
      {
        manager->destroy();
      }
      if( dataClient )
      {
        delete dataClient;
      }
      return result;
    }

    bool S63ENCDataManagementTests::loadTest7d( TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* configSection, const char* dataStorePath )
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection(configSection);
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      if( !TSLProfileHelper::lookupProfile("productsfileBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileBase' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileBase( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileBase' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileBase( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfileUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileUpdate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileUpdate( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileUpdate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileUpdate( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7d" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7d
      ////////////////////////////////////////////////////////////////////////////////////

      // The data client and manager will be deleted by the caller
      dataClient = new ScriptedS63DataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      manager = new TSLS63MediaIngestManager( dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);
      
      // Install the ENC permit file
      TSLS63CellPermitCollection* collection( TSLS63CellPermitCollection::create( permit.c_str(), dataClient ) );
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collection->destroy();
      
      // Load the base exchange set
      TSLS63SerialENCFile *fileBase( TSLS63SerialENCFile::create( serialFileBase.c_str() ) );
      if( !fileBase )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for base exchange set", TSLErrorCategoryInfo);
        return false;
      }
      if ( manager->loadExchangeSet( productsFileBase.c_str(), fileBase, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for base exchange set", TSLErrorCategoryInfo);
        fileBase->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileBase->destroy();

      // Load the update exchange set
      TSLS63SerialENCFile *fileUpdate( TSLS63SerialENCFile::create( serialFileUpdate.c_str() ) );
      if( !fileUpdate )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for update exchange set", TSLErrorCategoryInfo);
        return false;
      }
      if ( manager->loadExchangeSet( productsFileUpdate.c_str(), fileUpdate, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for update exchange set", TSLErrorCategoryInfo);
        fileUpdate->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileUpdate->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient->anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      // Ensure the ingest state matches the test definition
      if( dataClient->numIngestedCells() != 4 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }
      int edition(0);
      int update(0);
      if( !dataClient->getIngestedCellInfo("GB251200", edition, update) ||
           edition != 1 ||
           update != 8 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB251200' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB255000", edition, update) ||
           edition != 3 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB255000' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      // This cell has been cancelled
      if( !dataClient->getIngestedCellInfo("GB280200", edition, update) ||
           edition != 0 ||
           update != -1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB280200' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB301620", edition, update) ||
           edition != 2 ||
           update != 4 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB301620' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
     
      return true;
    }

    bool S63ENCDataManagementTests::test7e()
    {
      TSLS63MediaIngestManager* manager(NULL);
      ScriptedS63DataClient* dataClient;

      // Test 7e requires that test 7d has been run first, 
      // in order to setup the initial test environment.
      const char* configSection("Test7E");
      const char* dataStorePath("OEM_1_2/test7e");
      if( !loadTest7d(manager, dataClient, configSection, dataStorePath ) )
      {  
        if( manager )
        {
          manager->destroy();
        }
        if( dataClient )
        {
          delete dataClient;
        }
        TSLErrorStack::addError(0, "Test environment setup failed, because test 7d failed", TSLErrorCategoryInfo);
        return false;
      }

      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection(configSection);
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("permit7e", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit7e' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string script( m_scriptsDir + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfile7eBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile7eBase' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string productsFileBase( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile7eBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile7eBase' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string serialFileBase( m_dataLocation + buffer );

            if( !TSLProfileHelper::lookupProfile("productsfile7eUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile7eUpdate' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string productsFileUpdate( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile7eUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile7eUpdate' from config", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      std::string serialFileUpdate( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7e" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7e
      ////////////////////////////////////////////////////////////////////////////////////

      // Data from this point onwards is relative to the test 7e directory
      dataClient->setExchangeSetPrefix(dataClientExchangeSetPrefix);

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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        delete dataClient;
        manager->destroy();
        return false;
      }

      // Cell permits have been added to the manager, collection is no longer needed
      collection->destroy();

      // Load the base exchange set
      TSLS63SerialENCFile *fileBase = TSLS63SerialENCFile::create( serialFileBase.c_str() );
      if( !fileBase )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for base exchange set", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileBase.c_str(), fileBase, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load base exchange set", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        fileBase->destroy();
        return false;
      }
      fileBase->destroy();

      // Load the update exchange set
      TSLS63SerialENCFile *fileUpdate = TSLS63SerialENCFile::create( serialFileUpdate.c_str() );
      if( !fileUpdate )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for update exchange set", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileUpdate.c_str(), fileUpdate, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load update exchange set", TSLErrorCategoryInfo);
        delete dataClient;
        manager->destroy();
        fileUpdate->destroy();
        return false;
      }
      fileUpdate->destroy();

      // Cleanup
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

      // Ensure the ingest state matches the test definition
      if( dataClient->numIngestedCells() != 9 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      int edition(0);
      int update(0);
      if( !dataClient->getIngestedCellInfo("GB251200", edition, update) ||
           edition != 1 ||
           update != 8 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB251200' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB255000", edition, update) ||
           edition != 3 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB255000' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      // Cell has been cancelled
      if( !dataClient->getIngestedCellInfo("GB280200", edition, update) ||
           edition != 0 ||
           update != -1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB280200' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB301620", edition, update) ||
           edition != 2 ||
           update != 4 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB301620' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      // Cell has been cancelled
      if( !dataClient->getIngestedCellInfo("GB380620", edition, update) ||
           edition != 0 ||
           update != -1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB380620' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      // Cell has been cancelled
      if( !dataClient->getIngestedCellInfo("GB380720", edition, update) ||
           edition != 0 ||
           update != -1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB380720' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB40162A", edition, update) ||
           edition != 9 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40162A' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB40162B", edition, update) ||
           edition != 2 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40162B' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB40182A", edition, update) ||
           edition != 1 ||
           update != 5 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40182A' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      delete dataClient;
      return true;
    }

    bool S63ENCDataManagementTests::test7f()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST7F");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      if( !TSLProfileHelper::lookupProfile("productsfileBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileBase' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileBase( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileBase' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileBase( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfileUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileUpdate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileUpdate( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileUpdate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileUpdate( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7f" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test7f";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7f
      ////////////////////////////////////////////////////////////////////////////////////

      // The data client and manager will be deleted by the caller
      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      TSLS63MediaIngestManager* manager = new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);
      
      // Install the ENC permit file
      TSLS63CellPermitCollection* collection( TSLS63CellPermitCollection::create( permit.c_str(), &dataClient ) );
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collection->destroy();
      
      // Load the base exchange set
      TSLS63SerialENCFile *fileBase( TSLS63SerialENCFile::create( serialFileBase.c_str() ) );
      if( !fileBase )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for base exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileBase.c_str(), fileBase, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for base exchange set", TSLErrorCategoryInfo);
        fileBase->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileBase->destroy();

      // Load the update exchange set
      TSLS63SerialENCFile *fileUpdate( TSLS63SerialENCFile::create( serialFileUpdate.c_str() ) );
      if( !fileUpdate )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for update exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileUpdate.c_str(), fileUpdate, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for update exchange set", TSLErrorCategoryInfo);
        fileUpdate->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileUpdate->destroy();

      // Cleanup
      manager->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      // Ensure the ingest state matches the test definition
      if( dataClient.numIngestedCells() != 2 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }
      int edition(0);
      int update(0);
      if( !dataClient.getIngestedCellInfo("GB303040", edition, update) ||
           edition != 11 ||
           update != 10 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB303040' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB50162D", edition, update) ||
           edition != 6 ||
           update != 6 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB50162D' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      
      return true;
    }

    bool S63ENCDataManagementTests::test7g()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST7G");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      // Base exchange set 1
      if( !TSLProfileHelper::lookupProfile("productsfileBase1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileBase1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileBase1( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileBase1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileBase1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileBase1( m_dataLocation + buffer );

      // Base exchange set 2
      if( !TSLProfileHelper::lookupProfile("productsfileBase2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileBase2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileBase2( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileBase2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileBase2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileBase2( m_dataLocation + buffer );

      // Base exchange set 3
      if( !TSLProfileHelper::lookupProfile("productsfileBase3", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileBase3' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileBase3( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileBase3", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileBase3' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileBase3( m_dataLocation + buffer );

      // Update exchange set
      if( !TSLProfileHelper::lookupProfile("productsfileUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfileUpdate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFileUpdate( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileUpdate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfileUpdate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFileUpdate( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/7 ENC Data Management/Test 7g" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test7g";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 7g
      ////////////////////////////////////////////////////////////////////////////////////
      // This test specifies that the base exchange sets, and updates be ingested, and
      // requires that the SENC state is correct at the end of the test.
      //
      // It also requires that a warning is displayed to the user, along the lines of
      // 'This update CD is not compatible with currently installed ENCs. 
      //  Please install 'Base CD <Number>, dated <date>' 
      //  and then continue with the update process'.
      // This requires that the system reads the STATUS.LST files from the base/update 
      // exchange sets, in order to manage the dependencies.
      //
      // Currently, MapLink does not provide a class for reading the STATUS.LST files.
      //
      // This test performs the exchange set ingests,
      // and ensures that the state matches the test definition.
      // The S63 ingest manager will correctly reject the updates which would
      // trigger the warning message, as the base exchange set doesn't contain
      // the prerequisites for the update.
      ////////////////////////////////////////////////////////////////////////////////////

      // Setup the data client and ingest manager
      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      TSLS63HWID hardwareID( m_hw_id.c_str() );

      TSLS63MediaIngestManager* manager = new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);
      
      // Install the ENC permit file
      TSLS63CellPermitCollection* collection( TSLS63CellPermitCollection::create( permit.c_str(), &dataClient ) );
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }
      // Cell permits have been added to the manager, collection is no longer needed
      collection->destroy();

      // Load base exchange set 1
      TSLS63SerialENCFile *fileBase1( TSLS63SerialENCFile::create( serialFileBase1.c_str() ) );
      if( !fileBase1 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for base exchange set 1", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileBase1.c_str(), fileBase1, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for base exchange set 1", TSLErrorCategoryInfo);
        fileBase1->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileBase1->destroy();
      
      // Load base exchange set 2
      TSLS63SerialENCFile *fileBase2( TSLS63SerialENCFile::create( serialFileBase2.c_str() ) );
      if( !fileBase2 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for base exchange set 2", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileBase2.c_str(), fileBase2, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for base exchange set 2", TSLErrorCategoryInfo);
        fileBase2->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileBase2->destroy();

      // Load base exchange set 3
      TSLS63SerialENCFile *fileBase3( TSLS63SerialENCFile::create( serialFileBase3.c_str() ) );
      if( !fileBase3 )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for base exchange set 3", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileBase3.c_str(), fileBase3, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for base exchange set 3", TSLErrorCategoryInfo);
        fileBase3->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileBase3->destroy();

      // Load the update exchange set
      TSLS63SerialENCFile *fileUpdate( TSLS63SerialENCFile::create( serialFileUpdate.c_str() ) );
      if( !fileUpdate )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC for update exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if ( manager->loadExchangeSet( productsFileUpdate.c_str(), fileUpdate, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set for update exchange set", TSLErrorCategoryInfo);
        fileUpdate->destroy();
        manager->destroy();
        return false;
      }
      // Destroy the SERIAL.ENC file
      fileUpdate->destroy();

      // Cleanup
      manager->destroy();

      // Ensure the calls to the data client were correct
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      // Ensure the ingest state matches the test definition
      if( dataClient.numIngestedCells() != 12 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }
      int edition(0);
      int update(0);
      if( !dataClient.getIngestedCellInfo("GB302840", edition, update) ||
           edition != 23 ||
           update != 4 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB302840' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB303220", edition, update) ||
           edition != 4 ||
           update != 7 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB303220' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB303420", edition, update) ||
           edition != 3 ||
           update != 12 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB303420' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB303460", edition, update) ||
           edition != 11 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB303460' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB40162A", edition, update) ||
           edition != 9 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40162A' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB40184A", edition, update) ||
           edition != 2 ||
           update != 3 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40184A' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB40186D", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40186D' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB40202A", edition, update) ||
           edition != 4 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40202A' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB50162B", edition, update) ||
           edition != 11 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB50162B' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB50162C", edition, update) ||
           edition != 9 ||
           update != 5 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB50162C' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB50162D", edition, update) ||
           edition != 5 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB50162D' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient.getIngestedCellInfo("GB50182A", edition, update) ||
           edition != 2 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB50162D' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      
      return true;
    }
  };
};