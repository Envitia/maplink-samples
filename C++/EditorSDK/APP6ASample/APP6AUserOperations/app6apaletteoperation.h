//## begin module%1.4%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.4%.codegen_version

//## begin module%403C7C770373.cm preserve=no
//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************
//## end module%403C7C770373.cm

//## begin module%403C7C770373.cp preserve=no
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
//  MODULE      : app6apaletteoperation.h
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 12:08:33
//  ********************************************************
//## end module%403C7C770373.cp

//## Module: APP6APaletteOperation%403C7C770373; Pseudo Package specification
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6apaletteoperation.h

#ifndef app6apaletteoperation_h
#define app6apaletteoperation_h 1

//## begin module%403C7C770373.additionalIncludes preserve=no
//## end module%403C7C770373.additionalIncludes

//## begin module%403C7C770373.includes preserve=yes
#include "app6aoperation.h"
//## end module%403C7C770373.includes

//## begin module%403C7C770373.additionalDeclarations preserve=yes
class TSLAPP6AHelper ;
class TSLAPP6APalette ;
class TSLAPP6ARequest ;
//## end module%403C7C770373.additionalDeclarations


//## begin APP6APaletteOperation%403C7C770373.preface preserve=yes
//## end APP6APaletteOperation%403C7C770373.preface

//## Class: APP6APaletteOperation%403C7C770373
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class APP6APaletteOperation : public APP6AOperation  //## Inherits: <unnamed>%403C8DB20267
{
  //## begin APP6APaletteOperation%403C7C770373.initialDeclarations preserve=yes
  //## end APP6APaletteOperation%403C7C770373.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: APP6APaletteOperation%403C7D8101D1
      APP6APaletteOperation ();

    //## Destructor (specified)
      //## Operation: ~APP6APaletteOperation%403C7D850082
      virtual ~APP6APaletteOperation ();


    //## Other Operations (specified)
      //## Operation: activate%403C7D9F012A
      int activate (void *data);

      //## Operation: done%403C7DBD0278
      int done ();

      //## Operation: locator%403C7DCF00B1
      int locator (long x, long y, TSLLocatorType type, TSLButtonType button, TSLModifierType modifier);

      //## Operation: requestHandler%403C7E370092
      void requestHandler (TSLUserOperationRequest *handler);

      //## Operation: onPaletteClose%403C8C61014A
      void onPaletteClose (unsigned long flags);

    // Additional Public Declarations
      //## begin APP6APaletteOperation%403C7C770373.public preserve=yes
      APP6APaletteOperation(TSLAPP6ARequest*additionalPropertiesCallback );

      bool onPropertiesUpdate( unsigned long flags, const TSLAPP6ASymbol *symbol ) ; 
      //## end APP6APaletteOperation%403C7C770373.public

  protected:
    // Additional Protected Declarations
      //## begin APP6APaletteOperation%403C7C770373.protected preserve=yes
      //## end APP6APaletteOperation%403C7C770373.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: handler%403C7E7F00BE
      //## begin APP6APaletteOperation::handler%403C7E7F00BE.attr preserve=no  private: TSLUserOperationRequest * {UA} 0
      TSLUserOperationRequest *m_handler;
      //## end APP6APaletteOperation::handler%403C7E7F00BE.attr

      //## Attribute: dlg%403C80B803AB
      //## begin APP6APaletteOperation::dlg%403C80B803AB.attr preserve=no  private: TSLAPP6APalette * {UA} 0
      TSLAPP6APalette *m_dlg;
      //## end APP6APaletteOperation::dlg%403C80B803AB.attr

    // Additional Private Declarations
      //## begin APP6APaletteOperation%403C7C770373.private preserve=yes
      TSLAPP6ARequest*m_additionalPropertiesCallback;
      //## end APP6APaletteOperation%403C7C770373.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin APP6APaletteOperation%403C7C770373.implementation preserve=yes
      //## end APP6APaletteOperation%403C7C770373.implementation

};

//## begin APP6APaletteOperation%403C7C770373.postscript preserve=yes
//## end APP6APaletteOperation%403C7C770373.postscript

// Class APP6APaletteOperation 

//## begin module%403C7C770373.epilog preserve=yes
//## end module%403C7C770373.epilog


#endif
