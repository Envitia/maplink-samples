#ifndef _S63MULTIPLESERVERTESTS_H_
#define _S63MULTIPLESERVERTESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63MultipleServerTests : public S63TestBase
    {
    public:

      /*
      Test Group ID:
      S63 Multiple Data Server Tests

      Description:
      Multiple Server Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63MultipleServerTests(const S63ConfigurationObject &config);

      ~S63MultipleServerTests();

      /*
      Test ID:
      1.6.1

      Test Name:
      Test 7.1 – Test installation of cells from multiple data servers

      Reference 1, Section 5.8.1
      */

      bool test1();

      /*
      Test ID:
      1.6.2

      Test Name:
      Test 7.2 – Test installation of cells from multiple data servers

      Reference 1, Section 5.8.2
      */
      bool test2();

    private:

      S63MultipleServerTests(); // not required

      void readConfiguration(const string& configFile);

      string m_certificatePath;

      //user information
      string m_hw_id;

      //Certificate test data
      string m_exchangeSet_1;
      string m_exchangeSet_2;
      string m_exchangeSet_3;
      string m_exchangeSet_4;
      string m_exchangeSet_5;

      //data client scripts
      string m_test1script;
      string m_test2script;

      //cell permits
      string m_permit_1;
      string m_permit_2;
      string m_permit_3;

      TSLCoordinateSystem* m_csys;
    };

  };
};

#endif