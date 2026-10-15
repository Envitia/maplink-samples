#ifndef _S63DIGITALSIGNATURETESTS_H_
#define _S63DIGITALSIGNATURETESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63DigitalSignatureTests : public S63TestBase
    {
    public:

      /*
      Test Group ID:
      S63 Digital Signature Tests

      Description:
      Digital Signature Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63DigitalSignatureTests(const S63ConfigurationObject &configurationObject);

      ~S63DigitalSignatureTests();

      /*
      Test ID:
      1.4.1

      Test Name:
      Test 4.2 – Authenticate ENC signature

      Reference 1, Section 5.5.2
      */
      bool test1() const;

      /*
      Test ID:
      1.4.2

      Test Name:
      Test 4.3 – Identify Erroneous ENC signature

      Reference 1, Section 5.5.3
      */
      bool test2() const;

    private:

      S63DigitalSignatureTests(); // not required

      void readConfiguration(const string &configFile);

      string m_certificatePath;

      //user information
      string m_hw_id;

      string m_test4_3;
      string m_test5_1;

      string m_permits_1;

      string m_test1script;
      string m_test2script;

      const TSLCoordinateSystem* m_csys;
    };

  };
};

#endif