#ifndef S63ENCAUTHENTICATIONTESTS_PART2_H
#define S63ENCAUTHENTICATIONTESTS_PART2_H
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"
#include "ScriptedS63DataClient.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    class S63ENCAuthenticationTests_Part2 : public S63TestBase
    {
    public:

      /*
      Description:
      ENC Licencing - Permit Management

      References:
      1. Test definitions and scripts Edition 1.2
      */
      explicit S63ENCAuthenticationTests_Part2(const S63ConfigurationObject &config);
      ~S63ENCAuthenticationTests_Part2();

      /*
      Test ID: 5a

      Test Name:
      Test 5a - Invalid SA signature in the ENC Signature File

      Reference 1, Section 5
      */
      bool test5a();

     /*
      Test ID: 5b

      Test Name:
      Test 5b - Authentication against a non SA certificate/public key

      Reference 1, Section 5
      */
      bool test5b();

     /*
      Test ID: 5c

      Test Name:
      Test 5c - ENC signature validation

      Reference 1, Section 5
      */
      bool test5c();

     /*
      Test ID: 5d

      Test Name:
      Test 5d - ENC signature format validation

      Reference 1, Section 5
      */
      bool test5d();

     /*
      Test ID: 5e

      Test Name:
      Test 5e - Check authentication is continuous and complete

      Reference 1, Section 5
      */
      bool test5e();

     /*
      Test ID: 5f

      Test Name:
      Test 5f - Single exchange set with ENC signature files signed by multiple data servers

      Reference 1, Section 5
      */
      bool test5f();


    private:

      S63ENCAuthenticationTests_Part2(); // not required

      void  readConfiguration(const string &configFile);

      bool loadTest5c( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath );
      bool loadTest5d( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath );

      // Test data paths
      std::string m_configDir;
      std::string m_configFile;
      std::string m_scriptsDir;
      std::string m_certificatePath;

      // User information
      std::string m_hw_id;

      // Data Server ID
      std::string m_dsID;

      // Coordinate system used by the scripted data client
      const TSLCoordinateSystem* m_csys;
    };

  };
};

#endif