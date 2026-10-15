//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63digitalsignaturetests.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63DigitalSignatureTests::S63DigitalSignatureTests(const S63ConfigurationObject &configurationObject)
      : S63TestBase(configurationObject)
      , m_certificatePath()
      , m_hw_id()
      , m_test4_3()
      , m_test5_1()
      , m_permits_1()
      , m_test1script()
      , m_test2script()
      , m_csys()
    {
      string configurationFile = configurationObject.m_configurationLocation;
      configurationFile.append( "s63digitalsignaturetests.ini" ) ;

      readConfiguration(configurationFile);
    }

    S63DigitalSignatureTests::~S63DigitalSignatureTests()
    {

    }

    void S63DigitalSignatureTests::readConfiguration(const string &configFile)
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
        m_csys = TSLCoordinateSystem::findByName(buffer);
      }

      //User Info
      TSLProfileHelper::setDefaultSection( "USERDETAILS" );

      if ( TSLProfileHelper::lookupProfile( "hw_id", buffer, 1024 ) )
      {
        m_hw_id = buffer;
      }

      //Data Location
      TSLProfileHelper::setDefaultSection( "DATALOCATIONS" );

      if ( TSLProfileHelper::lookupProfile( "test4_3", buffer, 1024 ) )
      {
        m_test4_3 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test5_1", buffer, 1024 ) )
      {
        m_test5_1 = (m_dataLocation + buffer);
      }

      TSLProfileHelper::setDefaultSection( "CELLPERMIT" );
      if ( TSLProfileHelper::lookupProfile( "permits_1", buffer, 1024 ) )
      {
        m_permits_1 = (m_dataLocation + buffer);
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

    //Test 4.2 – Authenticate ENC signature
    bool S63DigitalSignatureTests::test1() const 
    {
      //------------------------------------------------------------------------
      //If the data is ingest correctly then the Data servers public key matches
      //MapLink doesn't provide access to the data server public key however
      //------------------------------------------------------------------------

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test1script, m_test5_1, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_permits_1.c_str(), &dataClient );

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
      if ( !dataClient.anyErrorSoFar() )
      {
        manager->destroy();
        collection->destroy();
        return false;
      }

      //Add the set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      const int noOfPermits = collection->size();
      for ( int i = 0; i < noOfPermits; ++i )
      {
        if ( !manager->addCellPermit( collection->at(i) ) )
        {
          collection->destroy();
          manager->destroy();
          return false;
        }
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Build the path to the PRODUCTS.TXT file
      string productsPath( m_test5_1 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_test5_1 );
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

      //Destroy the SERIAL.ENC file and manager
      file->destroy();
      manager->destroy();

      //This final call to the data client checks that all calls so far have been
      //successful but also that all the expected callbacks have been maded
      return dataClient.didItWork();
    }

    bool S63DigitalSignatureTests::test2() const
    {
      //------------------------------------------------------------------------
      //The Data Client script for this test checks that the S63 error SSE 9
      //is raised for each of the ingested cells
      //------------------------------------------------------------------------

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test2script, m_test4_3, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_permits_1.c_str(), &dataClient );

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
      string productsPath( m_test4_3 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_test4_3 );
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

      //Destroy the SERIAL.ENC file and manager
      file->destroy();
      manager->destroy();

      //We know that errors were raised on the error stack.
      //We'll clear them so that they don't interfer with other tests
      TSLThreadedErrorStack::clear();

      //This final call to the data client checks that all calls so far have been
      //successful but also that all the expected callbacks have been maded
      return dataClient.didItWork();
    }

  };
};