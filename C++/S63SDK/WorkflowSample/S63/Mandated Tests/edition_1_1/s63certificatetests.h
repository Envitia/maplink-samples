#ifndef _S63CERTIFICATETESTS_H_
#define _S63CERTIFICATETESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63CertificateTests : public S63TestBase
    {
    public:

      /*
      Test Group ID:
      S63 Certificate Tests

      Description:
      Certificate Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63CertificateTests(const S63ConfigurationObject &config);
      ~S63CertificateTests();

      /*
      Test ID:
      1.2.1

      Test Name:
      Test 3.2 - Data Client Uses Correct SA Public Key File

      Reference 1, Section 5.4.2
      */
      bool test1() const;

      /*
      Test ID:
      1.2.2

      Test Name:
      Test 3.3 – Data Client Authenticates SA Public Key on CD Against
      Installed SA Public Key

      Reference 1, Section 5.4.3
      */
      bool test2() const;

      /*
      Test ID:
      1.2.3

      Test Name:
      Test 3.4 – Data Client Identifies Erroneous SA Public Key


      Reference 1, Section 5.4.4
      */
      bool test3() const;

      /*
      Test ID:
      1.2.4

      Test Name:
      Test 3.6 – Authenticate Data Server Certificate in ENC Signature File

      Reference 1, Section 5.4.6
      */
      bool test4() const;

      /*
      Test ID:
      1.2.5

      Test Name:
      Test 3.7 - Identify Erroneous Data Server Certificate in ENC
      Signature File

      Reference 1, Section 5.4.7
      */
      bool test5() const;

      /*
      Test ID:
      1.2.6

      Test Name:
      Custom Test - Checking of X509 certificate format and reading of expiry date and
      issuer information.

      Input Data Description:
      X509 Data Server certificate (usually the IHO SA certificate)

      Expected result:
      The certificate can be read.

      Reference: UST1108 - MapLink S63 Test Plan: section 2.6.2.2.1
      */
      bool test6() const;

    private:

      S63CertificateTests() ; // not required

      void readConfiguration(const string &configFile);

      //user information
      string m_hw_id;

      //Certificate test data
      string m_test3_1;
      string m_test3_2;
      string m_test3_3;
      string m_test3_5;
      string m_test3_6;

      //data client scripts
      string m_test3script;
      string m_test4script;
      string m_test5script;

      //cell permits
      string m_cellPermit_2_1;

      //enc
      string m_encryptedENC_5_1;

      const TSLCoordinateSystem* m_csys;
    };

  };
};

#endif
