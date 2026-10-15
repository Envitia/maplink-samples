#ifndef _S63ENCRYPTIONDECRYPTIONTESTS_H_
#define _S63ENCRYPTIONDECRYPTIONTESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63EncryptionDecryptionTests : public S63TestBase
    {
    public:

      /*
      Test Group ID:
      S63 Encryption/Decryption Tests

      Description:
      Encryption & Decryption Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63EncryptionDecryptionTests(const S63ConfigurationObject &configurationObject);

      ~S63EncryptionDecryptionTests();

      /*
      Test ID:
      1.5.1

      Test Name:
      Test 5.2 - Decrypt ENC Base Cells and Update Files (Part 1)

      Reference 1, Section 5.6.2
      */

      bool test1() const;

      /*
      Test ID:
      1.5.2

      Test Name:
      Test 5.2 - Decrypt ENC Base Cells and Update Files (Part 2)

      Reference 1, Section 5.6.2
      */

      bool test2() const;

      /*
      Test ID:
      1.5.3

      Test Name:
      Test 5.4 - Decrypt Next Edition of ENC with Next Cell Key

      Reference 1, Section 5.6.4
      */
      bool test3() const;

      /*
      Test ID:
      1.5.4

      Test Name:
      Test 5.5 - Decrypt Erroneous ENC

      Reference 1, Section 5.6.5
      */
      bool test4() const;

    private:

      S63EncryptionDecryptionTests(); // not required

      void readConfiguration(const string &configFile);

      string m_certificatePath;

      //user information
      string m_hw_id;

      string m_comparisonDir1;
      string m_comparisonDir2;

      string m_test5_1;
      string m_test5_3;
      string m_test5_5;

      string m_cellPermit1;
      string m_cellPermit2;

      string m_test1script;
      string m_test2script;
      string m_test3script;
      string m_test4script;

      const TSLCoordinateSystem* m_csys;
    };

  };
};

#endif