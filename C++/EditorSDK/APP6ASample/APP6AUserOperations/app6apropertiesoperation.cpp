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
//  MODULE      : app6apropertiesoperation.cpp
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 15:53:02
//  ********************************************************
//## end module%403C7FB000B2.cp

//## Module: APP6APropertiesOperation%403C7FB000B2; Pseudo Package body
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6apropertiesoperation.cpp

//## begin module%403C7FB000B2.additionalIncludes preserve=no
//## end module%403C7FB000B2.additionalIncludes

//## begin module%403C7FB000B2.includes preserve=yes
#include "stdafx.h"
#include "app6apropertiesoperation.h"
//## end module%403C7FB000B2.includes

//## begin module%403C7FB000B2.additionalDeclarations preserve=yes
#define AP_APP6A_PALETTE 300
//## end module%403C7FB000B2.additionalDeclarations


// Class APP6APropertiesOperation 

APP6APropertiesOperation::APP6APropertiesOperation ()
  //## begin APP6APropertiesOperation::APP6APropertiesOperation%403C7FEE02EC.hasinit preserve=no
      : m_handler(0),
        m_dlg(0)
  //## end APP6APropertiesOperation::APP6APropertiesOperation%403C7FEE02EC.hasinit
  //## begin APP6APropertiesOperation::APP6APropertiesOperation%403C7FEE02EC.initialization preserve=yes
  , m_propertiesOperationActive( false ) 
  //## end APP6APropertiesOperation::APP6APropertiesOperation%403C7FEE02EC.initialization
{
  //## begin APP6APropertiesOperation::APP6APropertiesOperation%403C7FEE02EC.body preserve=yes
  //## end APP6APropertiesOperation::APP6APropertiesOperation%403C7FEE02EC.body
}


APP6APropertiesOperation::~APP6APropertiesOperation ()
{
  //## begin APP6APropertiesOperation::~APP6APropertiesOperation%403C7FF10214.body preserve=yes
  delete m_dlg ;
  m_dlg = 0 ;
  //## end APP6APropertiesOperation::~APP6APropertiesOperation%403C7FF10214.body
}



//## Other Operations (implementation)
int APP6APropertiesOperation::activate (void *data)
{
  //## begin APP6APropertiesOperation::activate%403C7FF60280.body preserve=yes
  m_propertiesOperationActive = true ;

  if ( !m_dlg )
  {
    
    TSLErrorStack::clear() ;

    CWnd * parent = (CWnd*) data ;
    HWND wnd = parent->GetSafeHwnd () ;
    m_dlg = new TSLAPP6AProperties ( this, false, wnd ) ;

    if ( !m_dlg->valid() )
    {
      // The create may have failed in the constructor so
      // we need to check the valid() method for this. The app
      // won't fall over, it just won't do anything.
      const char * msg = TSLErrorStack::errorString() ;
      if ( msg )
        AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg) ) ;
    }
  }

  if ( m_dlg )
    m_dlg->showWindow( true ) ;

    // First clear any existing symbols.
  m_symbols.clear() ;

  if ( m_dlg )
    m_dlg->clear() ;

  int num = m_handler->getNumSelected () ;

  for ( int ii = 0 ; ii < num ; ++ii )
  {
    TSLEntity * ent = m_handler->getSelection ( ii ) ;

    const TSLEntitySet * es = TSLEntitySet::isEntitySet (ent) ;
    
    if ( !es )
      continue ;
    
    // Example of how to use the TSLAPP6AHelper::isAPP6A() method,
    // this is not actually required because this property is checked
    // before the symbol is loaded.
    if ( !getHelper()->isAPP6A( es ) )
      continue ;

    // Load up the symbol. We are not clearing the 
    // list because we may want to load mulitple symbols, and
    // adjust there placement size from the properties dialog. This
    // feature is largely obsolete now because when we get the 
    // symbols back from the update, we can adjust there size
    // then.
    if ( !m_dlg || !m_dlg->load( es, false ) )
      continue ;

    // We need to know what entities are loaded so that we can overwrite
    // them later.
    m_symbols.push_back( es ) ;

  }

  return 0 ;
  //## end APP6APropertiesOperation::activate%403C7FF60280.body
}

int APP6APropertiesOperation::done ()
{
  //## begin APP6APropertiesOperation::done%403C800C039A.body preserve=yes
  if ( m_dlg )
    m_dlg->showWindow( false ) ;
  
  m_handler->deactivate( ) ;
  
  return 0 ;
  //## end APP6APropertiesOperation::done%403C800C039A.body
}

void APP6APropertiesOperation::deactivate ()
{
  //## begin APP6APropertiesOperation::deactivate%403C80130336.body preserve=yes
  m_propertiesOperationActive = false ;
  if ( m_dlg )
    m_dlg->showWindow( false ) ;  
  //## end APP6APropertiesOperation::deactivate%403C80130336.body
}

void APP6APropertiesOperation::requestHandler (TSLUserOperationRequest *handler)
{
  //## begin APP6APropertiesOperation::requestHandler%403C801A0141.body preserve=yes
  m_handler = handler ;
  //## end APP6APropertiesOperation::requestHandler%403C801A0141.body
}

void APP6APropertiesOperation::onPropertiesClose (unsigned long flags)
{
  //## begin APP6APropertiesOperation::onPropertiesClose%403C966A000E.body preserve=yes

  // Only do anything if this is the current operation,
  // since these callbacks can be triggered from the palette
  if ( !m_propertiesOperationActive )
    return ;

  // Called when the use clicks the close cross, or the ok
  // button within the properties window. All we need to do is
  // redirect the call to the done() method.
  
  // Check that this is not the result of our the done()s showWindow() method
  // or we will get in a loop. 
  if ( flags != APP6A_SHOW_WINDOW_CLOSE )
  {
    done() ;
  }

  //## end APP6APropertiesOperation::onPropertiesClose%403C966A000E.body
}

bool APP6APropertiesOperation::onPropertiesUpdate (unsigned long flags, const TSLAPP6ASymbol *symbol)
{
  //## begin APP6APropertiesOperation::onPropertiesUpdate%403C9F6E0201.body preserve=yes

  // Only do anything if this is the current operation,
  // since these callbacks can be triggered from the palette
  if ( !m_propertiesOperationActive )
    return true ;
  
  // Possible flags: 

  // APP6A_PROPS_UPDATING    
  // APP6A_PROPS_OK          
  // APP6A_PROPS_APPLY
  // APP6A_PROPS_UNDO
  if ( flags == APP6A_PROPS_OK || flags == APP6A_PROPS_APPLY || flags == APP6A_PROPS_UNDO )
  {
    // This method will be called once for each symbol
    // we have in the list. We want to overwrite
    // the correct item in the m_symbols vector and 
    // put the new one in its place.
  
    m_handler->displayHighlights( false ) ;

    int numInList = m_dlg ? m_dlg->getListSize() : 0 ;

    // This will match our selected symbols list.
    if ( numInList != (int)m_symbols.size() )
      return false ;
    
    for ( int ii = 0 ; ii < numInList ; ++ii )
    {
      // Grap the symbol.
      TSLAPP6ASymbol tmp ;
      if ( !m_dlg || !m_dlg->getSelectedSymbol( tmp, ii ) )
        return false ;
      
      // Convert it to an entityset.
      TSLEntitySet * es = getHelper()->getSymbolAsEntitySet( &tmp ) ;
        
      if ( !es )
        return false ;

      // Replace the old symbol with the new.
      TSLEntitySet * ent = (TSLEntitySet*) m_symbols[ii] ;
      TSLEnvelope oldExtent = ent->envelope() ;
      ent->clear() ;
      while ( es->size() != 0 )
        ent->insert( (*es)[0] ) ;
      
      // We need to redraw both extents because the symbol
      // may have got smaller.
      m_handler->updateEntityExtent ( ent ) ;

      TSLEnvelope newExtent = ent->envelope() ;

      m_handler->redraw2Extents ( &newExtent, &oldExtent, true ) ;

      m_handler->displayHighlights( true ) ;
    }
  }

  // As we have updated all the symbols we don't want
  // any more calls to this method. Stop them be returning false.
  return false ;
  //## end APP6APropertiesOperation::onPropertiesUpdate%403C9F6E0201.body
}

// Additional Declarations
  //## begin APP6APropertiesOperation%403C7FB000B2.declarations preserve=yes
void APP6APropertiesOperation::onPropertiesOpening (unsigned long flags)
{
  //called when the properties dialog is shown

}
  //## end APP6APropertiesOperation%403C7FB000B2.declarations

//## begin module%403C7FB000B2.epilog preserve=yes
//## end module%403C7FB000B2.epilog
