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
//  MODULE      : app6aselectoroperation.cpp
//  SUBSYSTEM   : <Top Level>
//  GEN DATE    : 25/02/2004
//  GEN TIME    : 15:53:10
//  ********************************************************
//## end module%403C7FD20101.cp

//## Module: APP6ASelectorOperation%403C7FD20101; Pseudo Package body
//## Source file: c:\secure\source\MapLink\EditorSDK\Samples\NT\APP6ASample\APP6AUserOperations\app6aselectoroperation.cpp

//## begin module%403C7FD20101.additionalIncludes preserve=no
//## end module%403C7FD20101.additionalIncludes

//## begin module%403C7FD20101.includes preserve=yes
#include "stdafx.h"
#include "app6aselectoroperation.h"
//## end module%403C7FD20101.includes

//## begin module%403C7FD20101.additionalDeclarations preserve=yes
//## end module%403C7FD20101.additionalDeclarations


// Class APP6ASelectorOperation 

APP6ASelectorOperation::APP6ASelectorOperation ()
  //## begin APP6ASelectorOperation::APP6ASelectorOperation%403C802000D1.hasinit preserve=no
      : m_handler(0),
        m_dlg(0)
  //## end APP6ASelectorOperation::APP6ASelectorOperation%403C802000D1.hasinit
  //## begin APP6ASelectorOperation::APP6ASelectorOperation%403C802000D1.initialization preserve=yes
        , m_selectorOperationActive( false )
//## end APP6ASelectorOperation::APP6ASelectorOperation%403C802000D1.initialization
{
  //## begin APP6ASelectorOperation::APP6ASelectorOperation%403C802000D1.body preserve=yes
  //## end APP6ASelectorOperation::APP6ASelectorOperation%403C802000D1.body
}


APP6ASelectorOperation::~APP6ASelectorOperation ()
{
  //## begin APP6ASelectorOperation::~APP6ASelectorOperation%403C802200DE.body preserve=yes
  delete m_dlg ;
  m_dlg = 0 ;
  //## end APP6ASelectorOperation::~APP6ASelectorOperation%403C802200DE.body
}



//## Other Operations (implementation)
int APP6ASelectorOperation::activate (void *data)
{
  //## begin APP6ASelectorOperation::activate%403C802400AF.body preserve=yes
  m_selectorOperationActive = true ;

  if ( !m_dlg )
  {

    TSLErrorStack::clear() ;

    CWnd * parent = (CWnd *) data ;
    HWND wnd = parent->GetSafeHwnd () ;
    m_dlg = new TSLAPP6ASelector ( this, false, wnd ) ;
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
  return 1 ;
  //## end APP6ASelectorOperation::activate%403C802400AF.body
}

int APP6ASelectorOperation::done ()
{
  //## begin APP6ASelectorOperation::done%403C80370305.body preserve=yes
  if ( m_dlg )
    m_dlg->showWindow( false ) ;
  
  m_handler->deactivate( ) ;

  return 1 ;
  //## end APP6ASelectorOperation::done%403C80370305.body
}

void APP6ASelectorOperation::requestHandler (TSLUserOperationRequest *handler)
{
  //## begin APP6ASelectorOperation::requestHandler%403C8045017F.body preserve=yes
  m_handler = handler ;
  //## end APP6ASelectorOperation::requestHandler%403C8045017F.body
}

void APP6ASelectorOperation::onSelectorClose (unsigned long flags)
{
  //## begin APP6ASelectorOperation::onSelectorClose%403C9681018E.body preserve=yes

  // Only do anything if this is the current operation,
  // since these callbacks can be triggered from the palette
  if ( !m_selectorOperationActive )
    return ;

  // Called when the use clicks the close cross, or the ok
  // button within the selector window. All we need to do is
  // redirect the call to the done() method.
  
  // Check that this is not the result of our the done()s showWindow() method
  // or we will get in a loop. 
  if ( flags != APP6A_SHOW_WINDOW_CLOSE )
  {
    done() ;
  }

  //## end APP6ASelectorOperation::onSelectorClose%403C9681018E.body
}

void APP6ASelectorOperation::getFilePath (CString &path, bool openDlg)
{
  //## begin APP6ASelectorOperation::getFilePath%403CAD3F02B2.body preserve=yes
  CFileDialog dlg(openDlg, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, _T("APP6A Palette Files (*.app6a)|*.app6a||") );
  
  if ( dlg.DoModal() == IDOK )
    path = (LPCTSTR)dlg.GetPathName() ;
  else
    path = "";
  //## end APP6ASelectorOperation::getFilePath%403CAD3F02B2.body
}

bool APP6ASelectorOperation::onSelectorUpdate (unsigned long flags, const TSLAPP6ASymbol *symbol)
{
  //## begin APP6ASelectorOperation::onSelectorUpdate%403CADDE0379.body preserve=yes

  // Only do anything if this is the current operation,
  // since these callbacks can be triggered from the palette
  if ( !m_selectorOperationActive )
    return true ;
  
  // This method demostrates how the symbol selector updates and 
  // also how to write symbols to a stream. We will open a stream
  // and save each symbol in turn to it.

  TSLAPP6ASymbol * sym = const_cast< TSLAPP6ASymbol * > ( symbol ) ;
    
  if ( flags == APP6A_FIRST || flags == APP6A_FIRST_AND_LAST )
  {
    // Get the location to save the symbols too.
    CString path = "" ;
    getFilePath( path, false ) ;
    if ( path != "" )
      m_stream.open ( TSLUTF8Encoder(path) ) ;
  }

  // Save it.
  sym->save ( m_stream ) ;
  
  // And close, if this is the last symbol.
  if ( flags == APP6A_LAST || flags == APP6A_FIRST_AND_LAST )
    m_stream.close () ;

  return true ;
  //## end APP6ASelectorOperation::onSelectorUpdate%403CADDE0379.body
}

// Additional Declarations
  //## begin APP6ASelectorOperation%403C7FD20101.declarations preserve=yes
void APP6ASelectorOperation::deactivate( )
{
  m_selectorOperationActive = false ;
}
  //## end APP6ASelectorOperation%403C7FD20101.declarations

//## begin module%403C7FD20101.epilog preserve=yes
//## end module%403C7FD20101.epilog
