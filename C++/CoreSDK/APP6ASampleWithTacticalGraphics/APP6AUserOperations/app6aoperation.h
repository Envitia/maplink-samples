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
//  MODULE      : app6aoperation.h
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 12:05:04
//  ********************************************************
//## end module%403C8C980103.cp

//## Module: APP6AOperation%403C8C980103; Pseudo Package specification
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6aoperation.h

#ifndef app6aoperation_h
#define app6aoperation_h 1

//## begin module%403C8C980103.additionalIncludes preserve=no
//## end module%403C8C980103.additionalIncludes

//## begin module%403C8C980103.includes preserve=yes
//## end module%403C8C980103.includes

// TSLAPP6ARequest #include "tslapp6arequest.h"
// TSLUserOperation #include "tsluseroperation.h"
//## begin module%403C8C980103.additionalDeclarations preserve=yes
//## end module%403C8C980103.additionalDeclarations


//## begin APP6AOperation%403C8C980103.preface preserve=yes
//## end APP6AOperation%403C8C980103.preface

//## Class: APP6AOperation%403C8C980103
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n



class APP6AOperation : public TSLUserOperation, //## Inherits: <unnamed>%403C8D3903A4
                       	public TSLAPP6ARequest  //## Inherits: <unnamed>%403C8D3C034E
{
  //## begin APP6AOperation%403C8C980103.initialDeclarations preserve=yes
  //## end APP6AOperation%403C8C980103.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: APP6AOperation%403C8CC30001
      APP6AOperation ();

    //## Destructor (specified)
      //## Operation: ~APP6AOperation%403C8CC500C2
      virtual ~APP6AOperation ();


    //## Other Operations (specified)
      //## Operation: getHelper%403C8CE802FD
      TSLAPP6AHelper * getHelper ();

      //## Operation: onHelp%403C8E1A0203
      bool onHelp (unsigned long flags, bool help, bool f1);

    // Additional Public Declarations
      //## begin APP6AOperation%403C8C980103.public preserve=yes
      //## end APP6AOperation%403C8C980103.public

  protected:
    // Additional Protected Declarations
      //## begin APP6AOperation%403C8C980103.protected preserve=yes
      //## end APP6AOperation%403C8C980103.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: count%403C8E720390
      //## begin APP6AOperation::count%403C8E720390.attr preserve=no  private: static int {UA} 
      static int m_count;
      //## end APP6AOperation::count%403C8E720390.attr

    // Additional Private Declarations
      //## begin APP6AOperation%403C8C980103.private preserve=yes
      //## end APP6AOperation%403C8C980103.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin APP6AOperation%403C8C980103.implementation preserve=yes
      static TSLAPP6AHelper *m_sharedHelper;
      //## end APP6AOperation%403C8C980103.implementation

};

//## begin APP6AOperation%403C8C980103.postscript preserve=yes
//## end APP6AOperation%403C8C980103.postscript

// Class APP6AOperation 

//## begin module%403C8C980103.epilog preserve=yes
//## end module%403C8C980103.epilog


#endif
