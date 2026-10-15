//## begin module%1.4%.codegen_version preserve=yes
//   Read the documentation to learn more about C++ code generator
//   versioning.
//## end module%1.4%.codegen_version

//## begin module%403C7FB000B2.cm preserve=no
//	********************************************************
//	*           Source Control Information
//	********************************************************
//	$Archive: $
//	$Author$
//	$Date$
//	$Revision$
//	********************************************************
//## end module%403C7FB000B2.cm

//## begin module%403C7FB000B2.cp preserve=no
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************
//  MODULE      : app6apropertiesoperation.h
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 15:53:02
//  ********************************************************
//## end module%403C7FB000B2.cp

//## Module: APP6APropertiesOperation%403C7FB000B2; Pseudo Package specification
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6apropertiesoperation.h

#ifndef app6apropertiesoperation_h
#define app6apropertiesoperation_h 1

//## begin module%403C7FB000B2.additionalIncludes preserve=no
//## end module%403C7FB000B2.additionalIncludes

//## begin module%403C7FB000B2.includes preserve=yes
#include "app6aoperation.h"
//## end module%403C7FB000B2.includes

//## begin module%403C7FB000B2.additionalDeclarations preserve=yes
class TSLEntitySet ;
class TSLAPP6AHelper ;
class TSLAPP6AProperties ;
class TSLAPP6ARequest ;
//## end module%403C7FB000B2.additionalDeclarations


//## begin APP6APropertiesOperation%403C7FB000B2.preface preserve=yes
//## end APP6APropertiesOperation%403C7FB000B2.preface

//## Class: APP6APropertiesOperation%403C7FB000B2
//## Category: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n



class APP6APropertiesOperation : public APP6AOperation  //## Inherits: <unnamed>%403C8DB802E8
{
  //## begin APP6APropertiesOperation%403C7FB000B2.initialDeclarations preserve=yes
  //## end APP6APropertiesOperation%403C7FB000B2.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: APP6APropertiesOperation%403C7FEE02EC
      APP6APropertiesOperation ();

    //## Destructor (specified)
      //## Operation: ~APP6APropertiesOperation%403C7FF10214
      ~APP6APropertiesOperation ();


    //## Other Operations (specified)
      //## Operation: activate%403C7FF60280
      int activate (void *data);

      //## Operation: done%403C800C039A
      int done ();

      //## Operation: deactivate%403C80130336
      void deactivate ();

      //## Operation: requestHandler%403C801A0141
      void requestHandler (TSLUserOperationRequest *handler);

      //## Operation: onPropertiesClose%403C966A000E
      void onPropertiesClose (unsigned long flags);

      //## Operation: onPropertiesUpdate%403C9F6E0201
      bool onPropertiesUpdate (unsigned long flags, const TSLAPP6ASymbol *symbol);

    // Additional Public Declarations
      //## begin APP6APropertiesOperation%403C7FB000B2.public preserve=yes
      void onPropertiesOpening (unsigned long flags);
      //## end APP6APropertiesOperation%403C7FB000B2.public

  protected:
    // Additional Protected Declarations
      //## begin APP6APropertiesOperation%403C7FB000B2.protected preserve=yes
      //## end APP6APropertiesOperation%403C7FB000B2.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: handler%403C8050024D
      //## begin APP6APropertiesOperation::handler%403C8050024D.attr preserve=no  private: TSLUserOperationRequest * {UA} 0
      TSLUserOperationRequest *m_handler;
      //## end APP6APropertiesOperation::handler%403C8050024D.attr

      //## Attribute: dlg%403C80C60301
      //## begin APP6APropertiesOperation::dlg%403C80C60301.attr preserve=no  private: TSLAPP6AProperties * {UA} 0
      TSLAPP6AProperties *m_dlg;
      //## end APP6APropertiesOperation::dlg%403C80C60301.attr

      //## Attribute: symbols%403C9ED6025D
      //## begin APP6APropertiesOperation::symbols%403C9ED6025D.attr preserve=no  private: std::vector< const TSLEntitySet * > {UA} 
      std::vector< const TSLEntitySet * > m_symbols;
      //## end APP6APropertiesOperation::symbols%403C9ED6025D.attr

    // Additional Private Declarations
      //## begin APP6APropertiesOperation%403C7FB000B2.private preserve=yes
      bool m_propertiesOperationActive ;
      //## end APP6APropertiesOperation%403C7FB000B2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin APP6APropertiesOperation%403C7FB000B2.implementation preserve=yes
      //## end APP6APropertiesOperation%403C7FB000B2.implementation

};

//## begin APP6APropertiesOperation%403C7FB000B2.postscript preserve=yes
//## end APP6APropertiesOperation%403C7FB000B2.postscript

// Class APP6APropertiesOperation 

//## begin module%403C7FB000B2.epilog preserve=yes
//## end module%403C7FB000B2.epilog


#endif
