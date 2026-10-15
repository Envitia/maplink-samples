#ifndef _S63CELLPERMITTESTS_H_
#define _S63CELLPERMITTESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63CellPermitTests : public S63TestBase
    {
    public:

      /*
      Description:
      Cell Permit Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63CellPermitTests(const S63ConfigurationObject &config);
      ~S63CellPermitTests();

      /*
      Test ID:
      1.1.1

      Test Name:
      Test 2.2 – Decrypt Cell Permit

      Reference 1, Section 5.3.2
      */
      bool test1() const;

      /*
      Test ID:
      1.1.2

      Test Name:
      Test 2.3 – Decrypt Erroneous Cell Permit

      Reference 1, Section 5.3.3
      */
      bool test2() const;

      /*
      Test ID:
      1.1.3

      Test Name:
      Test 2.4 – Check Expiry Date in Cell Permit

      Reference 1, Section 5.3.4
      */
      bool test3() const;

    private:

      S63CellPermitTests(); // not required

      bool test1( const string& permitLocation, const string & dataLocation ) const;
      bool test2( const string& permitLocation, const string & dataLocation ) const;
      bool test3( const string& permitLocation, const string & dataLocation ) const;

      void readConfiguration(const string &configFile);

      //user information
      string m_hw_id;

      //ds
      string m_dsID;

      //basic cell permits
      string m_cellPermit_2_1;
      string m_cellPermitExtended_2_1;
      string m_cellPermit_2_2;
      string m_cellPermitExtended_2_2;

      //incorrect permits
      string m_erroneousPermit;
      string m_erroneousPermitExtended;
      string m_expiredPermit;
      string m_expiredPermitExtended;

      //enc
      string m_encryptedENC_5_1;
      string m_unencryptedENC_5_1;
      string m_encryptedENC_5_3;
      string m_unencryptedENC_5_3;

      string m_test1script;
      string m_test2script;
      string m_test3script;

      const TSLCoordinateSystem* m_csys;
    };

  };
};

#endif