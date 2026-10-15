//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63ENCLicencingtests.h"
#include "scripteds63dataclient.h"

#include <sstream>

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    S63ENCLicencingTests::S63ENCLicencingTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
    {
      m_dataLocation = config.m_testDataLocation;
      m_configDir = config.m_configurationLocation;
      m_configFile = m_configDir + "s63ENCLicencingtests.ini";
      m_scriptsDir = m_configDir + "../scripts/";

      m_certificatePath = m_dataLocation + "/4 Authentication_Part1/Test 4b/IHO.CRT";

      readConfiguration(m_configFile); 
    }

    S63ENCLicencingTests::~S63ENCLicencingTests()
    {
    }

    void S63ENCLicencingTests::readConfiguration(const string &configFile)
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

    bool S63ENCLicencingTests::test2a()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2A");
      char buffer[1024];
      // The list of permit files to attempt to load
      std::string script;
      if( !TSLProfileHelper::lookupProfile("permit1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit1( m_dataLocation + buffer);
      if( !TSLProfileHelper::lookupProfile("permit2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit2(m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      script = m_scriptsDir + buffer;
      std::string dataStore = m_resultsPath + "/test2a";

      std::string firstError;

      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2a
      ////////////////////////////////////////////////////////////////////////////////////

      // Construct an implementation of TSLS63DataClient that operates from an
      // ini file. The ini instructs which calls should be received, the order
      // that they should be received and the responses that should be returned.
      ScriptedS63DataClient dataClient(script, m_dataLocation, dataStore, m_csys, false);

      // Load PERMIT.TXT - No cell permits listed
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit1.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // The permit file shouldn't contain any cell permits
      if( collection->size() != 0 )
      {
        TSLErrorStack::addError(0, "Permit collection contained cell permits, when none were specified in PERMIT.TXT", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      // First permit ingest should have worked
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      collection->destroy();

      // Try to ingest the non compliant text file
      collection = TSLS63CellPermitCollection::create(permit2.c_str(), &dataClient );
      if( collection )
      {
        TSLErrorStack::addError(0, "Managed to load non-compliant text file", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      // Check that the required errors were raised
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      // Test has passed
      TSLErrorStack::addError(0, "Note: The definition of this test specifies that 'SSE11 - Cell permit not found' should be returned when trying to load TEXT.TXT", TSLErrorCategoryInfo);
      TSLErrorStack::addError(0, "      MapLink instead returns 'SSE12 - Cell permit format is incorrect' instead. The cell permit can be found, but is not a valid cell permit file.", TSLErrorCategoryInfo);

      return true;
    }

    bool S63ENCLicencingTests::test2b()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2B");
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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2b/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2b";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2b
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

      // Permit should contain 1 valid cell permit
      if( collection->size() != 1 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 1 cell permit", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      const TSLS63CellPermit* cellPermit = collection->operator[](0);
      if( !cellPermit )
      {
        TSLErrorStack::addError(0, "Failed to query cell permit from collection", TSLErrorCategoryInfo);
        return false;
      }
      if( std::string("GB100002") != cellPermit->cellName() )
      {
        TSLErrorStack::addError(0, "Cell permit name incorrect", TSLErrorCategoryInfo);
        return false;
      }
      
      // First permit ingest should have worked
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
      // passing in the data client, the path to the SA's certificate
      // and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      // Ask the data client if all the callbacks to far have been as expected
      if ( !dataClient.anyErrorSoFar(&firstError) )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      // Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      // Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialFile.c_str() );
      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        return false;
      }

      // Load the exchange set
      // GB100012 - Should not be loaded, due to the invalid permit
      // GB100002 and updates - Should be loaded
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, m_certificatePath.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        return false;
      }

      //Destroy the SERIAL.ENC file and manager
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

    bool S63ENCLicencingTests::test2c()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2C");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("permit1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit1( m_dataLocation + buffer);
      
      if( !TSLProfileHelper::lookupProfile("permit2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit2( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );
      
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2c/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2c";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2c
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load the first permit
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit1.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // The permit collection should contain 2 permits,
      // as the format of PERMIT.TXT is valid
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "PERMIT.TXT didn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Add the cell permits to the ingest manager
      // The manager should log SSE13, for cell GB100001, and return false.
      // The permit for GB100002 should have been added.
      if( manager->addCellPermits( collection, m_dsID.c_str() ) == true )
      {
        TSLErrorStack::addError(0, "addCellPermits returned true when given an invalid cell permit", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }
      // Permits have been added to the manager, collection is no longer needed
      collection->destroy();

      // One of the permits was valid, check that it was added
      if( manager->numberOfCellPermits("GB") != 1 )
      {
        TSLErrorStack::addError(0, "No cell permits were added to manager", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      const TSLS63CellPermit* permit( manager->queryCellPermit(0, "GB") );
      if( !permit )
      {
        TSLErrorStack::addError(0, "Failed to query cell permit from manager", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if( std::string("GB100002") != permit->cellName() )
      {
        TSLErrorStack::addError(0, "Valid cell permit not added to manager", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      // Make sure the data client recieved SSE13
      if( dataClient.anyErrorSoFar(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Cleanup for the next test
      manager->destroy();

      // Load the second permit
      collection = TSLS63CellPermitCollection::create( permit2.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      // The permit collection should contain 2 valid permits,
      // as the format of PERMIT.TXT is valid
      if( collection->size() != 2 )
      {
        TSLErrorStack::addError(0, "PERMIT.TXT didn't contain 2 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      //Construct the media ingest manager
      manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Add the cell permits to the ingest manager
      // The manager should log SSE13, for cell GB100001, and return false
      // The permit for GB100002 should have been added however.
      if( manager->addCellPermits( collection, m_dsID.c_str() ) == true )
      {
        TSLErrorStack::addError(0, "addCellPermits returned true when given an invalid cell permit", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      // The permits have been added to the manager, collection is no longer needed
      collection->destroy();

      // One of the permits was valid, check that it was added
      if( manager->numberOfCellPermits("GB") != 1 )
      {
        TSLErrorStack::addError(0, "No cell permits were added to manager", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      
      permit = manager->queryCellPermit(0, "GB");
      if( !permit )
      {
        TSLErrorStack::addError(0, "Failed to query cell permit from manager", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }
      if( std::string("GB100002") != permit->cellName() )
      {
        TSLErrorStack::addError(0, "Valid cell permit not added to manager", TSLErrorCategoryInfo);
        manager->destroy();
        return false;
      }

      manager->destroy();

      // Make sure the data client recieved SSE13
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCLicencingTests::test2d()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2D");
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
      
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2d/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2d";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2d
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load the permits
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // The S63 SDK can be used to generate SSE20 in 2 ways.
      // 1. When an exchange set is ingested using the ingest manager, the date will be checked, 
      //    and SSE20 will be passed to the data client.
      // 2. The application can query the cell permits from the collection/ingest manager, 
      //    and raise the error.
      //
      // The test definition states that the system must raise SSE20 when installing the
      // cell permits. As such this test uses method 2.
      ////////////////////////////////////////////////////////////////////////////////////
      const unsigned int thirtyDaysInSeconds( 2592000 );
      for( unsigned int i(0); i < collection->size(); ++i )
      {
        const TSLS63CellPermit* permit( collection->operator[](i) );
        if( !permit )
        {
          TSLErrorStack::addError(0, "Failed to query permit from collection", TSLErrorCategoryInfo);
          collection->destroy();
          return false;
        }
        TSLTimeType expiryDate( permit->expiryDate() );
        if( (dataClient.currentTime() + thirtyDaysInSeconds) > expiryDate )
        {
          dataClient.notifyError(TSLS63ErrorCodeSSE20, permit->cellName(), permit->dataServerID(), 
            "Subscription service will expire in less than 30 days. Please contact your data supplier to renew the subscription licence.");
        }
      }

      // Cleanup
      collection->destroy();

      // Make sure the data client recieved SSE20
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCLicencingTests::test2e()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2E");
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
      
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2e/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2e";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2e
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load the first permit
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // The S63 SDK can be used to generate SSE15 in 2 ways.
      // 1. When an exchange set is ingested using the ingest manager, the date will be checked, 
      //    and SSE15 will be passed to the data client. This does not prevent the S63 SDK
      //    from ingesting an exchange set.
      // 2. The application can query the cell permits from the collection/ingest manager, 
      //    and raise the error.
      //
      // The test definition states that the system must raise SSE15 when installing the
      // cell permits. As such this test uses method 2.
      //
      // The test definition states that the system must display a permanent warning when
      // displaying a cell with an expired permit. MapLink will not automatically do this
      // but will trigger the S63 mandated errors such that the application can respond
      // accordingly.
      // In the S63 workflow sample the permanent warning is implemented as a
      // custom data layer, which is enabled if SSE15 is raised.
      ////////////////////////////////////////////////////////////////////////////////////
      for( unsigned int i(0); i < collection->size(); ++i )
      {
        const TSLS63CellPermit* permit( collection->operator[](i) );
        if( !permit )
        {
          TSLErrorStack::addError(0, "Failed to query permit from collection", TSLErrorCategoryInfo);
          collection->destroy();
          return false;
        }
        TSLTimeType expiryDate( permit->expiryDate() );
        if( dataClient.currentTime() > expiryDate )
        {
          dataClient.notifyError(TSLS63ErrorCodeSSE15, permit->cellName(), permit->dataServerID(),
            "Subscription service has expired. Please contact your data supplier to renew the subscription licence.");
        }
      }

      // Cleanup
      collection->destroy();

      // Make sure the data client recieved SSE15
      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCLicencingTests::test2f()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2F");
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
      
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2f/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2f";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2f
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load the first permit
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );

      // Failed to load PERMIT.TXT
      if( !collection )
      {
        TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // This test is meant to verify that the system displays some form of dialog/log to the user
      // when a permit is ingested successfully.
      // MapLink does not perform this alert itself, but exposes the functionality to 
      // inspect the loaded cell permits, such that the application can display a dialog.
      //
      // This test simply checks that the permits were loaded successfully, and prints
      // their details to the log.
      ////////////////////////////////////////////////////////////////////////////////////
      if( collection->size() != 10 )
      {
        TSLErrorStack::addError(0, "Incorrect number of cell permits loaded from PERMIT.TXT", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      //Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Add the cell permits to the ingest manager
      if( manager->addCellPermits( collection, m_dsID.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
        manager->destroy();
        return false;
      }

      // The permits have been loaded, and added to the ingest manager
      // Print the details of the ingested cell permits to the log
      for( unsigned int i(0); i < collection->size(); ++i )
      {
        const TSLS63CellPermit* permit( collection->operator[](i) );
        if( !permit )
        {
          TSLErrorStack::addError(0, "Failed to query permit from collection", TSLErrorCategoryInfo);
          collection->destroy();
          manager->destroy();
          return false;
        }
        std::stringstream str;
        str << "Ingested cell permit: " << permit->cellName() << " for data server: " << permit->dataServerID() << 
          " with expiry: " << permit->expiryDateAsString();
        TSLErrorStack::addError(0, str.str().c_str(), TSLErrorCategoryInfo);
      }

      // Cleanup
      collection->destroy();
      manager->destroy();

      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCLicencingTests::test2g()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2G");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("permit1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit1( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("permit2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit2( m_dataLocation + buffer );
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );
      
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2g/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2g";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2g
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load the first permit
      TSLS63CellPermitCollection * collection1 = TSLS63CellPermitCollection::create( permit1.c_str(), &dataClient );
      // Failed to load PERMIT.TXT
      if( !collection1 )
      {
        TSLErrorStack::addError(0, "Failed to load first PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      if( collection1->size() != 4 )
      {
        TSLErrorStack::addError(0, "Incorrect number of cell permits loaded from first PERMIT.TXT", TSLErrorCategoryInfo);
        collection1->destroy();
        return false;
      }
      
      // Load the second permit
      TSLS63CellPermitCollection * collection2 = TSLS63CellPermitCollection::create( permit2.c_str(), &dataClient );
      // Failed to load PERMIT.TXT
      if( !collection2 )
      {
        TSLErrorStack::addError(0, "Failed to load second PERMIT.TXT", TSLErrorCategoryInfo);
        collection1->destroy();
        return false;
      }
      if( collection2->size() != 4 )
      {
        TSLErrorStack::addError(0, "Incorrect number of cell permits loaded from second PERMIT.TXT", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // The test definition specifies that the system must manage permits supplied
      // from multiple data servers.
      // It also specifies that the user must be able to view the permits and their contents.
      // 
      // This test checks that the permits are loaded successfully into the S63 SDK, 
      // and prints the data server/cell permit information to the log.
      ////////////////////////////////////////////////////////////////////////////////////

      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Add the cell permits to the ingest manager
      if( (manager->addCellPermits( collection1, m_dsID.c_str() ) == false) ||
          (manager->addCellPermits( collection2, m_dsID.c_str() ) == false) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
        return false;
      }

      // Permits have been added to the manager, no longer needed
      collection1->destroy();
      collection2->destroy();

      // The permits have been loaded, and added to the ingest manager
      // Print the details of the ingested cell permits to the log
      for( unsigned int i(0); i < manager->numberOfDataServers(); ++i )
      {
        TSLSimpleString dsID;
        if( manager->queryDataServer(i, dsID) == false )
        {
          TSLErrorStack::addError(0, "Failed to query data server ID from manager", TSLErrorCategoryInfo);
          manager->destroy();
          return false;
        }
        for( unsigned int i(0); i < manager->numberOfCellPermits(dsID.c_str()); ++i )
        {
          const TSLS63CellPermit* permit( manager->queryCellPermit(i, dsID.c_str()) );
          if( !permit )
          {
            TSLErrorStack::addError(0, "Failed to query permit from collection", TSLErrorCategoryInfo);
            manager->destroy();
            return false;
          }
          std::stringstream str;
          str << "Ingested cell permit: " << permit->cellName() << " for data server: " << permit->dataServerID() << 
            " with expiry: " << permit->expiryDateAsString();
          TSLErrorStack::addError(0, str.str().c_str(), TSLErrorCategoryInfo);
        }
      }

      // Cleanup
      manager->destroy();

      if( dataClient.didItWork(&firstError) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }

    bool S63ENCLicencingTests::test2h()
    {
      // Read test-specific config
      // Note: This test uses the same data as Test 2g
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST2H");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("permit1", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit1' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit1( m_dataLocation + buffer);

      if( !TSLProfileHelper::lookupProfile("permit2", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'permit2' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string permit2( m_dataLocation + buffer );
      
      if( !TSLProfileHelper::lookupProfile("script", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'script' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string script( m_scriptsDir + buffer );
      
      // Note: This test uses the same data as Test 2g
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/2 ENC Licencing/Test 2g/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test2h";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 2h
      ////////////////////////////////////////////////////////////////////////////////////

      ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

      // Load the first permit
      TSLS63CellPermitCollection * collection1 = TSLS63CellPermitCollection::create( permit1.c_str(), &dataClient );
      // Failed to load PERMIT.TXT
      if( !collection1 )
      {
        TSLErrorStack::addError(0, "Failed to load first PERMIT.TXT", TSLErrorCategoryInfo);
        return false;
      }

      if( collection1->size() != 4 )
      {
        TSLErrorStack::addError(0, "Incorrect number of cell permits loaded from first PERMIT.TXT", TSLErrorCategoryInfo);
        collection1->destroy();
        return false;
      }
      
      // Load the second permit
      TSLS63CellPermitCollection * collection2 = TSLS63CellPermitCollection::create( permit2.c_str(), &dataClient );
      // Failed to load PERMIT.TXT
      if( !collection2 )
      {
        TSLErrorStack::addError(0, "Failed to load second PERMIT.TXT", TSLErrorCategoryInfo);
        collection1->destroy();
        return false;
      }
      if( collection2->size() != 4 )
      {
        TSLErrorStack::addError(0, "Incorrect number of cell permits loaded from second PERMIT.TXT", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // This test verifies that the system allows the user to delete cell permits.
      // 
      // MapLink allows this via both the TSLS63CellPermitCollection, and 
      // TSLS63MediaIngestManager
      //
      // This test checks that the permits are loaded successfully, and prints the
      // data server/cell permit information to the log.
      ////////////////////////////////////////////////////////////////////////////////////

      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Remove a cell permit from the collection before adding to the manager
      collection1->erase(0);
      if( collection1->size() != 3 )
      {
        TSLErrorStack::addError(0, "Failed to remove cell permit from TSLS63CellPermitCollection", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
        return false;
      }

      // Add the cell permits to the ingest manager
      if( (manager->addCellPermits( collection1, m_dsID.c_str() ) == false) ||
          (manager->addCellPermits( collection2, m_dsID.c_str() ) == false) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
        return false;
      }

      if( manager->numberOfDataServers() != 2 )
      {
        TSLErrorStack::addError(0, "Manager doesn't contain cell permits for 2 data servers", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      // Demonstrate removal of all cell permits for data server 'GB'
      if( manager->numberOfCellPermits("GB") != 3 )
      {
        TSLErrorStack::addError(0, "Manager contains the wrong number of cell permits for data server 'GB'", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      if( manager->removeDataServer("GB") == false )
      {
        TSLErrorStack::addError(0, "Failed to remove all cell permits for data server 'GB' ", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      if( manager->numberOfCellPermits("GB") != 0 )
      {
        TSLErrorStack::addError(0, "Manager contains cell permits for data server 'GB' after removal", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      // Demonstrate removal of one cell permit for the data server 'GB'
      if( manager->numberOfCellPermits("PM") != 4 )
      {
        TSLErrorStack::addError(0, "Manager contains the wrong number of cell permits for data server 'PM'", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      const TSLS63CellPermit* permit( manager->queryCellPermit(0, "PM") );
      if( permit == NULL )
      {
        TSLErrorStack::addError(0, "Failed to query first cell permit for data server 'PM", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      if( manager->removeCellPermit("PM", permit->cellName()) == false )
      {
        TSLErrorStack::addError(0, "Failed to remove cell permit from manager for data server 'PM'", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }
      
      if( manager->numberOfCellPermits("PM") != 3 )
      {
        TSLErrorStack::addError(0, "Manager contains the wrong number of cell permits for data server 'PM' after removal", TSLErrorCategoryInfo);
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
      }

      // Cleanup
      collection1->destroy();
      collection2->destroy();
      manager->destroy();

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