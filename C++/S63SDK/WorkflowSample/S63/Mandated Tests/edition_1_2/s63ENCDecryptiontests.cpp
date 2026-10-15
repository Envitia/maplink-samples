//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63ENCDecryptiontests.h"
#include "scripteds63dataclient.h"

#include <sstream>

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    S63ENCDecryptionTests::S63ENCDecryptionTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
    {
      m_dataLocation = config.m_testDataLocation;
      m_configDir = config.m_configurationLocation;
      m_configFile = m_configDir + "s63ENCDecryptiontests.ini";
      m_scriptsDir = m_configDir + "../scripts/";

      readConfiguration(m_configFile); 
    }

    S63ENCDecryptionTests::~S63ENCDecryptionTests()
    {
    }

    void S63ENCDecryptionTests::readConfiguration(const string &configFile)
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

    bool S63ENCDecryptionTests::test6a()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST6A");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/6 ENC Decryption/Test 6a/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test6a";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 6a
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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
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
        manager->destroy();
        return false;
      }

      // Load the exchange set
      // SSE 15 will be logged on the data client when the cell is ingested.
      if ( manager->loadExchangeSet( productsFile.c_str(), file, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        return false;
      }
      //Destroy the SERIAL.ENC file
      file->destroy();

      ////////////////////////////////////////////////////////////////////////////////////
      // This test verifies that the system raises both SSE15 and SSE25
      // These errors will be passed to the data client, to be handled by the application
      // SSE15 will be raised both when ingesting the data, and when loading it into
      // the TSLS63DataLayer.
      // SSE25 will be raised when loading the TSLS63DataLayer loads the cell for display.
      //
      // MapLink does not automatically display a 'Not for navigation' warning, but 
      // provides the mechanism for the application to do so if SSE25 is raised.
      // 
      // In the S63 workflow sample a custom data layer is enabled if SSE25 is raised,
      // which draws a watermark over the relevant cells.
      ////////////////////////////////////////////////////////////////////////////////////

      // Construct the S63 datalayer
      TSLS63DataLayerScaleBands* bands = createScaleBands();
      TSLS63DataLayer* dataLayer = new TSLS63DataLayer(&dataClient, &hardwareID, m_csys, bands);
      // Add the cell permits to the data layer
      for( unsigned int i(0); i < manager->numberOfDataServers(); ++i )
      {
        TSLSimpleString dsID;
        if( manager->queryDataServer(i, dsID) )
        {
          for( unsigned int j(0); j < manager->numberOfCellPermits(dsID.c_str()); ++j )
          {
            const TSLS63CellPermit* permit( manager->queryCellPermit(j, dsID.c_str()) );
            // We're not actually drawing the data, so the extent doesn't matter
            TSLGeodeticExtent extent(0.0, 0.0, 0.0, 0.0);
            if( !dataLayer->addCellPermit(permit, extent) )
            {
              continue;
            }
          }
        }
      }

      createMapLinkSurface();
      setupFeatureRendering(m_surface);
      if( !m_surface->addDataLayer(dataLayer, "s63Layer") )
      {
        bands->destroy();
        dataLayer->destroy();
        return false;
      }

      // This will trigger the loading of data from the intermediary data store, via the data client.
      // When the cells are loaded, SSE15 and SSE25 will be passed to the data client, to allow
      // the application to display the 'Not for Navigation' warning message.
      m_surface->reset(true);

      // Cleanup
      bands->destroy();
      m_surface->removeDataLayer("s63Layer");
      destroyMapLinkSurface();
      dataLayer->destroy();
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

    bool S63ENCDecryptionTests::test6b()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST6B");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/6 ENC Decryption/Test 6b/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test6b";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 6b
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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
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
        manager->destroy();
        return false;
      }

      // Load the exchange set
      // SSE20 will be logged on the data client when the cells are ingested
      if ( manager->loadExchangeSet( productsFile.c_str(), file, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
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

    bool S63ENCDecryptionTests::test6c()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST6C");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/6 ENC Decryption/Test 6c/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test6c";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 6c
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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
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
        manager->destroy();
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // This test verifies that the system raises SSE21,
      // and that the number of successful/unsuccessful imports can be reported.
      //
      // The SSE21 errors will be passed to the data client, to be handled by the application.
      // TSLS63DataClient::storeIngestedCell will be called for each cell ingested. 
      // The data client implementation should keep track of the ingest process, and report to the user as required.
      ////////////////////////////////////////////////////////////////////////////////////

      // Load the exchange set
      // Cells GB58932A and GB58932B will raise SSE21 on the data client.
      // They may also raise SSE23, when attempting to ingest a subsequent update.
      // All other cells will be imported without error.
      if ( manager->loadExchangeSet( productsFile.c_str(), file, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
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

    bool S63ENCDecryptionTests::test6d()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST6D");
      char buffer[1024];

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificate( m_dataLocation + buffer );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/6 ENC Decryption/Test 6d/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test6d";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 6d
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
      
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Construct the media ingest manager
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificate.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      if( !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
        collection->destroy();
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
        manager->destroy();
        return false;
      }

      // Load the exchange set
      // SSE16 will be raised on the data client when ingesting an update with invalid CRC
      // SSE23 will be raised on the data client when ingesting the next update, due to the previous failure
      if ( manager->loadExchangeSet( productsFile.c_str(), file, certificate.c_str() ) == false )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
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
  };
};