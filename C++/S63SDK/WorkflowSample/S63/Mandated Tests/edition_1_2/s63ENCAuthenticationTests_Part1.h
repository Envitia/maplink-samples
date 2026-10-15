#ifndef S63_ENCAUTHENTICATIONTESTS_PART1_H
#define S63_ENCAUTHENTICATIONTESTS_PART1_H
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"
#include "ScriptedS63DataClient.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    class S63ENCAuthenticationTests_Part1 : public S63TestBase
    {
    public:

      /*
      Description:
      ENC Licencing - Permit Management

      References:
      1. Test definitions and scripts Edition 1.2
      */
      explicit S63ENCAuthenticationTests_Part1(const S63ConfigurationObject &config);
      ~S63ENCAuthenticationTests_Part1();

      /*
      Test ID: 4a

      Test Name:
      Test 4a - Install and validate the SA certificate and/or public key

      Reference 1, Section 5
      */
      bool test4a();

      /*
      Test ID: 4b

      Test Name:
      Test 4b - Change and update installed certificate

      Reference 1, Section 5
      */
      bool test4b();

      /*
      Test ID: 4c

      Test Name:
      Test 4c - No pre-installed certificate/public key on the system

      Reference 1, Section 5
      */
      bool test4c();

      /*
      Test ID: 4d

      Test Name:
      Test 4d - Check SA Certificate Expiry Date

      Reference 1, Section 5
      */
      bool test4d();

      /*
      Test ID: 4e

      Test Name:
      Test 4e - Incorrectly formatted certificate and public key files.

      Reference 1, Section 5
      */
      bool test4e();

      /*
      Test ID: 4f

      Test Name:
      Test 4f - Check certificate parameter values

      Reference 1, Section 5
      */
      bool test4f();



    private:

      S63ENCAuthenticationTests_Part1();

      void  readConfiguration(const string &configFile);
      bool  loadTest4a( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath );

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