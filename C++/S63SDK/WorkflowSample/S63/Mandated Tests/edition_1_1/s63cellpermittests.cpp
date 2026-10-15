//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63cellpermittests.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63CellPermitTests::S63CellPermitTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
      , m_csys(NULL)
      , m_hw_id()
      , m_dsID()
      , m_cellPermit_2_1()
      , m_cellPermitExtended_2_1()
      , m_cellPermit_2_2()
      , m_cellPermitExtended_2_2()
      , m_erroneousPermit()
      , m_erroneousPermitExtended()
      , m_expiredPermit()
      , m_expiredPermitExtended()
      , m_encryptedENC_5_1()
      , m_unencryptedENC_5_1()
      , m_encryptedENC_5_3()
      , m_unencryptedENC_5_3()
      , m_test1script()
      , m_test2script()
      , m_test3script()
    {
      m_dataLocation = config.m_testDataLocation;

      string configurationFile = config.m_configurationLocation;
      configurationFile.append( "s63cellpermittests.ini" ) ;

      readConfiguration(configurationFile);
    }

    S63CellPermitTests::~S63CellPermitTests()
    {
    }

    void S63CellPermitTests::readConfiguration(const string &configFile)
    {
      char buffer[1024];

      //General Info
      TSLProfileHelper::setDefaultFilename( configFile.c_str() );
      TSLProfileHelper::setDefaultSection( "GENERAL" );

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

      //Standard Cell Permit Info
      TSLProfileHelper::setDefaultSection( "CELLPERMIT" );

      if ( TSLProfileHelper::lookupProfile( "standard_1", buffer, 1024 ) )
      {
        m_cellPermit_2_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "standard_2", buffer, 1024 ) )
      {
        m_cellPermitExtended_2_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "standard_3", buffer, 1024 ) )
      {
        m_cellPermit_2_2 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "standard_4", buffer, 1024 ) )
      {
        m_cellPermitExtended_2_2 = (m_dataLocation + buffer);
      }

      //Incorrect Cell Permit Info
      if ( TSLProfileHelper::lookupProfile( "erroneous_1", buffer, 1024 ) )
      {
        m_erroneousPermit = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "erroneous_2", buffer, 1024 ) )
      {
        m_erroneousPermitExtended = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "expired_1", buffer, 1024 ) )
      {
        m_expiredPermit = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "expired_2", buffer, 1024 ) )
      {
        m_expiredPermitExtended = (m_dataLocation + buffer);
      }

      //ENC
      TSLProfileHelper::setDefaultSection( "ENC" );

      if ( TSLProfileHelper::lookupProfile( "dsid", buffer, 1024 ) )
      {
        m_dsID = buffer;
      }

      if ( TSLProfileHelper::lookupProfile( "encrypted_1", buffer, 1024 ) )
      {
        m_encryptedENC_5_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "unencrypted_1", buffer, 1024 ) )
      {
        m_unencryptedENC_5_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "encrypted_2", buffer, 1024 ) )
      {
        m_encryptedENC_5_3 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "unencrypted_2", buffer, 1024 ) )
      {
        m_unencryptedENC_5_3 = (m_dataLocation + buffer);
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

      if ( TSLProfileHelper::lookupProfile( "test3", buffer, 1024 ) )
      {
        m_test3script = (m_dataLocation + buffer);
      }

    }

    bool S63CellPermitTests::test1() const
    {
      //First run the test using the non-extended permits from test set1
      if ( !test1(m_cellPermit_2_1, m_encryptedENC_5_1) )
        return false;

      //Then run the test using the extended permits from test set1
      if ( !test1(m_cellPermitExtended_2_1, m_encryptedENC_5_1) )
        return false;

      //Next run the test using the non-extended permits from test set2
      if ( !test1(m_cellPermit_2_2, m_encryptedENC_5_3) )
        return false;

      //Finally run the test using the extended permits from test set2
      if ( !test1(m_cellPermitExtended_2_2, m_encryptedENC_5_3) )
        return false;

      return true;
    }

    bool S63CellPermitTests::test2() const
    {
      //First run the test using the erroneous non-extended permits
      if ( !test2(m_erroneousPermit, m_encryptedENC_5_1) )
        return false;

      //Then run the test using the erroneous extended permits
      if ( !test2(m_erroneousPermitExtended, m_encryptedENC_5_1) )
        return false;

      return true;
    }


    bool S63CellPermitTests::test3() const 
    {
      //First run the test using the expired non-extended permits
      if ( !test3(m_expiredPermit, m_encryptedENC_5_1) )
        return false;

      //Then run the test using the expired extended permits
      if ( !test3(m_expiredPermitExtended, m_encryptedENC_5_1) )
        return false;

      return true; 
    }


    bool S63CellPermitTests::test1( const string& permitLocation, const string & dataLocation ) const
    {
      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test1script, dataLocation, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permitLocation.c_str(), &dataClient );

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

      //Construct the path to the SA's certificate
      string certificatePath ( m_dataLocation ) ;
      certificatePath += "/S-63 Test 3.1/IHO.CRT";

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);

      //Ask the data client if all the callbacks to far have been as expected
      //Add the first set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        collection->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Build the path to the PRODUCTS.TXT file
      string productsPath( dataLocation );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( dataLocation );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, certificatePath.c_str() ) )
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

    bool S63CellPermitTests::test2( const string& permitLocation, const string & dataLocation ) const
    {
      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test2script, dataLocation, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permitLocation.c_str(), &dataClient );

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

      //Construct the path to the SA's certificate
      string certificatePath ( m_dataLocation ) ;
      certificatePath += "/S-63 Test 3.1/IHO.CRT";

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone); 
      //Ask the data client if all the callbacks to far have been as expected
      //Add the first set of cell permits to the manager and make sure that the
      //  return value indicates that they are NOT added correctly
      if ( !dataClient.anyErrorSoFar() || manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        collection->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Build the path to the PRODUCTS.TXT file
      string productsPath( dataLocation );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( dataLocation );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, certificatePath.c_str() ) )
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

    bool S63CellPermitTests::test3( const string& permitLocation, const string & dataLocation ) const
    {
      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test3script, dataLocation, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( permitLocation.c_str(), &dataClient );

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

      //Construct the path to the SA's certificate
      string certificatePath ( m_dataLocation ) ;
      certificatePath += "/S-63 Test 3.1/IHO.CRT";

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificatePath.c_str(), &hardwareID );
      manager->setProcessingFlags(TSLFilterProcessNone);  
      //Ask the data client if all the callbacks to far have been as expected
      //Add the first set of cell permits to the manager and make sure that the
      //  return value indicates that they are added correctly
      if ( !dataClient.anyErrorSoFar() || !manager->addCellPermits( collection, m_dsID.c_str() ) )
      {
        collection->destroy();
        manager->destroy();
        return false;
      }

      //Destroy the collection of cell permits as the manager takes copies
      collection->destroy();

      //Build the path to the PRODUCTS.TXT file
      string productsPath( dataLocation );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( dataLocation );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the SA's certificate passed during construction of the manager.
      //
      //Call should raise SSE 15 for each of the expired cell permits
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, certificatePath.c_str() ) )
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

  };
};