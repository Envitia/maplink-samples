//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63encryptiondecryptiontests.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63EncryptionDecryptionTests::S63EncryptionDecryptionTests(const S63ConfigurationObject &configurationObject)
      : S63TestBase(configurationObject)
      , m_certificatePath()
      , m_hw_id()
      , m_comparisonDir1()
      , m_comparisonDir2()
      , m_test5_1()
      , m_test5_3()
      , m_test5_5()
      , m_cellPermit1()
      , m_cellPermit2()
      , m_test1script()
      , m_test2script()
      , m_test3script()
      , m_csys(NULL)
    {
      string configurationFile = configurationObject.m_configurationLocation;
      configurationFile.append( "s63encryptiondecryptiontests.ini" ) ;

      readConfiguration(configurationFile);
    }

    S63EncryptionDecryptionTests::~S63EncryptionDecryptionTests()
    {

    }

    void S63EncryptionDecryptionTests::readConfiguration(const string &configFile)
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

      //Comparison Location
      TSLProfileHelper::setDefaultSection( "COMPARISON_DIRS" );

      if ( TSLProfileHelper::lookupProfile( "dir1", buffer, 1024 ) )
      {
        m_comparisonDir1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "dir2", buffer, 1024 ) )
      {
        m_comparisonDir2 = (m_dataLocation + buffer);
      }

      //Data Location
      TSLProfileHelper::setDefaultSection( "DATALOCATIONS" );

      if ( TSLProfileHelper::lookupProfile( "test5_1", buffer, 1024 ) )
      {
        m_test5_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test5_3", buffer, 1024 ) )
      {
        m_test5_3 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test5_5", buffer, 1024 ) )
      {
        m_test5_5 = (m_dataLocation + buffer);
      }

      //Miscellaneous
      TSLProfileHelper::setDefaultSection( "MISCELLANEOUS" );

      if ( TSLProfileHelper::lookupProfile( "cellPermits_1", buffer, 1024 ) )
      {
        m_cellPermit1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "cellPermits_2", buffer, 1024 ) )
      {
        m_cellPermit2 = (m_dataLocation + buffer);
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

      if ( TSLProfileHelper::lookupProfile( "test4", buffer, 1024 ) )
      {
        m_test4script = (m_dataLocation + buffer);
      }
    }

    bool S63EncryptionDecryptionTests::test1() const
    {

      //Clear the error stack so that errors raised from previous tests
      //don't interfer with this test
      TSLThreadedErrorStack::clear();

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test1script, m_test5_1, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_cellPermit1.c_str(), &dataClient );

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
      //Add the set of cell permits to the manager and make sure that the
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
      string productsPath( m_test5_1 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_test5_1 );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //NOTE THIS CALL IS ONLY FOR USE WHEN PASSING THE S63 TESTS
      //It tells the manager the location of a directory containing
      //the unencrypted cell files. When the manager loads the exchange
      //set it will compare the enc cells files, once it has 
      //decrypted/decompressed them, to the files found in this directory
      //
      //If it cannot find a corresponding unencrypted ENC cell file
      //or if the decrypted ENC cell file does not match the one found
      //in the directory it will raise an error. The errors raised
      //will have the error category TSLErrorCategoryDebug so that
      //they can be identified separately from other errors that may
      //occur during ingest.
      manager->dbgReferenceCellDirectory(m_comparisonDir1.c_str());

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
      if ( !dataClient.didItWork() )
        return false;

      //This checks that no errors were raised with the error category 
      //TSLErrorCategoryDebug. I.E. no errors were raised from the comparison
      //of the decrypted data and the data held in the unencrypted directory.
      return TSLThreadedErrorStack::size(TSLErrorCategoryDebug) == 0 ;
    }

    bool S63EncryptionDecryptionTests::test2() const
    {
      //Clear the error stack so that errors raised from previous tests
      //don't interfere with this test
      TSLThreadedErrorStack::clear();

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test2script, m_test5_3, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_cellPermit2.c_str(), &dataClient );

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
      //Add the set of cell permits to the manager and make sure that the
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
      string productsPath( m_test5_3 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_test5_3 );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //NOTE THIS CALL IS ONLY FOR USE WHEN PASSING THE S63 TESTS
      //It tells the manager the location of a directory containing
      //the unencrypted cell files. When the manager loads the exchange
      //set it will compare the enc cells files, once it has 
      //decrypted/decompressed them, to the files found in this directory
      //
      //If it cannot find a corresponding unencrypted ENC cell file
      //or if the decrypted ENC cell file does not match the one found
      //in the directory it will raise an error. The errors raised
      //will have the error category TSLErrorCategoryDebug so that
      //they can be identified separately from other errors that may
      //occur during ingest.
      manager->dbgReferenceCellDirectory(m_comparisonDir2.c_str());

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
      if ( !dataClient.didItWork() )
        return false;

      //This checks that no errors were raised with the error category 
      //TSLErrorCategoryDebug. I.E. no errors were raised from the comparison
      //of the decrypted data and the data held in the unencrypted directory.
      return TSLThreadedErrorStack::size(TSLErrorCategoryDebug) == 0 ;
    }

    bool S63EncryptionDecryptionTests::test3() const
    {
      //----------------------------------------------------------------------------
      //This test is identifical to the first one (split between ::test1 && ::test2)
      //----------------------------------------------------------------------------
      return test1() && test2();
    }

    bool S63EncryptionDecryptionTests::test4() const
    {
      //Clear the error stack so that errors raised from previous tests
      //don't interfer with this test
      TSLThreadedErrorStack::clear();

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test4script, m_test5_5, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_cellPermit2.c_str(), &dataClient );

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
      //Add the set of cell permits to the manager and make sure that the
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
      string productsPath( m_test5_5 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_test5_5 );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //NOTE THIS CALL IS ONLY FOR USE WHEN PASSING THE S63 TESTS
      //It tells the manager to bypass the cell signature verification for the ENC cells
      //in the dataset. This is necessary to complete the test as no valid signature files
      //for the dataset are provided with the test data.
      //
      //When this option is enabled no cells will be ingested beyond the point of verifying
      //each decrypted ENC cell irrespective of whether the cells pass the CRC checksum or not.
      manager->dbgPerformCellSignatureVerification( false );

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
      if ( !dataClient.didItWork() )
        return false;

      //This checks that no errors were raised with the error category 
      //TSLErrorCategoryDebug. I.E. no errors were raised from the comparison
      //of the decrypted data and the data held in the unencrypted directory.
      return TSLThreadedErrorStack::size(TSLErrorCategoryDebug) == 0 ;
    }

  };
};