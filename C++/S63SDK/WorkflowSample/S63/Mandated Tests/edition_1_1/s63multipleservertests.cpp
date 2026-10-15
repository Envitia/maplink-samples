//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63multipleservertests.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63MultipleServerTests::S63MultipleServerTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
      , m_csys(NULL)
      , m_hw_id()
      , m_exchangeSet_1()
      , m_exchangeSet_2()
      , m_exchangeSet_3()
      , m_exchangeSet_4()
      , m_exchangeSet_5()
      , m_test1script()
      , m_test2script()
      , m_permit_1()
      , m_permit_2()
      , m_permit_3()
      , m_certificatePath()
    {
      string configurationFile = config.m_configurationLocation;
      configurationFile.append( "s63multipleservertests.ini" ) ;

      readConfiguration(configurationFile);
    }

    S63MultipleServerTests::~S63MultipleServerTests()
    {
      if ( m_csys )
      {
        m_csys->destroy();
      }
    }

    void S63MultipleServerTests::readConfiguration(const string& configFile)
    {
      char buffer[1024];

      //General Info
      TSLProfileHelper::setDefaultFilename( configFile.c_str() );
      TSLProfileHelper::setDefaultSection( "GENERAL" );

      if ( TSLProfileHelper::lookupProfile( "certificatePath", buffer, 1024 ) )
      {
        m_certificatePath = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "coordinateSystemName", buffer, 1024 ) )
      {
        const TSLCoordinateSystem* csys = TSLCoordinateSystem::findByName(buffer);
        if ( csys )
        {
          m_csys = csys->clone(1000);
          if ( TSLProfileHelper::lookupProfile( "coordinateSystemTMCMU", buffer, 1024 ) )
          {
            m_csys->setTMCperMU(atof(buffer));
          }
        }
      }

      //User Info
      TSLProfileHelper::setDefaultSection( "USERDETAILS" );

      if ( TSLProfileHelper::lookupProfile( "hw_id", buffer, 1024 ) )
      {
        m_hw_id = buffer;
      }

      //Data Location
      TSLProfileHelper::setDefaultSection( "DATALOCATIONS" );

      if ( TSLProfileHelper::lookupProfile( "exchangeSet1", buffer, 1024 ) )
      {
        m_exchangeSet_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "exchangeSet2", buffer, 1024 ) )
      {
        m_exchangeSet_2 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "exchangeSet3", buffer, 1024 ) )
      {
        m_exchangeSet_3 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "exchangeSet4", buffer, 1024 ) )
      {
        m_exchangeSet_4 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "exchangeSet5", buffer, 1024 ) )
      {
        m_exchangeSet_5 = (m_dataLocation + buffer);
      }

      //Standard Cell Permit Info
      TSLProfileHelper::setDefaultSection( "CELLPERMIT" );

      if ( TSLProfileHelper::lookupProfile( "permit_1", buffer, 1024 ) )
      {
        m_permit_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "permit_2", buffer, 1024 ) )
      {
        m_permit_2 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "permit_3", buffer, 1024 ) )
      {
        m_permit_3 = (m_dataLocation + buffer);
      }

      TSLProfileHelper::setDefaultSection( "DATACLIENT_SCRIPT_FILES" );
      if ( TSLProfileHelper::lookupProfile( "test1", buffer, 1024 ) )
      {
        m_test1script = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test2", buffer, 1024 ) )
      {
        m_test2script = (m_dataLocation + buffer);
      }

    }

    bool S63MultipleServerTests::test1()
    {
      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test1script, m_exchangeSet_1, m_resultsPath, m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection1 = TSLS63CellPermitCollection::create( m_permit_1.c_str(), &dataClient );

      //Ask the data client if all the callbacks to far have been as expected
      //Also check that the PERMIT.TXT file was constructed and that it contains
      //at least one permit
      if ( !dataClient.anyErrorSoFar() || !collection1 || collection1->size() == 0 )
      {
        if ( collection1 )
        {
          collection1->destroy() ;
        }
        return false;
      }

      //Load the second PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection2 = TSLS63CellPermitCollection::create( m_permit_2.c_str(), &dataClient );

      //Ask the data client if all the callbacks to far have been as expected
      //Also check that the PERMIT.TXT file was constructed and that it contains
      //at least one permit
      if ( !dataClient.anyErrorSoFar() || !collection2 || collection2->size() == 0 )
      {
        collection1->destroy();
        if ( collection2 )
        {
          collection2->destroy() ;
        }
        return false;
      }

      //Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);  
      //Ask the data client if all the callbacks to far have been as expected
      //Add the first set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection1 ) )
      {
        collection1->destroy();
        collection2->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the first collection of cell permits as the manager takes copies
      collection1->destroy();

      //Ask the data client if all the callbacks to far have been as expected
      //Add the second set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      //The ask the data client again if all the callbacks to far have been as expected
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection2 ) || !dataClient.anyErrorSoFar() )
      {
        collection2->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the second collection of cell permits as the manager takes copies
      collection2->destroy();

      //Build the path to the PRODUCTS.TXT file
      string productsPath( m_exchangeSet_1 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_exchangeSet_1 );
      serialPath += "SERIAL.ENC";

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, m_certificatePath.c_str() ) )
      {
        manager->destroy();
        file->destroy();
        return false;
      }

      //Destroy the loaded SERIAL.ENC file
      file->destroy();

      //Ask the data client if all of the callbacks so far have been as expected
      if ( !dataClient.anyErrorSoFar() )
      {
        manager->destroy();
        return false;
      }

      //Build the path to the second PRODUCTS.TXT file
      productsPath = m_exchangeSet_2;
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the second SERIAL.ENC file
      serialPath = m_exchangeSet_2;
      serialPath += "SERIAL.ENC";

      //Inform the data client that the location of the test data that is about to be
      //ingested differs from the location that was passed to it during construction
      //
      //This mechanism is there so that the pathes in the ini file that runs the data client
      //don't need to be absolute.
      dataClient.setExchangeSetPrefix(m_exchangeSet_2);

      //Load the second SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      //
      //Then ask the data client if all of the callbacks so far have been as expected
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, m_certificatePath.c_str() ) || !dataClient.anyErrorSoFar() )
      {
        manager->destroy();
        file->destroy();
        return false;
      }

      //Destroy the loaded SERIAL.ENC file
      file->destroy();

      //Build the path to the third PRODUCTS.TXT file
      productsPath = m_exchangeSet_3;
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the third SERIAL.ENC file
      serialPath = m_exchangeSet_3;
      serialPath += "SERIAL.ENC";

      //Inform the data client that the location of the test data that is about to be
      //ingested differs from the location that was passed to it during
      //construction/set through the last call to setExchangeSetPrefix
      //
      //This mechanism is there so that the pathes in the ini file that runs the data client
      //don't need to be absolute.
      dataClient.setExchangeSetPrefix(m_exchangeSet_3);

      //Load the third SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      //
      //Then ask the data client if all of the callbacks so far have been as expected
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, m_certificatePath.c_str() ) || !dataClient.anyErrorSoFar() )
      {
        manager->destroy();
        file->destroy();
        return false;
      }

      //Destroy the loaded SERIAL.ENC file
      file->destroy();

      //Build the path to the forth PRODUCTS.TXT file
      productsPath = m_exchangeSet_4;
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the forth SERIAL.ENC file
      serialPath = m_exchangeSet_4;
      serialPath += "SERIAL.ENC";

      //Inform the data client that the location of the test data that is about to be
      //ingested differs from the location that was passed to it during
      //construction/set through the last call to setExchangeSetPrefix
      //
      //This mechanism is there so that the pathes in the ini file that runs the data client
      //don't need to be absolute.
      dataClient.setExchangeSetPrefix(m_exchangeSet_4);

      //Load the forth SERIAL.ENC file
      file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, m_certificatePath.c_str() ) )
      {
        manager->destroy();
        file->destroy();
        return false;
      }

      //Destroy the loaded SERIAL.ENC file and the manager now that we have finished
      file->destroy();
      manager->destroy();

      //This final call to the data client checks that all calls so far have been
      //successful but also that all the expected callbacks have been maded
      return dataClient.didItWork();
    }

    bool S63MultipleServerTests::test2()
    {
      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test2script, m_exchangeSet_5, m_resultsPath, m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_permit_3.c_str(), &dataClient );

      //Ask the data client if all the callbacks to far have been as expected
      //Also check that the PERMIT.TXT file was constructed and that it contains
      //at least one permit
      if ( !dataClient.anyErrorSoFar() || !collection || collection->size() == 0 )
      {
        if ( collection )
        {
          collection->destroy() ;
        }
        return false;
      }

      //Construct a TSLS63HWID class around the user's hw_id (in ASCII)
      TSLS63HWID hardwareID( m_hw_id.c_str() );

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, m_certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);  
      //Ask the data client if all the callbacks to far have been as expected
      //Add the first set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection ) )
      {
        collection->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Build the path to the PRODUCTS.TXT file
      string productsPath( m_exchangeSet_5 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_exchangeSet_5 );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, m_certificatePath.c_str() ) )
      {
        manager->destroy();
        file->destroy();
        return false;
      }

      //Destroy the loaded SERIAL.ENC file and the manager now that we have finished
      file->destroy();
      manager->destroy();

      //This final call to the data client checks that all calls so far have been
      //successful but also that all the expected callbacks have been maded
      return dataClient.didItWork();
    }

  };
};