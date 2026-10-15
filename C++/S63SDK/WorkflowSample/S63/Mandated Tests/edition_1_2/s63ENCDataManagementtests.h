#ifndef S63_ENCDATAMANAGEMENTTESTS_H
#define S63_ENCDATAMANAGEMENTTESTS_H
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"
#include "ScriptedS63DataClient.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    class S63ENCDataManagementTests : public S63TestBase
    {
    public:

      /*
      Description:
      ENC Data Management

      References:
      1. Test definitions and scripts Edition 1.2
      */
      explicit S63ENCDataManagementTests(const S63ConfigurationObject &config);
      ~S63ENCDataManagementTests();

      /*
      Test ID: 7a

      Test Name:
      Test 7a - Encrypted ENCs supplied by different data servers

      Reference 1, Section 5
      */
      bool test7a();
      // Test 7a implementation is split to another method.
      // Test 7b needs to run test 7a first to setup the test environment.
      bool loadTest7a( TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* configSection, const char* dataStorePath );

      /*
      Test ID: 7b

      Test Name:
      Test 7b - Loading additional ENC cell permits and cells from a different data provider

      Reference 1, Section 5
      */
      bool test7b();

      /*
      Test ID: 7c

      Test Name:
      Test 7c - Test that the system operates correctly in a multiple data provider environment

      Reference 1, Section 5
      */
      bool test7c();

      /*
      Test ID: 7d

      Test Name:
      Test 7d - ECDIS management of cancelled cells

      Reference 1, Section 5
      */
      bool test7d();
      // Test 7d implementation is split to another method.
      // Test 7e needs to run test 7d first to setup the test environment.
      bool S63ENCDataManagementTests::loadTest7d( TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* configSection, const char* dataStorePath );

      /*
      Test ID: 7e

      Test Name:
      Test 7e - ECDIS Display of replacement cells

      Reference 1, Section 5
      */
      bool test7e();

      /*
      Test ID: 7f

      Test Name:
      Test 7f - ECDIS Management of ENC re-issued cells

      Reference 1, Section 5
      */
      bool test7f();

      /*
      Test ID: 7g

      Test Name:
      Test 7g - ECDIS Management of Base and Update Exchange Sets

      Reference 1, Section 5
      */
      bool test7g();

    private:

      S63ENCDataManagementTests(); // not required

      void readConfiguration(const std::string &configFile);

      // Test data paths
      std::string m_configDir;
      std::string m_configFile;
      std::string m_scriptsDir;

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