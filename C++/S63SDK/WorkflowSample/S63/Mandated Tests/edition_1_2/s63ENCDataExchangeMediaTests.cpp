//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "S63ENCDataExchangeMediaTests.h"
#include "scripteds63dataclient.h"

#include <string>
#include <vector>
#include <sstream>

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    S63ENCDataExchangeMediaTests::S63ENCDataExchangeMediaTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
    {
      m_dataLocation = config.m_testDataLocation;
      m_configDir = config.m_configurationLocation;
      m_configFile = m_configDir + "S63ENCDataExchangeMediaTests.ini";
      m_scriptsDir = m_configDir + "../scripts/";

      m_certificatePath = m_dataLocation + "/7 ENC Data Management/Test 7a/IHO.CRT";

      readConfiguration(m_configFile); 
    }

    S63ENCDataExchangeMediaTests::~S63ENCDataExchangeMediaTests()
    {
    }
 
    void S63ENCDataExchangeMediaTests::readConfiguration(const string &configFile)
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
    
    bool S63ENCDataExchangeMediaTests::test8a()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST8A");

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);

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
        return false;
      }
      std::string permit( m_dataLocation + buffer);

      // Multiple paths for PRODUCTS.TXT may be specified in the config
      // Please see the comments in S63ENDDataExchangeMediaTests.ini, and the test definition.
      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFile( buffer );

      int i(0);
      std::vector<std::string>  productLocations;
      while( TSLProfileHelper::lookupProfile("productLocation", buffer, 1024, "", i) )
      {
        productLocations.push_back(buffer);
        ++i;
      }
      if (productLocations.empty() )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsLocation' from config. Please see the comments in S63ENDDataExchangeMediaTests.ini", TSLErrorCategoryInfo);
        return false;
      }

      
      int tmp(0);
      TSLProfileHelper::lookupProfile("productLocationsAreAbsolute", &tmp, 0);
      bool productLocationsAreAbsolute( tmp );

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

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/8 Data Exchange Media/Test 8a/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test8a";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 8A
      ////////////////////////////////////////////////////////////////////////////////////
     
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Iterate through all the locations for the media specified in the config file.
      std::vector<std::string>::const_iterator it( productLocations.begin());
      std::vector<std::string>::const_iterator itE( productLocations.end());
      for( ; it != itE; ++it )
      {
        ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

        // Load PERMIT.TXT 
        TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );
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

        // Construct the media ingest manager
        manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
        manager->setProcessingFlags(TSLFilterProcessNone);

        if( !manager->addCellPermits(collection) )
        {
          TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
          collection->destroy();
          manager->destroy();
          return false;
        }

        //Destroy the collection of cell permits as the manager takes copies
        collection->destroy();

        //Load the SERIAL.ENC file
        file = TSLS63SerialENCFile::create( serialFile.c_str() );

        if( !file )
        {
          TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
          manager->destroy();
          return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Load the exchange set.
        //
        // It should load without error.
        // The products file location can be anywahere that is accessible, as defined by
        // the test including Hard-Drive, CD-ROM, DVD, USB Memory Stick, or any other appropriate
        // media
        ////////////////////////////////////////////////////////////////////////////////////

        std::string stProducts;
        if( productLocationsAreAbsolute )
        {
          stProducts = *it + productsFile;
        }
        else
        {
          stProducts = m_dataLocation + *it + productsFile;
        }

        if ( !manager->loadExchangeSet( stProducts.c_str(), file, certificateFile.c_str() ) )
        {
          std::stringstream str;
          str << "Failed to load exchange set from: " << stProducts;
          TSLErrorStack::addError(0, str.str().c_str(), TSLErrorCategoryInfo);
          manager->destroy();
          file->destroy();
          return false;
        }

        manager->destroy();
        file->destroy();

        // Ensure the calls to the data client were correct
        if( dataClient.didItWork(&firstError) == false )
        {
          std::string msg("Error from scripted data client: ");
          msg += firstError;
          TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
          return false;
        }
      }

      return true;
    }
  
    bool S63ENCDataExchangeMediaTests::test8b()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST8B");

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);

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
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      // Multiple paths for PRODUCTS.TXT may be specified in the config
      // Please see the comments in S63ENDDataExchangeMediaTests.ini, and the test definition.
      if( !TSLProfileHelper::lookupProfile("productsfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFile( buffer );

      int i(0);
      std::vector<std::string>  productLocations;
      while( TSLProfileHelper::lookupProfile("productLocation", buffer, 1024, "", i) )
      {
        productLocations.push_back(buffer);
        ++i;
      }
      if (productLocations.empty() )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsLocation' from config. Please see the comments in S63ENDDataExchangeMediaTests.ini", TSLErrorCategoryInfo);
        return false;
      }

      int tmp(0);
      TSLProfileHelper::lookupProfile("productLocationsAreAbsolute", &tmp, 0);
      bool productLocationsAreAbsolute( tmp );
            
      if( !TSLProfileHelper::lookupProfile("mediafile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'mediafile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string mediaFilePath( buffer );

      if( !TSLProfileHelper::lookupProfile("serialfile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFile( buffer );

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/8 Data Exchange Media/Test 8b/" );
      std::string dataStore = m_resultsPath + "OEM_1_2/test8b";
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 8B
      ////////////////////////////////////////////////////////////////////////////////////
     
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Iterate through all the locations for the media specified in the config file.
      std::vector<std::string>::const_iterator it( productLocations.begin() );
      std::vector<std::string>::const_iterator itE( productLocations.end() );
      for( ; it != itE; ++it )
      {
        ScriptedS63DataClient dataClient(script, dataClientExchangeSetPrefix, dataStore, m_csys, false);

        // Load PERMIT.TXT 
        TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permit.c_str(), &dataClient );

        // Failed to load PERMIT.TXT
        if( !collection )
        {
          TSLErrorStack::addError(0, "Failed to load PERMIT.TXT", TSLErrorCategoryInfo);
          return false;
        }

        // Permit should contain 10 valid cell permits
        if( collection->size() != 10 )
        {
          TSLErrorStack::addError(0, "Permit collection doesn't contain 10 cell permits", TSLErrorCategoryInfo);
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

        // Construct the media ingest manager
        // Note that this certificate is provided by the IHO, and is different to that used below
        manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
        manager->setProcessingFlags(TSLFilterProcessNone);

        if ( !manager->addCellPermits( collection, m_dsID.c_str() ) )
        {
          TSLErrorStack::addError(0, "Failed to add cell permits to manager", TSLErrorCategoryInfo);
          collection->destroy();
          manager->destroy();
          return false;
        }

        //Destroy the collection of cell permits as the manager takes copies
        collection->destroy();

        // Load and examine the MEDIA.TXT file, and load all exchange sets from the media.
        // The S63 SDK does not automatically read the contents of MEDIA.TXT, but
        // does provide a class for reading it.
        std::string mediaFileFullPath;
        if( productLocationsAreAbsolute )
        {
          mediaFileFullPath = *it + mediaFilePath;
        }
        else
        {
          mediaFileFullPath = m_dataLocation + *it + mediaFilePath;
        }
        TSLS63MediaFile *mediaFile = TSLS63MediaFile::create( mediaFileFullPath.c_str() );
        if ( mediaFile == NULL )
        {
          TSLErrorStack::addError(0, "Failed to load MEDIA.TXT", TSLErrorCategoryInfo);
          manager->destroy();
          return false;
        }

        std::string mediaLocation = mediaFileFullPath.erase(mediaFileFullPath.find_last_of("\\/")+1 ,mediaFileFullPath.size());
        for ( int i = 0; i < mediaFile->size(); i++ )
        {
          const TSLS63MediaRecord* pRecord = mediaFile->at(i);

          //	The location of the exchange set within the media, in
          //	the form of [M]1-99;[B or U]1-99
          //	Where M is a fixed character.
          //	B represents a base media exchange set.
          //	U represents an update media exchange set.
          std::string  recordLocation( pRecord->mediaLocation() );

          recordLocation = recordLocation.erase(0, recordLocation.find(";") + 1 );

          // The serial file location is derived from the location of the media file
          // plus the exchange set described within it
          std::string serialfileFromMedia( mediaLocation + recordLocation + serialFile );

          //Load the SERIAL.ENC file
          file = TSLS63SerialENCFile::create( serialfileFromMedia.c_str() );

          if( !file )
          {
            TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
            manager->destroy();
            mediaFile->destroy();
            return false;
          }

          ////////////////////////////////////////////////////////////////////////////////////
          // Load the exchange set.
          //
          // It should load without error.
          // The products file location can be anywhere that is accessible, as defined by
          // the test including Hard-Drive, CD-ROM, DVD, USB Memory Stick, or any other appropriate
          // media
          ////////////////////////////////////////////////////////////////////////////////////
          std::string stProducts;
          if( productLocationsAreAbsolute )
          {
            stProducts = *it + productsFile;
          }
          else
          {
            stProducts = m_dataLocation + *it + productsFile;
          }

          if ( !manager->loadExchangeSet( stProducts.c_str(), file, certificateFile.c_str() ) )
          {
            TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
            manager->destroy();
            file->destroy();
            mediaFile->destroy();
            return false;
          }

          file->destroy();
        }

        mediaFile->destroy();
        manager->destroy();

        // Ensure the calls to the data client were correct
        if( dataClient.didItWork(&firstError) == false )
        {
          std::string msg("Error from scripted data client: ");
          msg += firstError;
          TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
          return false;
        }
      }

      return true;
    }

    
    bool S63ENCDataExchangeMediaTests::loadTest8c( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath )
    {
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
        return false;
      }
      std::string permit( m_dataLocation + buffer);
      
      std::map<int, std::string> productFileLocations;
      int i(1);
      while( TSLProfileHelper::lookupProfile("productsfile", buffer, 1024, "", i ) )
      {
        productFileLocations[i] = buffer;
        ++i;
      }

      if (productFileLocations.empty() )
      {
        TSLErrorStack::addError(0, "Failed to read 'productFile' from config", TSLErrorCategoryInfo);
        return false;
      }
      
      // Product file for the update
      if( !TSLProfileHelper::lookupProfile("productsUpdatefile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsUpdatefile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsUpdateFile( m_dataLocation + buffer );
      
      if( !TSLProfileHelper::lookupProfile("serialUpdatefile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialUpdatefile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialUpdatefile( m_dataLocation + buffer );
            
      if( !TSLProfileHelper::lookupProfile("mediafile", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'mediafile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string mediafile( buffer );

      // There can be multiple serial file locations.
      std::vector<std::string> serialFileLocations;
      i = 1;
      while( TSLProfileHelper::lookupProfile("serialfileB", buffer, 1024, "", i) )
      {
        serialFileLocations.push_back(buffer);
        ++i;
      }

      if (serialFileLocations.empty() )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialFile' from config", TSLErrorCategoryInfo);
        return false;
      }

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      std::string dataClientExchangeSetPrefix( m_dataLocation + "/8 Data Exchange Media/Test 8c/BASE MEDIA/" );
      std::string dataStore = m_resultsPath + dataStorePath;
      TSLFileHelper::createDirectory(dataStore.c_str());

      std::string firstError;
      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 8C
      ////////////////////////////////////////////////////////////////////////////////////
     
      // Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      // Will be deleted by the caller
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

      // Permit should contain 8 valid cell permits
      if( collection->size() != 8 )
      {
        TSLErrorStack::addError(0, "Permit collection doesn't contain 8 cell permits", TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }
      
      // Permit ingest should have worked
      if( dataClient->anyErrorSoFar( &firstError ) == false )
      {
        std::string msg("Error from scripted data client: ");
        msg += firstError;
        TSLErrorStack::addError(0, msg.c_str(), TSLErrorCategoryInfo);
        collection->destroy();
        return false;
      }

      // Construct the media ingest manager
      // Will be deleted by the caller
      manager = new TSLS63MediaIngestManager( dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      // Ask the data client if all the callbacks to far have been as expected
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

      
      ////////////////////////////////////////////////////////////////////////////////////
      // This test requires that the OEM application will read the MEDIA.TXT from
      // the update media, and alert the user that the base media has not yet been installed.
      // The application must then guide the user through the rest of the ingest process.
      //
      // The S63 SDK will not automatically do this.
      // 
      // This test will read MEDIA.TXT from the update media, and use it to locate the
      // base media files for update 1.
      //
      // This test will then import the data in the correct sequence, as if the 
      // application had guided the user through the process.
      ////////////////////////////////////////////////////////////////////////////////////
      
      // Load and examine the MEDIA.TXT file
      std::string mediaFileFullPath( m_dataLocation + mediafile );
      TSLS63MediaFile *mediaFile = TSLS63MediaFile::create( mediaFileFullPath.c_str() );
      if ( mediaFile == NULL )
      {
        TSLErrorStack::addError(0, "Failed to load MEDIA.TXT", TSLErrorCategoryInfo);
        return false;
      }

      map<std::string,int>  mBaseMedia;
      // Iterate for each of the entries in the media file 
      // and find the update (U1)
      for ( int i = 0; i < mediaFile->size(); i++ )
      {
        const TSLS63MediaRecord* pRecord = mediaFile->at(i);

        //	The location of the exchange set within the media set in
        //	the form of [M]1-99;[B or U]1-99
        //	Where M is a fixed character.
        //	B represents a base media exchange set.
        //	U represents an update media exchange set.
        if ( pRecord->mediaLocation()[3] == 'B' )
        {
          // Base media specified. 
          // As we have not loaded any base media at this p[oint, it is arropriate to prompt the user
          // appropriatly
          mBaseMedia.insert( std::pair<std::string,int>(std::string(pRecord->mediaLocation()),0));
        }
        else
        {
          // The current record is for update 1
          // Verify that the corresponding base media 
          // is listed in MEDIA.TXT
          std::string  stBaseMedia( pRecord->mediaLocation() );
          stBaseMedia.replace(3,1,"B" );

          std::map<std::string,int>::iterator it;
          it = mBaseMedia.find(stBaseMedia);

          if ( it == mBaseMedia.end() )
          {
            TSLErrorStack::addError(0, "Failed to find base media for update", TSLErrorCategoryInfo);
            mediaFile->destroy();
            return false;
          }
        }
      }
      mediaFile->destroy();

      ////////////////////////////////////////////////////////////////////////////////////
      // At this point the OEM application should alert the user that the required
      // base media is not installed.
      //
      // The remainder of the test demonstrates the ingest of the test data, in the
      // required sequence.
      ////////////////////////////////////////////////////////////////////////////////////

      // Ingest all of the base exchange sets (B1 - B6)
      std::vector<std::string>::const_iterator it( serialFileLocations.begin() );
      std::vector<std::string>::const_iterator itE( serialFileLocations.end() );
      for( ; it != itE; ++it )
      {
        std::string vSerialFileLocation(*it);
        std::string productsFile;

        // The first character of the serial file config value
        // specifies which PRODUCTS.TXT should be used.
        int nProductsEntry = atoi( vSerialFileLocation.substr(0, 1).c_str() );

        std::map<int, std::string>::const_iterator itProd;
        itProd = productFileLocations.find(nProductsEntry);

        if ( itProd == productFileLocations.end() )
        {
          break; 
        }

        productsFile = m_dataLocation + (*itProd).second;

        // Remove the first 2 characters from the start of the serial file config value.
        // This is the index of PRODUCTS.TXT, plus a ;
        vSerialFileLocation.erase(0,2);
        vSerialFileLocation = m_dataLocation + vSerialFileLocation;

        if( file )
        {
          file->destroy();
          file = NULL;
        }
        // Load the SERIAL.ENC file
        // Will be deleted by the caller if needed
        file = TSLS63SerialENCFile::create( vSerialFileLocation.c_str() );

        if( !file )
        {
          TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
          return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Load the exchange set.
        // It should load without error.
        ////////////////////////////////////////////////////////////////////////////////////
        if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
        {
          TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
          return false;
        }
      }

      // Set the exchange set prefix to the update media location
      dataClientExchangeSetPrefix = m_dataLocation + "/8 Data Exchange Media/Test 8C/UPDATE MEDIA/";
      dataClient->setExchangeSetPrefix(dataClientExchangeSetPrefix);

      
      if( file )
      {
        file->destroy();
        file = NULL;
      }
      // Load the SERIAL.ENC file
      // Will be deleted by the caller if needed
      file = TSLS63SerialENCFile::create( serialUpdatefile.c_str() );
      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the update exchange set.
      // It should load without error.
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsUpdateFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        return false;
      }


      // Ensure the ingest state matches the test definition
      if( dataClient->numIngestedCells() != 8 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        return false;
      }

      int edition(0);
      int update(0);
      if( !dataClient->getIngestedCellInfo("GB100001", edition, update) ||
           edition != 3 ||
           update != 7 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB100001' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB100002", edition, update) ||
           edition != 13 ||
           update != 7 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB100002' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB100004", edition, update) ||
           edition != 8 ||
           update != 0 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB100004' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB281600", edition, update) ||
           edition != 1 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB281600' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB281800", edition, update) ||
           edition != 1 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB281800' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB301660", edition, update) ||
           edition != 5 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB301660' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB40162A", edition, update) ||
           edition != 9 ||
           update != 5 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40162A' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB61021B", edition, update) ||
           edition != 1 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB61021B' incorrect after ingest", TSLErrorCategoryInfo);
        return false;
      }

      return true;
    }
  
    bool S63ENCDataExchangeMediaTests::test8c()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST8C");

      std::string firstError;

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);
  
      const char* dataStorePath( "OEM_1_2/test8c" );

      // Load the configuration data for test 5C
      // and run the test
      bool result( loadTest8c( file, manager, dataClient, dataStorePath) );

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
        return result;
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


    bool S63ENCDataExchangeMediaTests::test8d()
    {
      // Read test-specific config
      TSLProfileHelper::setDefaultFilename(m_configFile.c_str());
      TSLProfileHelper::setDefaultSection("TEST8D");

      std::string firstError;

      TSLS63SerialENCFile       *file(NULL);
      TSLS63MediaIngestManager  *manager(NULL);
      ScriptedS63DataClient     *dataClient(NULL);
  
      char buffer[1024];
  
      // Get the products file. Note that in this test, it is no longer relative to the
      // data location
      if( !TSLProfileHelper::lookupProfile("productsfileD", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsFile( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("productsfileDBase", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'productsfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string productsfileBase( m_dataLocation + buffer );

      if( !TSLProfileHelper::lookupProfile("serialfileD", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialfile' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string serialFile( m_dataLocation + buffer );

      std::vector<std::string> baseSerialFileLocations;
      int i(1);
      while( TSLProfileHelper::lookupProfile( "serialfileDB", buffer, 1024, "", i ) )
      {
        baseSerialFileLocations.push_back(buffer);
        ++i;
      }
      if( baseSerialFileLocations.empty() )
      {
        TSLErrorStack::addError(0, "Failed to read 'serialFile' from config", TSLErrorCategoryInfo);
        return false;
      }

      if( !TSLProfileHelper::lookupProfile("certificate", buffer, 1024) )
      {
        TSLErrorStack::addError(0, "Failed to read 'certificate' from config", TSLErrorCategoryInfo);
        return false;
      }
      std::string certificateFile( m_dataLocation + buffer );

      // Set the data exchange location as 8D
      std::string dataClientExchangeSetPrefix( m_dataLocation + "/8 Data Exchange Media/Test 8D/UPDATE MEDIA/" );

      const char* dataStorePath( "OEM_1_2/test8d" );

      // Load the configuration data for test 8C
      // and run the test
      if ( !loadTest8c( file, manager, dataClient, dataStorePath ) )
      {
        TSLErrorStack::addError(0, "Initial test setup invalid, because test 8c failed", TSLErrorCategoryInfo);
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

      if( file )
      {
        file->destroy();
      }

      
      ////////////////////////////////////////////////////////////////////////////////////
      // Perform Test 8D
      //
      // This test requires that the OEM application manages dependencies between
      // base and update exchange sets.
      // The application must display a warning to the user when trying to ingest the 
      // update exchange set, along the lines of:
      // 'The Update Media is not compatible with currently installed ENCs. 
      //  Please load the latest version of BASE MEDIA 1 –Week 40/07 -dated 04 October 2007 
      //  and continue with the update process'
      //
      // The S63 SDK will not automatically do this.
      //
      // This test will:
      // - Ingest the update exchange set
      // - Ingest the base exchange sets
      // - Ingest the update exchange set
      // - Ensure the cells were ingested correctly, to match the test definition.
      ////////////////////////////////////////////////////////////////////////////////////
      dataClient->setExchangeSetPrefix( dataClientExchangeSetPrefix );

      // Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );
      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Load the update exchange set.
      //
      // Two SSE 23 errors need to be produced
      ////////////////////////////////////////////////////////////////////////////////////
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }
      // Now the update set has been loaded, as far as is possible
      file->destroy();
      file = NULL;

      // Set the data exchange location as Base 8D
      dataClientExchangeSetPrefix = m_dataLocation + "/8 Data Exchange Media/Test 8D/BASE MEDIA/";
      dataClient->setExchangeSetPrefix( dataClientExchangeSetPrefix );

      // Load the exchange sets from the base media
      std::vector<string>::iterator it = baseSerialFileLocations.begin();
      std::vector<string>::iterator itE = baseSerialFileLocations.end();
      for( ; it != itE ; ++it)
      {
        std::string stSerialFileLocation(*it);

        stSerialFileLocation = m_dataLocation + stSerialFileLocation;

        // Load the SERIAL.ENC file
        file = TSLS63SerialENCFile::create( stSerialFileLocation.c_str() );
        if( !file )
        {
          TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
          manager->destroy();
          delete dataClient;
          return false;
        }

        ////////////////////////////////////////////////////////////////////////////////////
        // Load the exchange set.
        // It should load without error.
        ////////////////////////////////////////////////////////////////////////////////////
        if ( !manager->loadExchangeSet( productsfileBase.c_str(), file, certificateFile.c_str() ) )
        {
          TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
          manager->destroy();
          file->destroy();
          delete dataClient;
          return false;
        }

        file->destroy();
      }

      ////////////////////////////////////////////////////////////////////////////////////
      // Try to ingest the update exchange set again.
      // 
      // The exchange set should ingest successfully,
      // and the system status should match the test definition
      ////////////////////////////////////////////////////////////////////////////////////
      dataClientExchangeSetPrefix = m_dataLocation + "/8 Data Exchange Media/Test 8D/UPDATE MEDIA/";
      dataClient->setExchangeSetPrefix( dataClientExchangeSetPrefix );

      // Load the SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialFile.c_str() );
      if( !file )
      {
        TSLErrorStack::addError(0, "Failed to load SERIAL.ENC", TSLErrorCategoryInfo);
        manager->destroy();
        delete dataClient;
        return false;
      }
      if ( !manager->loadExchangeSet( productsFile.c_str(), file, certificateFile.c_str() ) )
      {
        TSLErrorStack::addError(0, "Failed to load exchange set", TSLErrorCategoryInfo);
        manager->destroy();
        file->destroy();
        delete dataClient;
        return false;
      }
      // Now the update set has been loaded, as far as is possible
      file->destroy();
      file = NULL;

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
      if( dataClient->numIngestedCells() != 8 )
      {
        TSLErrorStack::addError(0, "Data client contains the wrong number of cells after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      int edition(0);
      int update(0);
      if( !dataClient->getIngestedCellInfo("GB100001", edition, update) ||
           edition != 3 ||
           update != 7 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB100001' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB100002", edition, update) ||
           edition != 13 ||
           update != 9 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB100002' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB100004", edition, update) ||
           edition != 8 ||
           update != 10 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB100004' incorrect after ingest", TSLErrorCategoryInfo);
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
      if( !dataClient->getIngestedCellInfo("GB301660", edition, update) ||
           edition != 5 ||
           update != 1 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB301660' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB40162A", edition, update) ||
           edition != 9 ||
           update != 6 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB40162A' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }
      if( !dataClient->getIngestedCellInfo("GB61021B", edition, update) ||
           edition != 1 ||
           update != 2 )
      {
        TSLErrorStack::addError(0, "State of cell 'GB61021B' incorrect after ingest", TSLErrorCategoryInfo);
        delete dataClient;
        return false;
      }

      delete dataClient;

      return true;
    }
  };
};