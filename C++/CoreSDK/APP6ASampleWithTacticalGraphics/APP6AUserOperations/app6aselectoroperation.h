//## begin module%1.4%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.4%.codegen_version

//## begin module%403C7FD20101.cm preserve=no
//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************
//## end module%403C7FD20101.cm

//## begin module%403C7FD20101.cp preserve=no
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
//  MODULE      : app6aselectoroperation.h
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 15:53:10
//  ********************************************************
//## end module%403C7FD20101.cp

//## Module: APP6ASelectorOperation%403C7FD20101; Pseudo Package specification
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6aselectoroperation.h

#ifndef app6aselectoroperation_h
#define app6aselectoroperation_h 1

//## begin module%403C7FD20101.additionalIncludes preserve=no
//## end module%403C7FD20101.additionalIncludes

//## begin module%403C7FD20101.includes preserve=yes
#include "app6aoperation.h"
//## end module%403C7FD20101.includes

//## begin module%403C7FD20101.additionalDeclarations preserve=yes
class TSLAPP6AHelper ;
class TSLAPP6ASelector ;
class TSLAPP6ARequest ;
//## end module%403C7FD20101.additionalDeclarations


//## begin APP6ASelectorOperation%403C7FD20101.preface preserve=yes
//## end APP6ASelectorOperation%403C7FD20101.preface

//## Class: APP6ASelectorOperation%403C7FD20101
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n



class APP6ASelectorOperation : public APP6AOperation  //## Inherits: <unnamed>%403C8DBC01F4
{
  //## begin APP6ASelectorOperation%403C7FD20101.initialDeclarations preserve=yes
  //## end APP6ASelectorOperation%403C7FD20101.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: APP6ASelectorOperation%403C802000D1
      APP6ASelectorOperation ();

    //## Destructor (specified)
      //## Operation: ~APP6ASelectorOperation%403C802200DE
      ~APP6ASelectorOperation ();


    //## Other Operations (specified)
      //## Operation: activate%403C802400AF
      int activate (void *data);

      //## Operation: done%403C80370305
      int done ();

      //## Operation: requestHandler%403C8045017F
      void requestHandler (TSLUserOperationRequest *handler);

      //## Operation: onSelectorClose%403C9681018E
      void onSelectorClose (unsigned long flags);

      //## Operation: getFilePath%403CAD3F02B2
      void getFilePath (CString &path, bool openDlg);

      //## Operation: onSelectorUpdate%403CADDE0379
      bool onSelectorUpdate (unsigned long flags, const TSLAPP6ASymbol *symbol);

    // Additional Public Declarations
      //## begin APP6ASelectorOperation%403C7FD20101.public preserve=yes
      void deactivate( ) ;
      //## end APP6ASelectorOperation%403C7FD20101.public

  protected:
    // Additional Protected Declarations
      //## begin APP6ASelectorOperation%403C7FD20101.protected preserve=yes
      //## end APP6ASelectorOperation%403C7FD20101.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: handler%403C805D036E
      //## begin APP6ASelectorOperation::handler%403C805D036E.attr preserve=no  private: TSLUserOperationRequest * {UA} 0
      TSLUserOperationRequest *m_handler;
      //## end APP6ASelectorOperation::handler%403C805D036E.attr

      //## Attribute: dlg%403C80D603E0
      //## begin APP6ASelectorOperation::dlg%403C80D603E0.attr preserve=no  private: TSLAPP6ASelector * {UA} 0
      TSLAPP6ASelector *m_dlg;
      //## end APP6ASelectorOperation::dlg%403C80D603E0.attr

      //## Attribute: stream%403CAEAE03BE
      //## begin APP6ASelectorOperation::stream%403CAEAE03BE.attr preserve=no  private: TSLofstream {UA} 
      TSLofstream m_stream;
      //## end APP6ASelectorOperation::stream%403CAEAE03BE.attr

    // Additional Private Declarations
      //## begin APP6ASelectorOperation%403C7FD20101.private preserve=yes
      bool m_selectorOperationActive ;
      //## end APP6ASelectorOperation%403C7FD20101.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin APP6ASelectorOperation%403C7FD20101.implementation preserve=yes
      //## end APP6ASelectorOperation%403C7FD20101.implementation

};

//## begin APP6ASelectorOperation%403C7FD20101.postscript preserve=yes
//## end APP6ASelectorOperation%403C7FD20101.postscript

// Class APP6ASelectorOperation 

//## begin module%403C7FD20101.epilog preserve=yes
//## end module%403C7FD20101.epilog


#endif
