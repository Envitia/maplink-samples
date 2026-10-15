//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63certificatetests.h"
#include "scripteds63dataclient.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63CertificateTests::S63CertificateTests(const S63ConfigurationObject &config)
      : S63TestBase(config)
      , m_hw_id()
      , m_test3_1()
      , m_test3_2()
      , m_test3_3()
      , m_test3_5()
      , m_test3_6()
      , m_test3script()
      , m_test4script()
      , m_test5script()
      , m_cellPermit_2_1()
      , m_encryptedENC_5_1()
      , m_csys(NULL)
    {
      string configurationFile = config.m_configurationLocation;
      configurationFile.append( "s63certificatetests.ini" ) ;

      readConfiguration(configurationFile);
    }

    S63CertificateTests::~S63CertificateTests()
    {

    }

    void S63CertificateTests::readConfiguration(const string &configFile)
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

      //Data Location
      TSLProfileHelper::setDefaultSection( "DATALOCATIONS" );

      if ( TSLProfileHelper::lookupProfile( "test3_1", buffer, 1024 ) )
      {
        m_test3_1 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test3_2", buffer, 1024 ) )
      {
        m_test3_2 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test3_3", buffer, 1024 ) )
      {
        m_test3_3 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test3_5", buffer, 1024 ) )
      {
        m_test3_5 = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test3_6", buffer, 1024 ) )
      {
        m_test3_6 = (m_dataLocation + buffer);
      }

      //Standard Cell Permit Info
      TSLProfileHelper::setDefaultSection( "CELLPERMIT" );

      if ( TSLProfileHelper::lookupProfile( "standard_1", buffer, 1024 ) )
      {
        m_cellPermit_2_1 = (m_dataLocation + buffer);
      }

      //ENC
      TSLProfileHelper::setDefaultSection( "ENC" );

      if ( TSLProfileHelper::lookupProfile( "encrypted_1", buffer, 1024 ) )
      {
        m_encryptedENC_5_1 = (m_dataLocation + buffer);
      }

      TSLProfileHelper::setDefaultSection( "DATACLIENT_SCRIPT_FILES" );
      if ( TSLProfileHelper::lookupProfile( "test3", buffer, 1024 ) )
      {
        m_test3script = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test4", buffer, 1024 ) )
      {
        m_test4script = (m_dataLocation + buffer);
      }

      if ( TSLProfileHelper::lookupProfile( "test5", buffer, 1024 ) )
      {
        m_test5script = (m_dataLocation + buffer);
      }
    }

    //Test 3.2 - Data Client Uses Correct SA Public Key File
    bool S63CertificateTests::test1() const
    {
      //-------------------------------------------------------------------------------
      //MapLink is unable to verify this test
      //
      //It is up to the OEM to only ever provide MapLink with certificates
      //from a securely held location on the data client as opposed to from
      //the exchange set media. Neither the certificate parameter to the 
      //construction of the TSLS63MediaIngestManager class nor the certificate
      //parameter to the TSLS63MediaIngestManager::loadExchangeSet method should
      //be taken from the input media.
      //
      //The certificate passed during the construction of the TSLS63MediaIngestManager
      //class should only ever be the SA's certificate.
      //
      //In most cases the certificate passed to the loadExchangeSet method will again
      //be the SA's certificate. If, however, the data held on the exchange set is 
      //self-signed by the DS, then this certificate should be the DS self sign 
      //certificate which again should be held in a secure location on the data client.
      //-------------------------------------------------------------------------------
      return true;
    }

    //Test 3.3 – Data Client Authenticates SA Public Key on CD Against
    bool S63CertificateTests::test2() const
    {
      //-------------------------------------------------------------------------------
      //MapLink is unable to verify this test
      //
      //MapLink does however provide the ability for the OEM to pass this test through
      //using the TSLS63MediaIngestManager::compareCertificatePublicKeys. The reason
      //that MapLink, through the media ingest manager, doesn't perform this test
      //it self is that the media ingest manager doesn't explore the exchange set,
      //instead it relies on the OEM providing the paths the relevant files on
      //the exchange set. Equally, the media ingest manager should never use
      //the certificate held on the exchange set for any other purpose.

      //For the purposes of this test case however the below test performs a
      //check to ensure that were the OEM to validate certificates, the
      //TSLS63MediaIngestManager::compareCertificatePublicKeys function
      //works
      //-------------------------------------------------------------------------------

      //Clear the error stack, we will need to check for errors
      //raise during the comparison.
      TSLThreadedErrorStack::clear();

      //Construct the path to the valid certificate
      string validCertificate(m_test3_1);
      validCertificate += "IHO.CRT";

      //Construct the path to the invalid certificate
      string invalidCertificate(m_test3_3);
      invalidCertificate += "Erroneous_IHO.CRT";

      //Ask MapLink to compare the public keys of the two certificates
      const bool match = TSLS63MediaIngestManager::compareCertificatePublicKeys(validCertificate.c_str(), invalidCertificate.c_str());

      //they shouldn't match
      if ( match )
      {
        return false;
      }

      //false may also be returned if the ::compareCertificatePublicKeys function raised an error during the 
      //comparison so we need to check that the error stack is still clear
      if ( TSLThreadedErrorStack::size() )
      {
        return false;
      }

      //Now just to prove the function is working correctly
      return TSLS63MediaIngestManager::compareCertificatePublicKeys(validCertificate.c_str(), validCertificate.c_str());
    }

    //Test 3.4 – Data Client Identifies Erroneous SA Public Key
    bool S63CertificateTests::test3() const
    {
      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test3script, m_encryptedENC_5_1, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_cellPermit_2_1.c_str(), &dataClient );

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

      //This is the path to the invalid IHO certificate
      string invalidCertificatePath ( m_test3_3 ) ;
      invalidCertificatePath += "Erroneous_IHO.CRT";

      //Constuct the media ingest manager
      //passing in the data client, the path to the invalid SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, invalidCertificatePath.c_str(), &hardwareID );
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
      string productsPath( m_encryptedENC_5_1 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_encryptedENC_5_1 );
      serialPath += "ENC_ROOT/SERIAL.ENC";

      //Load the SERIAL.ENC file
      TSLS63SerialENCFile *file = TSLS63SerialENCFile::create( serialPath.c_str() );

      //Ask the manager to the load the exchange set, passing in the path to the
      //PRODUCTS.TXT file from the exchange set, the loaded SERIAL.ENC file and the path to
      //the certificate. In this case the path to the certificate is the same as the path
      //to the invalid SA's certificate passed during construction of the manager.
      if ( !manager->loadExchangeSet( productsPath.c_str(), file, invalidCertificatePath.c_str() ) )
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

    //Test 3.6 – Authenticate Data Server Certificate in ENC Signature File
    bool S63CertificateTests::test4() const
    {
      //-------------------------------------------------------------------------
      //If the data is ingest correctly then the Data servers public key matches
      //MapLink doesn't provide access to the data server public key however
      //-------------------------------------------------------------------------

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test4script, m_encryptedENC_5_1, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_cellPermit_2_1.c_str(), &dataClient );

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

      //Construct the path to the valid certificiate
      string certificatePath ( m_test3_1 ) ;
      certificatePath += "IHO.CRT";

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificatePath.c_str(), &hardwareID );
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
      string productsPath( m_encryptedENC_5_1 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_encryptedENC_5_1 );
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

    //Test 3.7 - Identify Erroneous Data Server Certificate in ENC
    bool S63CertificateTests::test5() const
    {
      //----------------------------------------------------------
      //The 3.6 directory contains the 5.1 ingest set but with the 
      //enc signature files replaced with the erroneous ones
      //----------------------------------------------------------

      //Construct an implementation of TSLS63DataClient that operates from an
      //ini file. The ini instructs which calls should be received, the order
      //that they should be received and the responses that should be returned.
      //This means that the test can be changed without modifying too much of the code
      //but also that a different data client implementation doesn't have to be 
      //written for each test.
      ScriptedS63DataClient dataClient(m_test5script, m_test3_6, "", m_csys, false);

      //Load the first PERMIT.TXT file. Pass in the data client so that if any S63 mandated errors occur
      //that we detect them and fail this test
      TSLS63CellPermitCollection * collection = TSLS63CellPermitCollection::create( m_cellPermit_2_1.c_str(), &dataClient );

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

      //Construct the path to the valid certificiate
      string certificatePath ( m_test3_1 ) ;
      certificatePath += "IHO.CRT";

      //Constuct the media ingest manager
      //passing in the data client, the path to the SA's certificate
      //and the hw_id
      TSLS63MediaIngestManager *manager = new TSLS63MediaIngestManager( &dataClient, certificatePath.c_str(), &hardwareID );
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
      string productsPath( m_test3_6 );
      productsPath += "INFO/PRODUCTS.TXT";

      //Build the path to the SERIAL.ENC file
      string serialPath( m_test3_6 );
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

    bool S63CertificateTests::test6() const
    {
      //----------------------------------------------------
      // This test first queries the expiry date from the CRT file
      // then makes sure that the current time falls betweeen the
      // valid from and expiry dates
      //----------------------------------------------------

      //Construct the certicate parth
      string certificateLocation( m_test3_1 + "/IHO.CRT" );

      //Storage for the valid from and expiry dates
      TSLTimeType validFromDate = 0, expiryDate = 0;

      //Get the current time
      // Certificate has expired and there is no new one to use
      TSLTimeType currentTime = 1376819995; // 8/18/13 9:59:55 GMT;
      //_time64( &currentTime );
      TSLSimpleString issuer;

      //Ask MapLink to extract the valid from and expiry date from the certificate
      if( !TSLS63MediaIngestManager::queryCertificate( certificateLocation.c_str(), validFromDate, expiryDate, &issuer ) )
      {
        //If MapLink failed to load the certificate, return failure
        return false;
      }

      //Make sure the returned information is valid
      if( validFromDate == 0 || expiryDate == 0 || expiryDate <= validFromDate )
      {
        return false;
      }

      //Ask MapLink to validate that the current time falls between the valid from and expiry dates
      if ( !TSLS63MediaIngestManager::validateCertificateTimes(currentTime, validFromDate, expiryDate) )
      {
        return false;
      }

      //No errors were encountered so return true
      return true;
    }

  };
};