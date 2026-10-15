#ifndef _S63USERPERMITTESTS_H_
#define _S63USERPERMITTESTS_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "s63testbase.h"

namespace S63OEMTests
{
  namespace Edition_1_1
  {

    class S63UserPermitTests : public S63TestBase
    {
    public:

      /*
      Test Group ID
      S63 User Permit Tests

      Description:
      User Permit Testing

      Reference:
      1. Test Data Implementation Guide, Edition 1.1 (July 2004)
      */

      explicit S63UserPermitTests(const S63ConfigurationObject & configurationObject);

      virtual ~S63UserPermitTests();

      /*
      Test ID:
      1.7.1

      Test Name:
      Test 1.1 - User Permit Generation

      Reference 1, Section 5.2.1
      */

      bool test1() const;

    private:

      S63UserPermitTests(); // not required

    };
  };
};
#endif