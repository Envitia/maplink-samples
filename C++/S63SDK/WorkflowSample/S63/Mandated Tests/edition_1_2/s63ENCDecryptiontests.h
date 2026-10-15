#ifndef S63_ENCDECRYPTIONTESTS_H
#define S63_ENCDECRYPTIONTESTS_H
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    class S63ENCDecryptionTests : public S63TestBase
    {
    public:

      /*
      Description:
      ENC Decryption

      References:
      1. Test definitions and scripts Edition 1.2
      */
      explicit S63ENCDecryptionTests(const S63ConfigurationObject &config);
      ~S63ENCDecryptionTests();

      /*
      Test ID: 6a

      Test Name:
      Test 6a - Install ENCs when pre-installed permits have expired

      Reference 1, Section 5
      */
      bool test6a();

      /*
      Test ID: 6b

      Test Name:
      Test 6b - Permit expiry within 30 days

      Reference 1, Section 5
      */
      bool test6b();

      /*
      Test ID: 6c

      Test Name:
      Test 6c - Incorrect cell keys encrypted in the ENC permits
      
      Reference 1, Section 5
      */
      bool test6c();

      /*
      Test ID: 6d

      Test Name:
      Test 6d - Validate ENC data file integrity

      Reference 1, Section 5
      */
      bool test6d();
      
    private:

      S63ENCDecryptionTests(); // not required

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