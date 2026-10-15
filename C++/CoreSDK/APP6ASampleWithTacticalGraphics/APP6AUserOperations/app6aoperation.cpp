//## begin module%1.4%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.4%.codegen_version

//## begin module%403C8C980103.cm preserve=no
//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************
//## end module%403C8C980103.cm

//## begin module%403C8C980103.cp preserve=no
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
//  MODULE      : app6aoperation.cpp
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 12:05:04
//  ********************************************************
//## end module%403C8C980103.cp

//## Module: APP6AOperation%403C8C980103; Pseudo Package body
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6aoperation.cpp

//## begin module%403C8C980103.additionalIncludes preserve=no
//## end module%403C8C980103.additionalIncludes

//## begin module%403C8C980103.includes preserve=yes
#include "stdafx.h"
#include "app6aoperation.h"
#include "../App6aSample.h"
//## end module%403C8C980103.includes

// APP6AOperation #include "app6aoperation.h"
//## begin module%403C8C980103.additionalDeclarations preserve=yes
TSLAPP6AHelper* APP6AOperation::m_sharedHelper = NULL;
//## end module%403C8C980103.additionalDeclarations


// Class APP6AOperation 

APP6AOperation::APP6AOperation ()
  //## begin APP6AOperation::APP6AOperation%403C8CC30001.hasinit preserve=no
  //## end APP6AOperation::APP6AOperation%403C8CC30001.hasinit
  //## begin APP6AOperation::APP6AOperation%403C8CC30001.initialization preserve=yes
  //## end APP6AOperation::APP6AOperation%403C8CC30001.initialization
{
  //## begin APP6AOperation::APP6AOperation%403C8CC30001.body preserve=yes

  // By default the helper loads the app6aConfig.csv file
  // at your maplink home/config directory. 
  // This sample demonstrates how to specify the particular symbols 
  // file you wish to use. 
  // This is done each time you create a new instance of the class.
  // There is no enforced reference counting within the internal
  // helper, thus it is best if you implement a simple
  // count yourself. This is demonstrated below.

  if ( m_count == 0 )
  {
    TSLErrorStack::clear() ;
    const char * mlh = TSLUtilityFunctions::getMapLinkHome() ;
    if ( !mlh )
    {
      exit(1);
    }

    string symbolCSVFile( mlh );
    // Tell the TSLAPP6AHelper class which Symbols file to use
    // If 2525B symbols are required, use one of the other files listed.
    symbolCSVFile += "/config/app6aConfig.csv" ;
//    symbolCSVFile += "/config/2525bunfilledConfig.csv" ;
//    symbolCSVFile += "/config/2525bConfig.csv" ;
    m_sharedHelper = new TSLAPP6AHelper( symbolCSVFile.c_str() ) ;
    
    // Check for errors loading the config file.
    if ( !m_sharedHelper->valid() )
    {
      const char * msg = TSLErrorStack::errorString() ;
      if ( msg )
        AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg) ) ;
    }
  }
  
  // Point the helper for this operation at the globally shared helper
  m_helper = m_sharedHelper;

  ++m_count ;
  //## end APP6AOperation::APP6AOperation%403C8CC30001.body
}


APP6AOperation::~APP6AOperation ()
{
  //## begin APP6AOperation::~APP6AOperation%403C8CC500C2.body preserve=yes
  --m_count ;
  if ( m_count == 0 )
  {  
    // Destroy only when no-one requires it any more.
    // In our case this will happen when the application shuts
    // down.
    m_sharedHelper->destroy() ;
    m_sharedHelper = 0 ;
  }
  //## end APP6AOperation::~APP6AOperation%403C8CC500C2.body
}



//## Other Operations (implementation)
TSLAPP6AHelper * APP6AOperation::getHelper ()
{
  //## begin APP6AOperation::getHelper%403C8CE802FD.body preserve=yes
  return m_helper ;
  //## end APP6AOperation::getHelper%403C8CE802FD.body
}

bool APP6AOperation::onHelp (unsigned long flags, bool help, bool f1)
{
  //## begin APP6AOperation::onHelp%403C8E1A0203.body preserve=yes

  // Forward on help calls to HtmlHelp - not supported in Visual C++ 6
#if _MSC_VER > 1200
  CAPP6ASampleApp::getAPP ()->HtmlHelp ( flags ) ;
#endif
  return true ;

  //## end APP6AOperation::onHelp%403C8E1A0203.body
}

// Additional Declarations
  //## begin APP6AOperation%403C8C980103.declarations preserve=yes
int APP6AOperation::m_count = 0 ;
//## end APP6AOperation%403C8C980103.declarations
//## begin module%403C8C980103.epilog preserve=yes
//## end module%403C8C980103.epilog
