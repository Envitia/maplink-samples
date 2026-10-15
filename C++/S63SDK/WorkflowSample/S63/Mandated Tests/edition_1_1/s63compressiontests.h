#ifndef _S63COMPRESSIONTESTS_H_
#define _S63COMPRESSIONTESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63CompressionTests : public S63TestBase
    {
    public:

      /*
      Test Group ID:
      S63 Compression Tests

      Description:
      Compression Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63CompressionTests(const S63ConfigurationObject &config);

      ~S63CompressionTests();

      /*
      Test ID:
      1.3.1

      Test Name:
      Test 6.2 - Decompress ENC

      Reference 1, Section 5.7.2
      */

      bool test1();

      /*
      Test ID:
      1.3.2

      Test Name:
      Test 6.3 – Decompress Corrupted ENC

      Reference 1, Section 5.7.3
      */

      bool test2();

    private:

      S63CompressionTests(); // not required

      void readConfiguration(const string &configFile);

      vector<pair<string, string> > m_test1Paths;
      vector<string> m_test2Paths;
    };

  };
};

#endif