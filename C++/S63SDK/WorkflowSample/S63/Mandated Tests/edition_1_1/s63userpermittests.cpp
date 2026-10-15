//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "stdafx.h"
#include "s63userpermittests.h"


namespace S63OEMTests
{
  namespace Edition_1_1
  {

    S63UserPermitTests::S63UserPermitTests(const S63ConfigurationObject &configurationObject)
      :
    S63TestBase(configurationObject)
    {

    }

    S63UserPermitTests::~S63UserPermitTests()
    {
    }

    bool S63UserPermitTests::test1() const
    {
      //The OEM ID (in ASCII)
      const char * m_id = "10";
      //The OEM Key (in ASCII)
      const char * m_key = "10121";
      //The user's hw_id (in ASCII)
      const char * hw_id_ascii = "12345";

      //Construct the TSLS63HWID class around the user's hw_id string
      TSLS63HWID hw_id(hw_id_ascii);

      //Check that the hw_id is valid (that the characters fall within hex range (0-9)(A-F/a-f)
      if ( !hw_id.isValid() )
      {
        return false;
      }

      //Storage for the user permit
      TSLSimpleString result;

      //Ask MapLink to generate a user permit, making sure that it returns a success value
      if ( TSLS63UserPermitGenerator::generateUserPermit(&hw_id, m_key, m_id, result) != TSLS63UserPermitGeneratorResultSuccess )
      {
        return false;
      }

      //Check that the result is as expected
      return ( result == "66B5CBFDF7E4139D5B6086C23130" );
    }

  };
};