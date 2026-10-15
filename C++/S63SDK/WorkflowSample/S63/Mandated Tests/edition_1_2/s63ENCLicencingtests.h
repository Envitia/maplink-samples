#ifndef S63_ENCLICENCINGTESTS_H
#define S63_ENCLICENCINGTESTS_H

//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    class S63ENCLicencingTests : public S63TestBase
    {
    public:

      /*
      Description:
      ENC Licencing - Permit Management

      References:
      1. Test definitions and scripts Edition 1.2
      */
      explicit S63ENCLicencingTests(const S63ConfigurationObject &config);
      ~S63ENCLicencingTests();

      /*
      Test ID: 2a

      Test Name:
      Test 2a - Check permit string is availability

      Reference 1, Section 5
      */
      bool test2a();

      /*
      Test ID: 2b

      Test Name:
      Test 2b - ENC cell permit string incorrect format

      Reference 1, section 5
      */
      bool test2b();

      /*
      Test ID: 2c

      Test Name:
      Test 2c - Validate Permit CRC

      Reference 1, section 5
      */
      bool test2c();

      /*
      Test ID: 2d

      Test Name:
      Test 2d - Check remaining permit expiry period

      Reference 1, section 5
      */
      bool test2d();

      /*
      Test ID: 2e

      Test Name:
      Test 2e - Check for expired permits

      Reference 1, section 5
      */
      bool test2e();

      /*
      Test ID: 2f

      Test Name:
      Test 2f - Permit installation and reporting

      Reference 1, section 5
      */
      bool test2f();

      /*
      Test ID: 2g

      Test Name:
      Test 2g - Management of permits from multiple data servers

      Reference 1, section 5
      */
      bool test2g();

      /*
      Test ID: 2h

      Test Name:
      Test 2h - Management of installed permits

      Reference 1, section 5
      */
      bool test2h();

    private:

      S63ENCLicencingTests();

      void readConfiguration(const string &configFile);

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