#ifndef S63_ENCDATAEXCHANGEMEDIA_H
#define S63_ENCDATAEXCHANGEMEDIA_H
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"
#include "ScriptedS63DataClient.h"

namespace S63OEMTests
{
  namespace Edition_1_2
  {

    class S63ENCDataExchangeMediaTests : public S63TestBase
    {
    public:

      /*
      Description:
      Data Exchange Media

      References:
      1. Test definitions and scripts Edition 1.2
      */
      explicit S63ENCDataExchangeMediaTests(const S63ConfigurationObject &config);
      ~S63ENCDataExchangeMediaTests();

      /*
      Test ID: 8a

      Test Name:
      Test 8a - Exchange Set and Media Delivery

      Reference 1, Section 5
      */
      bool test8a();

      /*
      Test ID: 8b

      Test Name:
      Test 8b - Single Media containing Multiple Exchange Sets

      Reference 1, Section 5
      */
      bool test8b();

      /*
      Test ID: 8c

      Test Name:
      Test 8c - Multiple exchange sets across multiple media sets

      Reference 1, Section 5
      */
      bool test8c();

      /*
      Test ID: 8d

      Test Name:
      Test 8d - Media validation of encrypted ENC service status

      Reference 1, Section 5
      */
      bool test8d();

    private:

      S63ENCDataExchangeMediaTests(); // not required

      void  readConfiguration(const string &configFile);

      bool loadTest8c( TSLS63SerialENCFile *&file, TSLS63MediaIngestManager *&manager, ScriptedS63DataClient *&dataClient, const char* dataStorePath );

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